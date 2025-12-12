#include "gsm_sms.h"

/* ====================================== GLOBAL VARIABLES ========================================== */
const char phone_num[16] = "0837645067";
sms_entity_t sms_entity = {
    .idx = 0,
    .content = ""
};
sms_state_t sms_state = 0;
/* ================================================================================================== */

/* ====================================== STATIC VARIABLES ========================================== */
static uint8_t sharedStep = 0;
static uint8_t sharedTimeout_count = 0;
/* ================================================================================================== */

void set_new_sms_idx(uint8_t idx)
{
    sms_entity.idx = idx;
}

void set_new_sms_content(const char* content)
{
    strncpy(sms_entity.content, content, sizeof(sms_entity.content) - 1);
    sms_entity.content[sizeof(sms_entity.content) - 1] = '\0';
}

bool send_sms(const char* str, const char* phone)
{

}

void delete_sms(uint8_t idx)
{    
    bool tmp = false;
    char idx_str[4];
    fast_itoa(idx, idx_str);
    char cmd_str[32] = "AT+CMGD=";
    strcat(cmd_str, idx_str);

    gsm_at_cmd_t at_cmd;
    memset(&at_cmd, 0, sizeof(at_cmd));

    strncpy(at_cmd.cmd, cmd_str, sizeof(at_cmd.cmd) - 1);
    strncpy(at_cmd.expect, "", sizeof(at_cmd.expect) - 1);

    at_cmd.start_tick = get_systick();
    at_cmd.timeout    = 7000;
    at_cmd.callback   = at_basic_cb;

    tmp = send_at_cmd(at_cmd);
}

void read_sms(uint8_t idx)
{
    bool tmp = false;
    char idx_str[4];
    fast_itoa(idx, idx_str);
    switch (sharedStep)
    {
    case 0:
    {
        char cmd_str[32] = "AT+CMGR=";
        strcat(cmd_str, idx_str);

        gsm_at_cmd_t at_cmd;
        memset(&at_cmd, 0, sizeof(at_cmd));

        strncpy(at_cmd.cmd, cmd_str, sizeof(at_cmd.cmd) - 1);
        strncpy(at_cmd.expect, "", sizeof(at_cmd.expect) - 1);

        at_cmd.start_tick = get_systick();
        at_cmd.timeout    = 7000;
        at_cmd.callback   = at_simple_cb;

        send_at_cmd(at_cmd);
        break;
    }

    // case 1:
    // {
    //     event_t event;
    //     if (!pop_event(&response_event_queue, &event))
    //             return;

    //     switch (event.response)
    //     {
    //         case EVT_OK:
    //             sharedStep = 0;
    //             DEBUG_PRINT("READ SMS OK\r\n");
    //             break;

    //         default: 
    //             sharedStep = 0;
    //             DEBUG_PRINT("READ SMS FAIL\r\n");
    //             break;
    //     }    
        
    // }

    // case 2:
    // {
    //     char cmd_str[32] = "AT+CMGD=";
    //     strcat(cmd_str, idx_str);

    //     gsm_at_cmd_t at_cmd;
    //     memset(&at_cmd, 0, sizeof(at_cmd));

    //     strncpy(at_cmd.cmd, cmd_str, sizeof(at_cmd.cmd) - 1);
    //     strncpy(at_cmd.expect, "", sizeof(at_cmd.expect) - 1);

    //     at_cmd.start_tick = get_systick();
    //     at_cmd.timeout    = 7000;
    //     at_cmd.callback   = at_basic_cb;

    //     tmp = send_at_cmd(at_cmd);

    //     if (tmp) sharedStep++;
    //     break;
    // }

    // case 3:
    // {
    //     event_t event;
    //     if (!pop_event(&response_event_queue, &event))
    //             return;

    //     switch (event.response)
    //     {
    //         case EVT_OK:
    //             sharedStep = 0;
    //             DEBUG_PRINT("SMS DELETED\r\n");
    //             break;

    //         default:
    //             sharedStep = 0;
    //             DEBUG_PRINT("SMS NOT DELETED\r\n");
    //             break;
    //     }   
    // }

    default:
        break;
    }
}


void check_sms(const char* str)
{
    if (sms_entity.content[0] == '\0')
        return;

    if (strcmp(sms_entity.content, "OTA") == 0)
    {
        DEBUG_PRINT("OTA CMD DETECT");
        set_cur_task(OTA_TASK);
    }
    else if (strcmp(sms_entity.content, "LED13 ON") == 0)
    {
        DEBUG_PRINT("LED13 TURNING ON");
        GPIO_ResetBits(GPIOC, GPIO_Pin_13);
    }
    else if (strcmp(sms_entity.content, "LED13 OFF") == 0)
    {
        DEBUG_PRINT("LED13 TURNING ON");
        GPIO_SetBits(GPIOC, GPIO_Pin_13);
    }
}

void gsm_common_delete_all_sms(void)
{
    bool tmp = false;
    event_t event;

    switch (sharedStep)
    {

    case 0:
    {
        gsm_at_cmd_t at_cmd = {
            .cmd = "AT+CMGF=1",
            .expect = "",
            .start_tick = get_systick(),
            .timeout = 5000,
            .callback = at_basic_cb
        };

        if (send_at_cmd(at_cmd))
            sharedStep++;
        break;
    }

    case 1:
    {
        if (!pop_event(&response_event_queue, &event))
            return;

        if (event.response == EVT_OK)
        {
            sharedStep++;
        }
        else
        {
            DEBUG_PRINT("CMGF FAIL\r\n");
            goto _fail;
        }
        break;
    }

    case 2:
    {
        gsm_at_cmd_t at_cmd = {
            .cmd = "AT+CMGD=1,4",
            .expect = "",
            .start_tick = get_systick(),
            .timeout = 7000,
            .callback = at_basic_cb
        };

        if (send_at_cmd(at_cmd))
            sharedStep++;
        break;
    }

    case 3:
    {
        if (!pop_event(&response_event_queue, &event))
            return;

        if (event.response == EVT_OK)
        {
            sharedStep++;
        }
        else
        {
            DEBUG_PRINT("CMGD FAIL\r\n");
            goto _fail;
        }
        break;
    }

    case 4:
    {
        gsm_at_cmd_t at_cmd = {
            .cmd = "AT+CPMS=\"SM\",\"SM\",\"SM\"",
            .expect = "",
            .start_tick = get_systick(),
            .timeout = 7000,
            .callback = at_basic_cb
        };

        if (send_at_cmd(at_cmd))
            sharedStep++;
        break;
    }

    case 5:
    {
        if (!pop_event(&response_event_queue, &event))
            return;

        if (event.response == EVT_OK)
        {
            sharedStep++;
        }
        else
        {
            DEBUG_PRINT("CPMS FAIL\r\n");
            goto _fail;
        }
        break;
    }

    case 6:
    {
        gsm_at_cmd_t at_cmd = {
            .cmd = "AT+CNMI=2,1,0,0,0",
            .expect = "",
            .start_tick = get_systick(),
            .timeout = 5000,
            .callback = at_basic_cb
        };

        if (send_at_cmd(at_cmd))
            sharedStep++;
        break;
    }

    case 7:
    {
        if (!pop_event(&response_event_queue, &event))
            return;

        if (event.response == EVT_OK)
        {
            DEBUG_PRINT("DELETE ALL SMS DONE\r\n");
            sharedStep = 0;
            gsm_state = GSM_STATE_DECISION;
        }
        else
        {
            DEBUG_PRINT("CNMI FAIL\r\n");
            goto _fail;
        }
        break;
    }

    default:
        break;
    }

    return;

_fail:
    sharedStep = 0;
    gsm_state = GSM_STATE_DECISION;
}

