#include "gsm_mqtt.h"
#include "jsmn.h"
/* ====================================== GLOBAL VARIABLES ========================================== */

/* ================================================================================================== */


/* ====================================== STATIC VARIABLES ========================================== */
static uint8_t sharedStep = 0;
static uint8_t sharedTimeout_count = 0;
/* ================================================================================================== */

bool gsm_mqtt_start(void)
{
    bool tmp = false;
    switch (sharedStep)
    {
    case 0:
        {
            gsm_at_cmd_t at_cmd = {
                .cmd = "AT+CMQTTSTART",
                .expect = "",
                .start_tick = get_systick(),
                .timeout = 7000,
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
                    DEBUG_PRINT("MQTT OK\r\n");
                    sharedStep = 0;
                    return true;


                case EVT_TIMEOUT:
                    DEBUG_PRINT("MQTT TIMEOUT\r\n");
                    sharedStep = 0;
                    if (sharedTimeout_count >= 3){
                        sharedTimeout_count = 0;
                    }
                    return false;
                

                case EVT_ERR:
                    sharedStep = 0;
                    DEBUG_PRINT("MQTT ERR\r\n");
                    sharedTimeout_count = 0;
                    return false;
            }    
        }
    
    default:
        break;
    }
}

bool gsm_mqtt_stop(void)
{
    bool tmp = false;
    switch (sharedStep)
    {
    case 0:
        {
            gsm_at_cmd_t at_cmd = {
                .cmd = "AT+CMQTTSTOP",
                .expect = "",
                .start_tick = get_systick(),
                .timeout = 7000,
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
                    DEBUG_PRINT("MQTT STOPPED\r\n");
                    sharedStep = 0;
                    return true;


                case EVT_TIMEOUT:
                    DEBUG_PRINT("STOP MQTT TIMEOUT\r\n");
                    decision_flag.sync_at = false;
                    sharedStep = 0;
                    if (sharedTimeout_count >= 3){
                        sharedTimeout_count = 0;
                    }
                    return false;
                

                case EVT_ERR:
                    sharedStep = 0;
                    DEBUG_PRINT("STOP MQTT ERR\r\n");
                    sharedTimeout_count = 0;
                    return false;
            }    
        }
    
    default:
        return false;
    }
}

bool gsm_mqtt_acquire_client(uint8_t client_index, const char* client_id, uint8_t server_type)
{
    bool tmp = false;
    switch (sharedStep)
    {
    case 0:
        {
            char at_cmd_promt[64];
            snprintf(at_cmd_promt, sizeof(at_cmd_promt),
                "AT+CMQTTACCQ=%d,\"%s\",%d",
                client_index,
                client_id,
                server_type);

            gsm_at_cmd_t at_cmd = {
                .expect = "",
                .start_tick = get_systick(),
                .timeout = 7000,
                .callback = at_basic_cb
            };
            strncpy(at_cmd.cmd, at_cmd_promt, sizeof(at_cmd.cmd) - 1);
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
                    DEBUG_PRINT("CLIENT ACQUIRE\r\n");
                    sharedStep = 0;
                    return true;


                case EVT_TIMEOUT:
                    DEBUG_PRINT("CLIENT ACQUIRE TIMEOUT\r\n");
                    decision_flag.sync_at = false;
                    sharedStep = 0;
                    if (sharedTimeout_count >= 3){
                        sharedTimeout_count = 0;
                    }
                    return false;
                

                case EVT_ERR:
                    sharedStep = 0;
                    DEBUG_PRINT("CLIENT ACQUIRE ERR\r\n");
                    sharedTimeout_count = 0;
                    return false;
            }    
        }
    
    default:
        return false;
    }
}

bool gsm_mqtt_release_client(uint8_t client_index)
{
    bool tmp = false;
    switch (sharedStep)
    {
    case 0:
        {
            char at_cmd_promt[64];
            snprintf(at_cmd_promt, sizeof(at_cmd_promt),
                "AT+CMQTTREL=%d", client_index);

            gsm_at_cmd_t at_cmd = {
                .expect = "",
                .start_tick = get_systick(),
                .timeout = 7000,
                .callback = at_basic_cb
            };
            strncpy(at_cmd.cmd, at_cmd_promt, sizeof(at_cmd.cmd) - 1);
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
                    DEBUG_PRINT("AT+CMQTTREL OK\r\n");
                    sharedStep = 0;
                    return true;


                case EVT_TIMEOUT:
                    DEBUG_PRINT("AT+CMQTTREL TIMEOUT\r\n");
                    decision_flag.sync_at = false;
                    sharedStep = 0;
                    if (sharedTimeout_count >= 3){
                        sharedTimeout_count = 0;
                    }
                    return false;
                

                case EVT_ERR:
                    sharedStep = 0;
                    DEBUG_PRINT("AT+CMQTTREL ERR\r\n");
                    sharedTimeout_count = 0;
                    return false;
            }    
        }
    
    default:
        return false;
    }
}

bool gsm_mqtt_tls_config(uint8_t client_index, uint8_t ssl_index)
{
    bool tmp = false;
    switch (sharedStep)
    {
    case 0:
        {
            char at_cmd_promt[64];
            snprintf(at_cmd_promt, sizeof(at_cmd_promt),
                "AT+CMQTTSSLCFG=%d,%d",
                client_index, ssl_index);

            gsm_at_cmd_t at_cmd = {
                .expect = "",
                .start_tick = get_systick(),
                .timeout = 7000,
                .callback = at_basic_cb
            };
            strncpy(at_cmd.cmd, at_cmd_promt, sizeof(at_cmd.cmd) - 1);
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
                    DEBUG_PRINT("SSL CONFIG DONE\r\n");
                    sharedStep = 0;
                    return true;


                case EVT_TIMEOUT:
                    DEBUG_PRINT("SSL CONFIG TIMEOUT\r\n");
                    decision_flag.sync_at = false;
                    sharedStep = 0;
                    if (sharedTimeout_count >= 3){
                        sharedTimeout_count = 0;
                    }
                    return false;
                

                case EVT_ERR:
                    sharedStep = 0;
                    DEBUG_PRINT("SSL CONFIG ERR\r\n");
                    sharedTimeout_count = 0;
                    return false;
            }    
        }
    
    default:
        return false;
    }   
}

bool gsm_mqtt_sslver_config(uint8_t ssl_index, uint8_t mode)
{
    bool tmp = false;
    switch (sharedStep)
    {
    case 0:
        {
            char at_cmd_promt[64];
            snprintf(at_cmd_promt, sizeof(at_cmd_promt),
                "AT+CSSLCFG=\"sslversion\",%d,%d",
                ssl_index, mode);

            gsm_at_cmd_t at_cmd = {
                .expect = "",
                .start_tick = get_systick(),
                .timeout = 7000,
                .callback = at_basic_cb
            };
            strncpy(at_cmd.cmd, at_cmd_promt, sizeof(at_cmd.cmd) - 1);
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
                    DEBUG_PRINT("SSL CONFIG DONE\r\n");
                    sharedStep = 0;
                    return true;


                case EVT_TIMEOUT:
                    DEBUG_PRINT("SSL CONFIG TIMEOUT\r\n");
                    decision_flag.sync_at = false;
                    sharedStep = 0;
                    if (sharedTimeout_count >= 3){
                        sharedTimeout_count = 0;
                    }
                    return false;
                

                case EVT_ERR:
                    sharedStep = 0;
                    DEBUG_PRINT("SSL CONFIG ERR\r\n");
                    sharedTimeout_count = 0;
                    return false;
            }    
        }
    
    default:
        return false;
    }   
}

bool gsm_mqtt_connect_broker(uint8_t client_index, uint8_t keep_alive, bool clean)
{
    bool tmp = false;
    switch (sharedStep)
    {
    case 0:
        {
            char at_cmd_promt[128];

            snprintf(at_cmd_promt, sizeof(at_cmd_promt),
                    "AT+CMQTTCONNECT=%d,\"%s\",%u,%u,\"%s\",\"%s\"",
                    client_index,
                    MQTT_URL,
                    keep_alive,
                    clean ? 1 : 0,
                    MQTT_USER,
                    MQTT_PASS);



            gsm_at_cmd_t at_cmd = {
                .expect = "",
                .start_tick = get_systick(),
                .timeout = 7000,
                .callback = at_basic_cb
            };
            strncpy(at_cmd.cmd, at_cmd_promt, sizeof(at_cmd.cmd) - 1);
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
                    DEBUG_PRINT("MQTT BROKER CONNECTED\r\n");
                    sharedStep = 0;
                    return true;


                case EVT_TIMEOUT:
                    DEBUG_PRINT("MQTT BROKER TIMEOUT\r\n");
                    decision_flag.sync_at = false;
                    sharedStep = 0;
                    if (sharedTimeout_count >= 3){
                        sharedTimeout_count = 0;
                    }
                    return false;
                

                case EVT_ERR:
                    sharedStep = 0;
                    DEBUG_PRINT("MQTT BROKER ERR\r\n");
                    sharedTimeout_count = 0;
                    return false;
            }    
        }
    
    default:
        return false;
    }
}

bool check_time(void)
{
    bool tmp = false;
    switch (sharedStep)
    {
    case 0:
        {

            gsm_at_cmd_t at_cmd = {
                .cmd = "AT+CCLK?",
                .expect = "",
                .start_tick = get_systick(),
                .timeout = 7000,
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
                    DEBUG_PRINT("SSL CONFIG DONE\r\n");
                    sharedStep = 0;
                    return true;


                case EVT_TIMEOUT:
                    DEBUG_PRINT("SSL CONFIG TIMEOUT\r\n");
                    decision_flag.sync_at = false;
                    sharedStep = 0;
                    if (sharedTimeout_count >= 3){
                        sharedTimeout_count = 0;
                    }
                    return false;
                

                case EVT_ERR:
                    sharedStep = 0;
                    DEBUG_PRINT("SSL CONFIG ERR\r\n");
                    sharedTimeout_count = 0;
                    return false;
            }    
        }
    
    default:
        return false;
    }   
}

bool check_internet(void)
{
bool tmp = false;
    switch (sharedStep)
    {
    case 0:
        {

            gsm_at_cmd_t at_cmd = {
                .cmd = "AT+CPING=\"8.8.8.8\"",
                .expect = "",
                .start_tick = get_systick(),
                .timeout = 7000,
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
                    DEBUG_PRINT("SSL CONFIG DONE\r\n");
                    sharedStep = 0;
                    return true;


                case EVT_TIMEOUT:
                    DEBUG_PRINT("SSL CONFIG TIMEOUT\r\n");
                    decision_flag.sync_at = false;
                    sharedStep = 0;
                    if (sharedTimeout_count >= 3){
                        sharedTimeout_count = 0;
                    }
                    return false;
                

                case EVT_ERR:
                    sharedStep = 0;
                    DEBUG_PRINT("SSL CONFIG ERR\r\n");
                    sharedTimeout_count = 0;
                    return false;
            }    
        }
    
    default:
        return false;
    }   
}

bool gsm_mqtt_sni_config(uint8_t ssl_index, uint8_t client_index)
{
    bool tmp = false;
    switch (sharedStep)
    {
    case 0:
        {
            char at_cmd_promt[64];
            snprintf(at_cmd_promt, sizeof(at_cmd_promt),
                "AT+CSSLCFG=\"enableSNI\",%d,%d",
                ssl_index, client_index);

            gsm_at_cmd_t at_cmd = {
                .expect = "",
                .start_tick = get_systick(),
                .timeout = 7000,
                .callback = at_basic_cb
            };
            strncpy(at_cmd.cmd, at_cmd_promt, sizeof(at_cmd.cmd) - 1);
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
                    DEBUG_PRINT("SSL CONFIG DONE\r\n");
                    sharedStep = 0;
                    return true;


                case EVT_TIMEOUT:
                    DEBUG_PRINT("SSL CONFIG TIMEOUT\r\n");
                    decision_flag.sync_at = false;
                    sharedStep = 0;
                    if (sharedTimeout_count >= 3){
                        sharedTimeout_count = 0;
                    }
                    return false;
                

                case EVT_ERR:
                    sharedStep = 0;
                    DEBUG_PRINT("SSL CONFIG ERR\r\n");
                    sharedTimeout_count = 0;
                    return false;
            }    
        }
    
    default:
        return false;
    }    
}
