#include "gsm_at_engine.h"

char promt_want_to_send[64];

extern lwrb_t at_rb;
extern uint8_t http_data_buff[HTTP_DATA_BUFFER];
extern http_state_t http_state;
extern uint32_t http_reading_chunk;
extern http_action_entity_t http_action_entity;

extern uint8_t mqtt_data_buff[128];

extern sms_state_t sms_state;

/* ====================================== STATIC DECLARATIONS ======================================= */
uint8_t* http_buff_ptr = http_data_buff;

static bool is_busy = false;
static gsm_at_cmd_t executing_cmd;

static bool mqtt_wait_payload = false;
static bool mqtt_wait_topic = false;
static uint8_t mqtt_data_collect_len = 0;

static void gsm_parser_process_line(void);
static void at_line_handle(const char* line);
static void at_response_handle(const char* line);
static void at_urc_handle(const char* line);
/* ================================================================================================== */

void set_promt_want_to_send(const char* promt)
{
    snprintf(promt_want_to_send, sizeof(promt_want_to_send), "%s", promt);
}

void gsm_at_process(void)
{
    gsm_parser_process_line();
    if (is_busy) {
        uint32_t now = get_systick();
        if (now - executing_cmd.start_tick >= executing_cmd.timeout) {
            is_busy = false;
            if (executing_cmd.callback){
                executing_cmd.callback(GSM_CMD_STATE_TIMEOUT,"TIMEOUT", 7);
                is_busy = false;
            }
        }
    }
}

bool send_at_cmd(gsm_at_cmd_t cmd)
{
    if (is_busy)
        return false;
    executing_cmd = cmd;
    is_busy = true;

    usart_sendstring(A7670C_USART, executing_cmd.cmd);
    usart_sendstring(A7670C_USART, "\r\n");
    DEBUG_PRINT(">> AT CMD: ");
    DEBUG_PRINT(executing_cmd.cmd);
    DEBUG_PRINT("\r\n");
    return true;
}

static void gsm_parser_process_line(void)
{
    static uint8_t line_buff[AT_LWRB_SIZE];
    static uint16_t line_len = 0;
    uint8_t c;
    while (lwrb_read(&at_rb, &c, 1)){
        if (http_state == HTTP_BUSY) {
            *http_buff_ptr++ = c;
            http_reading_chunk--;
            if (http_reading_chunk == 0) {
                http_state = HTTP_DONE;
            }
            continue; 
        }

        /*<! SET BY URC MQTTRX */
        if (mqtt_data_collect_len != 0)
        {
            mqtt_data_buff[line_len++] = c;
            mqtt_data_collect_len--;

            if (mqtt_data_collect_len == 0)
            {
                mqtt_data_buff[line_len] = '\0';

                if (mqtt_wait_topic)
                {
                    DEBUG_PRINT("MQTT TOPIC: %s\r\n", mqtt_data_buff);
                    mqtt_wait_topic = false;
                }

                if (mqtt_wait_payload)
                {
                    DEBUG_PRINT("MQTT PAYLOAD: %s\r\n", mqtt_data_buff);
                    mqtt_wait_payload = false;
                }

                line_len = 0; 
            }
            continue;
        }

        
        /*<! WHEN WANT TO SEND DATA */
        if (c == '>')
        {
            usart_sendstring(A7670C_USART, promt_want_to_send);
            usart_sendstring(A7670C_USART, "\r\n");
            DEBUG_PRINT(">> %s\r\n", promt_want_to_send);
            return;
        }

        if (line_len < sizeof(line_buff) - 1){
            if (c != 0xFF)
                line_buff[line_len++] = c;
        }
        if (c == '\n'){
            line_buff[line_len] = '\0';
            DEBUG_PRINT("RAW RESPONSE: %s\r\n", line_buff);
            if (sms_state == SMS_READING)
            {

                size_t len = strlen((char *)line_buff);
                if (len <= 2)   
                {
                    line_len = 0;
                    return;
                }
                if (strcmp((char *)line_buff, "OK\r\n") == 0 ||
                    strcmp((char *)line_buff, "OK") == 0)
                {
                    sms_state = SMS_IDLE;
                    at_line_handle((char *)line_buff);  
                    line_len = 0;
                    return;
                }

                set_new_sms_content((char *)line_buff);
                DEBUG_PRINT("SMS CONTENT: %s\r\n", line_buff);
                check_sms((char *)line_buff);

                sms_state = SMS_IDLE;

                line_len = 0;
                return;
            }

    
            else 
            {
                at_line_handle(line_buff);
            }
            line_len = 0;
        }
    }
}

bool is_urc(const char *line)
{
    if (!line) return false;
    while (*line == ' ' || *line == '\t')
        line++;

    if (*line == '\0')
        return false;

    if (*line == '+' || *line == '*' || *line == '^')
        return true;
    
    if (strncmp(line, "RDY", 3) == 0)
        return true;
    if (strncmp(line, "READY", 5) == 0)
        return true;
    
    if (strncmp(line, "SMS DONE", 8) == 0)
        return true;
    if (strncmp(line, "SMS READY", 9) == 0)
        return true;
    
    if (strncmp(line, "Call Ready", 10) == 0)
        return true;
    if (strncmp(line, "RING", 4) == 0)
        return true;
    if (strncmp(line, "NO CARRIER", 10) == 0)
        return true;
    if (strncmp(line, "BUSY", 4) == 0)
        return true;
    if (strncmp(line, "NO ANSWER", 9) == 0)
        return true;

    if (strncmp(line, "NORMAL POWER DOWN", 17) == 0)
        return true;
    if (strncmp(line, "UNDER-VOLTAGE POWER DOWN", 24) == 0)
        return true;
    if (strncmp(line, "UNDER-VOLTAGE WARNNING", 22) == 0)
        return true;
    if (strncmp(line, "OVER-VOLTAGE POWER DOWN", 23) == 0)
        return true;
    if (strncmp(line, "OVER-VOLTAGE WARNNING", 21) == 0)
        return true;
    
    if (strncmp(line, "PDP DEACT", 9) == 0)
        return true;
    
    if (strncmp(line, "SIM REMOVED", 11) == 0)
        return true;
    if (strncmp(line, "SIM INSERTED", 12) == 0)
        return true;

    if (strncmp(line, "GPS READY", 9) == 0)
        return true;
    
    if (strncmp(line, "DOWNLOAD", 8) == 0)
        return true;

    if (strncmp(line, "PB DONE", 7) == 0)
        return true;
    
    return false;
}

static void at_line_handle(const char* line)
{
    if (!is_urc(line))
    {
        at_response_handle(line);
   
    }
    else
    {
        at_urc_handle(line);
    }
    
}

void at_response_handle(const char* line_buff)
{
    if (!is_busy)
        return;
    /*<! If the line is empty */
    size_t len = strlen(line_buff);
    if (len == 0 || line_buff[0] == '\r' || line_buff[0] == '\n')
        return;
        
    /*<! PRIMARY response */
    if (executing_cmd.expect[0] != '\0' &&
        strncmp(line_buff, executing_cmd.expect, strlen(executing_cmd.expect)) == 0)
    {
        if (executing_cmd.callback)
            executing_cmd.callback(GSM_CMD_STATE_PRIMARY, line_buff, strlen(line_buff));
        return;
    }

    /*<! OK response */
    if (strstr(line_buff, "OK") != NULL)
    {
        is_busy = false;
        if (executing_cmd.callback)
            executing_cmd.callback(GSM_CMD_STATE_OK ,line_buff, 2);
        return;
    }

    /*<! ERROR response*/    
    if (strstr(line_buff, "ERROR") != NULL)
    {
        is_busy = false;
        if (executing_cmd.callback)
            executing_cmd.callback(GSM_CMD_STATE_ERR, line_buff, 5);

        return;
    }

    /*<! INTERMEDIATE response */
    if (executing_cmd.callback) {
        executing_cmd.callback(GSM_CMD_STATE_INTERMEDIATE, line_buff, strlen(line_buff));
    }
}

void at_urc_handle(const char* line)
{
    if (strstr(line, "+CPIN:"))
    {
        char status[12];
        gsm_parse_cpin(line, status);
        if (strstr(status, "READY"))
        {
            set_sim_state(SIM_READY);
        }
        else if (strstr(status, "PUK"))
        {
            set_sim_state(SIM_PUK);
        }
        else 
        {
            set_sim_state(SIM_PIN);
        }
    }

    if (strstr(line, "+CREG:"))
    {
        char status[8];
        gsm_parse_creg(line, status);
        uint8_t creg_val = fast_atoi(status);   

        switch (creg_val)
        {
            case 1:
                set_sim_creg_state(CREG_REGISTERED_HOME);
                break;

            case 5:
                set_sim_creg_state(CREG_REGISTERED_ROAMING);
                break;

            case 0:
                set_sim_creg_state(CREG_NOT_REGISTERED);
                break;

            case 2:
                set_sim_creg_state(CREG_SEARCHING);
                break;

            case 3:
                set_sim_creg_state(CREG_REG_DENIED);
                break;

            case 4:
                set_sim_creg_state(CREG_UNKNOWN);
                break;

            case 6:
                set_sim_creg_state(CREG_REGISTERED_SMS);
                break;

            case 7:
                set_sim_creg_state(CREG_REGISTERED_SMS_ROAM);
                break;
        }
    }

    if (strncmp(line, "+HTTPACTION:", 12) == 0)
    {
        uint8_t status;
        uint16_t method;
        uint32_t data_size;
        gsm_parse_httpget(line, &status, &method, &data_size);
        set_http_action(1, method, status, data_size);
        DEBUG_PRINT("HTTP STATUS: %u, HTTP DATA: %lu\r\n", status, data_size);
    }

    if (strncmp(line, "+HTTPREAD:", 10) == 0)
    {
        uint32_t tmp = 0;
        tmp = gsm_parse_httpread_len(line);
        if ( (tmp == 0) && (http_state == HTTP_DONE) )
        {
            http_state = HTTP_IDLE;
            http_action_entity.read_done = 1;
            http_buff_ptr = http_data_buff;
            return;
        }
        http_state = HTTP_BUSY;
    }

    if (strncmp(line, "+CMTI:", 6) == 0) 
    {
        uint8_t idx = 0;
        idx = new_sms_dispatch(line);
        DEBUG_PRINT("New SMS RECEIVED, INDEX IS: %u\r\n", idx);
        set_new_sms_idx(idx);
        read_sms(idx);
        delete_sms(idx);
    }

    if (strncmp(line, "+CMGR:", 6) == 0) 
    {
        sms_state = SMS_READING;
    }

    if (strncmp(line, "+CMQTTRXTOPIC:", 14) == 0)
    {
        uint8_t topic_len = mqtt_parse_rxtopic_len(line);
        mqtt_wait_topic = true;
        mqtt_data_collect_len += topic_len;
    }

    if (strncmp(line, "+CMQTTRXPAYLOAD:", 16) == 0)
    {
        uint8_t payload_len = mqtt_parse_rxpayload_len(line);
        mqtt_wait_payload = true;
        mqtt_data_collect_len += payload_len;
    }

}



