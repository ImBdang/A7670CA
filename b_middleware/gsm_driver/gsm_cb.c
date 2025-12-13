#include "gsm_cb.h"

extern event_queue_t response_event_queue;

void at_basic_cb(gsm_cmd_state_t resp_state, const char *resp, uint8_t)
{
    uint16_t len = strlen(resp);
        switch (resp_state){

        case GSM_CMD_STATE_PRIMARY:{
            event_t event = {
                .response = EVT_PRIMARY
            };
            DEBUG_PRINT("<!>: ");
            DEBUG_PRINT("PRIMARY EVENT\r\n");
            push_event(&response_event_queue, event);
            break;
        }

        case GSM_CMD_STATE_OK:{
            event_t event = {
                .response = EVT_OK
            };
            DEBUG_PRINT("<!>: ");
            DEBUG_PRINT("EVENT OK\r\n");
            push_event(&response_event_queue, event);
            break;
        }

        case GSM_CMD_STATE_ERR:{
            event_t event = {
                .response = EVT_ERR
            };
            DEBUG_PRINT("<!>: ");
            DEBUG_PRINT("EVENT ERROR\r\n");
            push_event(&response_event_queue, event);
            break;
        }

        case GSM_CMD_STATE_TIMEOUT:{
            event_t event = {
                .response = EVT_TIMEOUT
            };
            DEBUG_PRINT("<!>: ");
            DEBUG_PRINT("EVENT TIMEOUT\r\n");
            push_event(&response_event_queue, event);
            break;
        }
    }   
}

void at_simple_cb(gsm_cmd_state_t resp_state, const char *resp, uint8_t len)
{
    (void)resp;
    (void)len;

    event_t event;

    switch (resp_state)
    {
    case GSM_CMD_STATE_OK:
        event.response = EVT_OK;
        DEBUG_PRINT("<!>: EVENT OK\r\n");
        break;

    case GSM_CMD_STATE_ERR:
        event.response = EVT_ERR;
        DEBUG_PRINT("<!>: EVENT ERROR\r\n");
        break;

    case GSM_CMD_STATE_TIMEOUT:
        event.response = EVT_TIMEOUT;
        DEBUG_PRINT("<!>: EVENT TIMEOUT\r\n");
        break;

    default:
        return;
    }
}

