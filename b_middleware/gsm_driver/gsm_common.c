#include "gsm_common.h"

extern gsm_decision_flag_t decision_flag;
extern gsm_state_t gsm_state;
extern sim_entity_t sim_entity;

/* ====================================== STATIC DECLARATIONS ======================================= */
static uint8_t sharedStep = 0;
static uint8_t sharedTimeout_count = 0;

/* ================================================================================================== */

void gsm_common_sync_at(void)
{
    bool tmp = false;
    switch (sharedStep)
    {
    case 0:
        {
            gsm_at_cmd_t at_cmd = {
                .cmd = "AT",
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
            break;
        }
    case 1:
        {   
            event_t event;
            if (!pop_event(&response_event_queue, &event))
                    return;

            switch (event.response)
            {
                case EVT_OK:
                    DEBUG_PRINT("AT OK\r\n");
                    sharedStep = 0;
                    decision_flag.sync_at = true;
                    gsm_state = GSM_STATE_DECISION;
                    break;


                case EVT_TIMEOUT:
                    DEBUG_PRINT("AT TIMEOUT\r\n");
                    decision_flag.sync_at = false;
                    sharedStep = 0;
                    if (sharedTimeout_count >= 3){
                        decision_flag.error = true;
                        sharedTimeout_count = 0;
                    }
                    gsm_state = GSM_STATE_DECISION;
                    break;
                

                case EVT_ERR:
                    sharedStep = 0;
                    DEBUG_PRINT("AT ERR\r\n");
                    sharedTimeout_count = 0;
                    decision_flag.error = true;
                    gsm_state = GSM_STATE_DECISION;
                    break;
            }    
        }
    
    default:
        break;
    }
}

void gsm_common_config(void)
{
    bool tmp = false;
    switch (sharedStep)
    {
    case 0:
    {
        gsm_at_cmd_t at_cmd = {
            .cmd = "AT+CFUN=1",
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
        break;
    }
    case 1:
    {   
        event_t event;
        if (!pop_event(&response_event_queue, &event))
                return;

        switch (event.response)
        {
            case EVT_OK:
                DEBUG_PRINT("CFUN=1 OK\r\n");
                sharedStep = 0;
                decision_flag.config = true;
                gsm_state = GSM_STATE_DECISION;
                break;


            case EVT_TIMEOUT:
                DEBUG_PRINT("CFUN=1 TIMEOUT\r\n");
                decision_flag.config = false;
                sharedStep = 0;
                if (sharedTimeout_count >= 3){
                    decision_flag.error = true;
                    sharedTimeout_count = 0;
                }
                gsm_state = GSM_STATE_DECISION;
                break;
            

            case EVT_ERR:
                sharedStep = 0;
                DEBUG_PRINT("CFUN=1 ERR\r\n");
                sharedTimeout_count = 0;
                decision_flag.error = true;
                gsm_state = GSM_STATE_DECISION;
                break;
        }    
    }

    default:
        break;
    }
}

void gsm_common_check_sim(void)
{
    bool tmp = false;
    switch (sharedStep)
    {
    case 0:
    {
        gsm_at_cmd_t at_cmd = {
            .cmd = "AT+CPIN?",
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
        break;
    }
    case 1:
    {   
        event_t event;
        if (!pop_event(&response_event_queue, &event))
            return;
        switch (sim_entity.state)
        {
            case SIM_UNKNOW:
            {
                DEBUG_PRINT("SIM UNDEFINED\r\n");
                sharedStep = 0;
                decision_flag.check_sim = false;
                gsm_state = GSM_STATE_DECISION;
                break;
            }

            case SIM_PIN:
            {
                DEBUG_PRINT("SIM PIN\r\n");
                sharedStep = 0;
                decision_flag.check_sim = false;
                gsm_state = GSM_STATE_DECISION;
                sharedTimeout_count = 0;
                break;
            }

            case SIM_PUK:
            {
                DEBUG_PRINT("SIM PUK\r\n");
                sharedStep = 0;
                decision_flag.check_sim = false;
                gsm_state = GSM_STATE_DECISION;
                sharedTimeout_count = 0;
                break;
            }

            case SIM_READY:
            {
                DEBUG_PRINT("SIM READY\r\n");
                sharedStep = 0;
                decision_flag.check_sim = true;
                gsm_state = GSM_STATE_DECISION;
                sharedTimeout_count = 0;
                break;
            }
        }    
    }

    default:
        break;
    }
}

void gsm_common_check_network(void)
{
    bool tmp = false;
    switch (sharedStep)
    {
    case 0:
    {
        gsm_at_cmd_t at_cmd = {
            .cmd = "AT+CREG?",
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
        break;
    }
    case 1:
    {
        event_t event;
        if (!pop_event(&response_event_queue, &event))
            return;
        switch (sim_entity.creg)
        {
            case CREG_REGISTERED_HOME:
            {
                DEBUG_PRINT("CREG: REGISTERED HOME\r\n");
                sharedStep = 0;
                decision_flag.reg_network = true;
                gsm_state = GSM_STATE_DECISION;
                sharedTimeout_count = 0;
                break;
            }

            case CREG_REGISTERED_ROAMING: 
            {
                DEBUG_PRINT("CREG: REGISTERED ROAMING\r\n");
                sharedStep = 0;
                decision_flag.reg_network = true;
                gsm_state = GSM_STATE_DECISION;
                sharedTimeout_count = 0;
                break;
            }

            default:
            {
                DEBUG_PRINT("CREG: 0,%d\r\n", sim_entity.creg);
                sharedStep = 0;
                decision_flag.reg_network = false;
                gsm_state = GSM_STATE_DECISION;
                break;
            }
        }
    }
    default:
        break;
    }
}

void gsm_common_check_gprs(void)
{
    bool tmp = false;
    switch (sharedStep)
    {
    case 0:
        {
            gsm_at_cmd_t at_cmd = {
                .cmd = "AT+CGATT=1",
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
            break;
        }
    case 1:
    {
        event_t event;
        if (!pop_event(&response_event_queue, &event))
            return;

        switch (event.response)
        {
            case EVT_OK:
            {
                sharedStep = 0;
                DEBUG_PRINT("CGATT=1 OK\r\n");
                decision_flag.attach_gprs = true;  
                sharedTimeout_count = 0;

                gsm_state = GSM_STATE_DECISION;
                break;
            }

            case EVT_TIMEOUT:
            {
                sharedStep = 0;
                DEBUG_PRINT("CGATT=1 TIMEOUT\r\n");
                decision_flag.attach_gprs = false;
                sharedTimeout_count++;

                if (sharedTimeout_count >= 3) {
                    decision_flag.error = true;
                    sharedTimeout_count = 0;
                }

                gsm_state = GSM_STATE_DECISION;
                break;
            }

            case EVT_ERR:
            {
                sharedStep = 0;
                DEBUG_PRINT("CGATT=1 ERR\r\n");
                decision_flag.attach_gprs = false;
                decision_flag.error = true;
                sharedTimeout_count = 0;

                gsm_state = GSM_STATE_DECISION;
                break;
            }
        }
        break;
    }

    
    default:
        break;
    }
}

void gsm_common_init_pdp(void)
{
    bool tmp = false;
    switch (sharedStep)
    {
    case 0:
        {
            gsm_at_cmd_t at_cmd = {
                .cmd = "AT+CGDCONT=1,\"IP\",\"v-internet\"",
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
            break;
        }
    case 1:
    {
        event_t event;
        if (!pop_event(&response_event_queue, &event))
            return;

        switch (event.response)
        {
            case EVT_OK:
            {
                sharedStep++;    
                DEBUG_PRINT("CGDCONT OK\r\n");
                sharedTimeout_count = 0;
                break;
            }

            case EVT_TIMEOUT:
            {
                sharedStep = 0;
                DEBUG_PRINT("CGDCONT TIMEOUT\r\n");
                decision_flag.activate_pdp = false;
                sharedTimeout_count++;
                if (sharedTimeout_count >= 3) {
                    decision_flag.error = true;
                    sharedTimeout_count = 0;
                }
                gsm_state = GSM_STATE_DECISION;
                break;
            }

            case EVT_ERR:
            {
                sharedStep = 0;
                DEBUG_PRINT("CGDCONT ERR\r\n");

                decision_flag.activate_pdp = false;
                decision_flag.error = true;
                sharedTimeout_count = 0;

                gsm_state = GSM_STATE_DECISION;
                break;
            }
        }
        break;
    }


    case 2:
    {
        gsm_at_cmd_t at_cmd = {
            .cmd = "AT+CGACT=1,1",
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
        break;
    }
    case 3:
    {
        event_t event;
        if (!pop_event(&response_event_queue, &event))
            return;

        switch (event.response)
        {
            case EVT_OK:
            {
                sharedStep = 0;
                DEBUG_PRINT("CGACT OK\r\n");

                decision_flag.activate_pdp = true;
                sharedTimeout_count = 0;

                gsm_state = GSM_STATE_DECISION;
                break;
            }

            case EVT_TIMEOUT:
            {
                sharedStep = 0;
                DEBUG_PRINT("CGACT TIMEOUT\r\n");

                decision_flag.activate_pdp = false;
                sharedTimeout_count++;

                if (sharedTimeout_count >= 3) {
                    decision_flag.error = true;
                    sharedTimeout_count = 0;
                }

                gsm_state = GSM_STATE_DECISION;
                break;
            }

            case EVT_ERR:
            {
                sharedStep = 0;
                DEBUG_PRINT("CGACT ERR\r\n");

                decision_flag.activate_pdp = false;
                decision_flag.error = true;
                sharedTimeout_count = 0;

                gsm_state = GSM_STATE_DECISION;
                break;
            }
        }
        break;
    }

    default:
        break;
    }
}