#include "gsm_mqtt.h"
#include "jsmn.h"
/* ====================================== GLOBAL VARIABLES ========================================== */
uint8_t mqtt_data_buff[128];
/* ================================================================================================== */


/* ====================================== STATIC VARIABLES ========================================== */
static uint8_t sharedStep = 0;
static uint8_t sharedTimeout_count = 0;


static bool gsm_mqtt_start(void);
static bool gsm_mqtt_stop(void);

static bool gsm_mqtt_acquire_client(uint8_t client_index, const char *client_id, uint8_t server_type);
static bool gsm_mqtt_release_client(uint8_t client_index);

static bool gsm_mqtt_tls_config(uint8_t client_index, uint8_t ssl_index);
static bool gsm_mqtt_sslver_config(uint8_t ssl_index, uint8_t mode);
static bool gsm_mqtt_sni_config(uint8_t ssl_index, uint8_t client_index);

static bool gsm_mqtt_connect_broker(uint8_t client_index, uint8_t keep_alive, bool clean);
static bool gsm_mqtt_disconnect_broker(uint8_t client_index);
/* ================================================================================================== */



static bool gsm_mqtt_start(void)
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

static bool gsm_mqtt_stop(void)
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

static bool gsm_mqtt_acquire_client(uint8_t client_index, const char* client_id, uint8_t server_type)
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

static bool gsm_mqtt_release_client(uint8_t client_index)
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

static bool gsm_mqtt_tls_config(uint8_t client_index, uint8_t ssl_index)
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

static bool gsm_mqtt_sslver_config(uint8_t ssl_index, uint8_t mode)
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

static bool gsm_mqtt_connect_broker(uint8_t client_index, uint8_t keep_alive, bool clean)
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

static bool gsm_mqtt_sni_config(uint8_t ssl_index, uint8_t client_index)
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

bool gsm_mqtt_sub(uint8_t client_index,
                  uint8_t req_len,
                  uint8_t qos)
{
    static uint8_t step = 0;
    bool tmp = false;

    switch (step)
    {
        case 0:
        {
            char cmd[64];
            snprintf(cmd, sizeof(cmd),
                     "AT+CMQTTSUB=%u,%u,%u",
                     client_index, req_len, qos);

            gsm_at_cmd_t at = {
                .start_tick = get_systick(),
                .timeout = 240000,
                .callback = at_basic_cb
            };

            strncpy(at.cmd, cmd, sizeof(at.cmd)-1);

            tmp = send_at_cmd(at);
            if (tmp)
            {
                step = 1;
                set_promt_want_to_send(MQTT_CONTROL_TOPIC);
            }
            return false;
        }

        case 1:
        {
            event_t evt;
            if (!pop_event(&response_event_queue, &evt))
                return false;

            if (evt.response == EVT_OK)
            {
                DEBUG_PRINT("MQTT SUB SUCCESS\n");
                step = 0;
                return true;
            }

            DEBUG_PRINT("MQTT SUB FAIL\n");
            step = 0;
            return false;
        }
    }

    return false;
}

// bool gsm_mqtt_subscribe(uint8_t client_index,
//                         const char *topic,
//                         uint16_t msg_id,
//                         uint8_t qos)
// {
//     static uint8_t step = 0;
//     switch (step)
//     {
//         case 0:
//             if (gsm_mqtt_sub(client_index, msg_id, qos))
//                 step = 1;
//             return false;

//         case 1:
//             if (gsm_mqtt_subtopic(client_index, topic, qos))
//             {
//                 DEBUG_PRINT("FULL SUBSCRIBE DONE\r\n");
//                 step = 0;
//                 return true;
//             }
//             return false;
//     }

//     return false;
// }


bool gsm_mqtt_publish(uint8_t client_index,
                      const char *topic,
                      const char *payload,
                      uint8_t qos,
                      uint8_t retain)
{
    static uint8_t step = 0;
    static uint16_t topic_len, payload_len;
    bool tmp = false;

    switch (step)
    {
        case 0:
        {
            topic_len = strlen(topic);

            char cmd[48];
            snprintf(cmd, sizeof(cmd),
                     "AT+CMQTTTOPIC=%d,%u",
                     client_index, topic_len);

            gsm_at_cmd_t at = {
                .start_tick = get_systick(),
                .timeout = 7000,
                .callback = at_basic_cb
            };
            strncpy(at.cmd, cmd, sizeof(at.cmd) - 1);

            tmp = send_at_cmd(at);
            if (tmp)
                step = 1;
            return false;
        }

        case 1:
        {
            // send_raw((uint8_t*)topic, topic_len);
            // send_raw((uint8_t*)"\r\n", 2);
            step = 2;
            return false;
        }

        case 2:
        {
            payload_len = strlen(payload);

            char cmd[48];
            snprintf(cmd, sizeof(cmd),
                     "AT+CMQTTPAYLOAD=%d,%u",
                     client_index, payload_len);

            gsm_at_cmd_t at = {
                .start_tick = get_systick(),
                .timeout = 7000,
                .callback = at_basic_cb
            };
            strncpy(at.cmd, cmd, sizeof(at.cmd) - 1);

            tmp = send_at_cmd(at);
            if (tmp)
                step = 3;
            return false;
        }

        case 3:
        {
            // send_raw((uint8_t*)payload, payload_len);
            // send_raw((uint8_t*)"\r\n", 2);
            step = 4;
            return false;
        }

        case 4:
        {
            char cmd[48];
            snprintf(cmd, sizeof(cmd),
                     "AT+CMQTTPUB=%d,%u,%u",
                     client_index, qos, retain);

            gsm_at_cmd_t at = {
                .start_tick = get_systick(),
                .timeout = 7000,
                .callback = at_basic_cb
            };
            strncpy(at.cmd, cmd, sizeof(at.cmd) - 1);

            tmp = send_at_cmd(at);
            if (tmp)
                step = 5;
            return false;
        }

        case 5:
        {
            event_t evt;
            if (!pop_event(&response_event_queue, &evt))
                return false;

            if (evt.response == EVT_OK)
            {
                DEBUG_PRINT("PUB OK\r\n");
                step = 0;
                return true;
            }

            DEBUG_PRINT("PUB FAIL\r\n");
            step = 0;
            return false;
        }
    }
    return false;
}


bool gsm_mqtt_process(uint8_t client_index,
                    const char *client_id,
                    uint8_t server_type,
                    uint8_t ssl_index,
                    uint8_t sslversion_mode,
                    uint8_t enable_sni,
                    uint8_t keep_alive,
                    bool clean_session)
{
    static uint8_t mqtt_step = 0;
    static uint8_t mqtt_timeout_count = 0;

    bool tmp = false;

    switch (mqtt_step)
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
            if (tmp) mqtt_step = 1;
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
                    DEBUG_PRINT("CMQTTSTART OK\r\n");
                    mqtt_timeout_count = 0;
                    mqtt_step = 2;
                    return false;

                case EVT_TIMEOUT:
                    DEBUG_PRINT("CMQTTSTART TIMEOUT\r\n");
                    mqtt_step = 0;
                    if (++mqtt_timeout_count >= 3) mqtt_timeout_count = 0;
                    return false;

                case EVT_ERR:
                default:
                    DEBUG_PRINT("CMQTTSTART ERR\r\n");
                    mqtt_step = 0;
                    mqtt_timeout_count = 0;
                    return false;
            }
        }

        case 2:
        {
            if (gsm_mqtt_acquire_client(client_index, client_id, server_type))
                mqtt_step = 3;
            return false;
        }

        case 3:
        {
            if (gsm_mqtt_sslver_config(ssl_index, sslversion_mode))
                mqtt_step = 4;
            return false;
        }

        case 4:
        {
            if (gsm_mqtt_sni_config(ssl_index, enable_sni))
                mqtt_step = 5;
            return false;
        }

        case 5:
        {
            if (gsm_mqtt_tls_config(client_index, ssl_index))
                mqtt_step = 6;
            return false;
        }

        case 6:
        {
            if (gsm_mqtt_connect_broker(client_index, keep_alive, clean_session))
            {
                DEBUG_PRINT("MQTT START DONE\r\n");
                mqtt_step = 0;
                mqtt_timeout_count = 0;
                return true;
            }
            return false;
        }

        default:
            mqtt_step = 0;
            mqtt_timeout_count = 0;
            return false;
    }
}

static bool gsm_mqtt_disconnect_broker(uint8_t client_index)
{
    bool tmp = false;
    switch (sharedStep)
    {
    case 0:
        {
            char at_cmd_promt[128];

            snprintf(at_cmd_promt, sizeof(at_cmd_promt),
                    "AT+CMQTTDISC=%u,0",
                    client_index);
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
                    sharedStep = 0;
                    return true;


                case EVT_TIMEOUT:
                    sharedStep = 0;
                    if (sharedTimeout_count >= 3){
                        sharedTimeout_count = 0;
                    }
                    return false;
                

                case EVT_ERR:
                    sharedStep = 0;
                    sharedTimeout_count = 0;
                    return false;
            }    
        }
    
    default:
        return false;
    }
}

bool gsm_mqtt_close(uint8_t client_index)
{
    static uint8_t mqtt_close_step = 0;
    bool tmp = false;
    switch (mqtt_close_step)
    {
    case 0:
        tmp = gsm_mqtt_disconnect_broker(client_index);
        if (tmp)
            mqtt_close_step = 1;
        return false;

    case 1:
        tmp = gsm_mqtt_release_client(client_index);
        if (tmp)
            mqtt_close_step = 2;
        return false;
    
    case 2:
        if (gsm_mqtt_stop())
        {
            mqtt_close_step = 0;
            return true;
        }
        return false;
    
    default:
        return false;
    }
}

