#include "gsm_http.h"

/* ====================================== GLOBAL VARIABLES ========================================== */
uint8_t http_data_buff[HTTP_DATA_BUFFER];               /*<! HTTP READ BFFER TO STORE */
http_state_t http_state = HTTP_IDLE;
uint32_t http_reading_chunk = 0;
/* ================================================================================================== */


/* ====================================== STATIC DECLARATIONS ======================================= */
static uint8_t sharedStep = 0;
/* ================================================================================================== */


bool gsm_http_init(void)
{
    bool tmp = false;
    switch (sharedStep)
    {
    case 0:
    {
        gsm_at_cmd_t at_cmd = {
            .cmd = "AT+HTTPINIT",
            .expect = "",
            .start_tick = get_systick(),
            .timeout = 10000,
            .callback = at_basic_cb
        };
        tmp = send_at_cmd(at_cmd);
        if (tmp)
        {
            sharedStep++;
        }
        return false;
    }
    
    case 1:
    { 
        event_t event;
        if (!pop_event(&response_event_queue, &event))
            return false;

        switch (event.response)
        {
            case EVT_OK:
                DEBUG_PRINT("HTTP INIT OK\r\n");
                sharedStep = 0;
                return true;


            case EVT_TIMEOUT:
                DEBUG_PRINT("HTTP INIT TIMEOUT\r\n");
                sharedStep = 0;
                return false;
            

            case EVT_ERR:
                sharedStep = 0;
                DEBUG_PRINT("HTTP INIT ERR\r\n");
                return false;
        }    
    }
    default:
        break;
    }
}

bool gsm_http_term(void)
{
    bool tmp = false;
    switch (sharedStep)
    {
    case 0:
    {
        gsm_at_cmd_t at_cmd = {
            .cmd = "AT+HTTPTERM",
            .expect = "",
            .start_tick = get_systick(),
            .timeout = 10000,
            .callback = at_basic_cb
        };
        tmp = send_at_cmd(at_cmd);
        if (tmp)
        {
            sharedStep++;
        }
        return false;
    }
    
    case 1:
    { 
        event_t event;
        if (!pop_event(&response_event_queue, &event))
            return false;

        switch (event.response)
        {
            case EVT_OK:
                DEBUG_PRINT("HTTP TERM OK\r\n");
                sharedStep = 0;
                return true;


            case EVT_TIMEOUT:
                DEBUG_PRINT("HTTP TERM TIMEOUT\r\n");
                sharedStep = 0;
                return false;
            

            case EVT_ERR:
                sharedStep = 0;
                DEBUG_PRINT("HTTP TERM ERR\r\n");
                return false;
        }    
    }
    default:
        break;
    }
}

bool gsm_http_set_url(const char* url)
{
    bool tmp = false;
    switch (sharedStep)
    {
        case 0:
        {
            gsm_at_cmd_t at_cmd = {
                .expect = "",
                .timeout = 10000,
                .start_tick = get_systick(),
                .callback = at_basic_cb
            };

            snprintf(at_cmd.cmd, sizeof(at_cmd.cmd), 
                    "AT+HTTPPARA=\"URL\",\"%s\"", url);

            tmp = send_at_cmd(at_cmd);

            if (tmp)
                sharedStep++;

            return false;   
        }

        case 1:
        {
            event_t event;

            if (!pop_event(&response_event_queue, &event))
                return false;  

            switch (event.response)
            {
                case EVT_OK:
                    DEBUG_PRINT("HTTP SET URL OK\r\n");
                    sharedStep = 0;
                    return true;

                case EVT_TIMEOUT:
                    DEBUG_PRINT("HTTP SET URL TIMEOUT\r\n");
                    sharedStep = 0;
                    return false;

                case EVT_ERR:
                    DEBUG_PRINT("HTTP SET URL ERR\r\n");
                    sharedStep = 0;
                    return false;
            }
            break;
        }
    }

    return false;
}

bool gsm_http_action(uint8_t action)
{
    bool tmp = false;

    switch (sharedStep)
    {
        case 0:
        {
            gsm_at_cmd_t at_cmd = {
                .expect = "+HTTPACTION:",
                .timeout = 10000,
                .start_tick = get_systick(),
                .callback = at_basic_cb
            };

            char action_str[4];
            fast_itoa(action, action_str);

            snprintf(at_cmd.cmd, sizeof(at_cmd.cmd),
                     "AT+HTTPACTION=%s", action_str);

            tmp = send_at_cmd(at_cmd);

            if (tmp)
                sharedStep++;

            return false;   
        }

        case 1:
        {
            event_t event;

            if (!pop_event(&response_event_queue, &event))
                return false;

            switch (event.response)
            {
                case EVT_OK:
                    DEBUG_PRINT("HTTP ACTION OK\r\n");
                    sharedStep = 0;
                    return true;

                case EVT_ERR:
                    DEBUG_PRINT("HTTP ACTION ERR\r\n");
                    sharedStep = 0;
                    return false;

                case EVT_TIMEOUT:
                    DEBUG_PRINT("HTTP ACTION TIMEOUT\r\n");
                    sharedStep = 0;
                    return false;
            }
            break;
        }
    }

    return false;
}

bool gsm_http_read(uint32_t offset, uint32_t chunk)
{
    static uint8_t timeout_count = 0;
    const uint8_t max_timeout = 3;
    bool tmp = false;

    switch (sharedStep)
    {
        case 0: 
        {
            static char cmd_buf[32];
            static char offset_str[12];
            static char chunk_str[12];

            fast_itoa(offset, offset_str);
            fast_itoa(chunk, chunk_str);

            snprintf(cmd_buf, sizeof(cmd_buf),
                     "AT+HTTPREAD=%s,%s",
                     offset_str, chunk_str);

            gsm_at_cmd_t at_cmd = {
                .expect = "",
                .timeout = 12000,
                .start_tick = get_systick(),
                .callback = at_basic_cb
            };

            strcpy(at_cmd.cmd, cmd_buf);
            tmp = send_at_cmd(at_cmd);

            if (tmp)
            {
                sharedStep++;
                http_reading_chunk = chunk;     
            }
            return false;          
        }

        case 1:     
        {
            event_t event;

            if (!pop_event(&response_event_queue, &event))
                return false;      

            switch (event.response)
            {
                case EVT_OK:
                    DEBUG_PRINT("HTTPREAD OK\r\n");
                    timeout_count = 0;
                    sharedStep = 0;     
                    return true;

                case EVT_TIMEOUT:
                    DEBUG_PRINT("HTTPREAD TIMEOUT\r\n");
                    timeout_count++;
                    if (timeout_count >= max_timeout) {
                        timeout_count = 0;
                        sharedStep = 0;
                        return false;
                    }

                    sharedStep = 0;
                    return false;

                case EVT_ERR:
                    DEBUG_PRINT("HTTPREAD ERROR\r\n");
                    timeout_count = 0;
                    sharedStep = 0;
                    return false;
            }
            break;
        }
    }

    return false;
}


