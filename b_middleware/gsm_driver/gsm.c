#include "gsm.h"

gsm_state_t gsm_state = GSM_STATE_DECISION;

lwrb_t      at_rb;
uint8_t     at_rb_buff[AT_LWRB_SIZE];

/* ====================================== STATIC DECLARATIONS ======================================= */
static uint32_t sharedStep = 0;
gsm_decision_flag_t decision_flag;

static void clear_decision_flags(void);
static void decision_state(void);
/* ================================================================================================== */


static void clear_decision_flags(void)
{
    decision_flag.power_on      = false;
    decision_flag.sync_at       = false;
    decision_flag.config        = false;
    decision_flag.check_sim     = false;
    decision_flag.reg_network   = false;
    decision_flag.attach_gprs   = false;
    decision_flag.activate_pdp  = false;
    decision_flag.error         = false;
    decision_flag.ready         = false;
}

/**
 * @brief   Check if GSM is ready
 */
bool gsm_is_ready(void){
    if (gsm_state == GSM_STATE_READY)
        return true;
    return false;
}

/**
 * @brief   Power on modem
 */
bool gsm_power_on(void)
{
    if (SIM_STATUS_READ())
    {   
        decision_flag.power_on = true;
        gsm_state = GSM_STATE_DECISION;
        DEBUG_PRINT("STATUS is HIGH, MODEM is now ONLINE\r\n");
        return true;
    }
    static uint32_t t0 = 0;
    switch (sharedStep)
    {
        case 0:
            SIM_PWKEY_LOW();
            t0 = get_systick();
            sharedStep = 1;
            DEBUG_PRINT("POWERING ON, PWKEY LOW 50ms\r\n");
            return false;

        case 1:
            if (get_systick() - t0 < 50)
                return false;
            SIM_PWKEY_HIGH();
            t0 = get_systick();
            sharedStep = 2;
            DEBUG_PRINT("POWERING ON, PWKEY HIGH about 12s\r\n");
            return false;

        case 2:
            if (SIM_STATUS_READ())
            {
                DEBUG_PRINT("STATUS is HIGH, MODEM is now ONLINE\r\n");
                sharedStep = 0;
                decision_flag.power_on = true;
                gsm_state = GSM_STATE_DECISION;
                return true;
            }
            if (get_systick() - t0 > 12000) 
            {
                sharedStep = 0;                   
                DEBUG_PRINT("TIMEOUT, retry\r\n");
                return false;
            }
            DEBUG_PRINT("Waiting STATUS ON\r\n");
            return false;
        
        default:
            break;
    }
    return false;
}

/**
 * @brief   Power off modem
 */
bool gsm_power_off(void){
    if (!SIM_STATUS_READ())
    {
        DEBUG_PRINT("STATUS is LOW, MODEM is now OFFLINE\r\n");
        return true;
    }
    static uint32_t t0 = 0;
    switch (sharedStep)
    {
        case 0:
            SIM_PWKEY_LOW();
            t0 = get_systick();
            sharedStep = 1;
            DEBUG_PRINT("POWERING OFF, PWKEY LOW 2.5s\r\n");
            return false;

        case 1:
            if (get_systick() - t0 < 2500)
                return false;
            SIM_PWKEY_HIGH();
            t0 = get_systick();
            sharedStep = 2;
            DEBUG_PRINT("POWERING OFF, PWKEY HIGH about 4s\r\n");
            return false;

        case 2:
            if (!SIM_STATUS_READ())
            {
                DEBUG_PRINT("STATUS is LOW, MODEM is now OFFLINE\r\n");
                sharedStep = 0;
                return true;
            }
            if (get_systick() - t0 > 2000) 
            {
                sharedStep = 0;                     
                DEBUG_PRINT("TIMEOUT, retry\r\n");
                return false;
            }
            DEBUG_PRINT("Waiting STATUS OFF\r\n");
            return false;
    }
    return false;
}


/**
 * @brief   Change state by this decision func
 * 
 * @note    If that state flag is false that mean its not done yet so switch to it
 */
static void decision_state(void)
{
    /*<! STATE: ERROR */
    if (decision_flag.error) {
        gsm_state = GSM_STATE_ERROR;
        return;
    }

    // if (!decision_flag.power_on){
    //     gsm_state = GSM_STATE_POWER_ON;
    //     return;
    // }

    /*<! STATE: SYNC_AT */
    if (!decision_flag.sync_at) {
        gsm_state = GSM_STATE_AT_SYNC;
        return;
    }

    /*<! STATE: CONFIG */
    if (!decision_flag.config) {
        gsm_state = GSM_STATE_CONFIG;
        return;
    }

    /*<! STATE: CHECK_SIM */
    if (!decision_flag.check_sim) {
        gsm_state = GSM_STATE_CHECK_SIM;
        return;
    }

    /*<! STATE: REG_NETWORK */
    if (!decision_flag.reg_network) {
        gsm_state = GSM_STATE_CHECK_NETWORK;
        return;
    }

    /*<! STATE: ATTACH_PSD */
    if (!decision_flag.attach_gprs) {
        gsm_state = GSM_STATE_CHECK_GPRS;
        return;
    }

    /*<! STATE: ACTIVATE_PDP */
    if (!decision_flag.activate_pdp) {
        gsm_state = GSM_STATE_INIT_PDP;
        return;
    }

    /*<! STATE: READY */
    if (!decision_flag.ready) {
        gsm_state = GSM_STATE_READY;
        return;
    }
}

void GSM_Manager(void)
{
    gsm_at_process();
    switch (gsm_state)
    {
        case GSM_STATE_DECISION: decision_state(); break;

        case GSM_STATE_AT_SYNC: gsm_common_sync_at(); break;

        case GSM_STATE_CONFIG:gsm_common_config(); break;

        case GSM_STATE_CHECK_SIM: gsm_common_check_sim(); break;
    
        case GSM_STATE_CHECK_NETWORK: gsm_common_check_network(); break;

        case GSM_STATE_CHECK_GPRS: gsm_common_check_gprs();  break;

        case GSM_STATE_INIT_PDP: gsm_common_init_pdp(); break;

        case GSM_STATE_READY: decision_state(); break;
        case GSM_STATE_ERROR:
        {
            DEBUG_PRINT("TOO MUCH TIMEOUT, ERROR STATE TRY TO CLEAR FLAGS AND RESTART\r\n");
            clear_decision_flags();
            gsm_state = GSM_STATE_DECISION;
            break;
        }
    }
}

void gsm_init(void)
{
    lwrb_init(&at_rb, at_rb_buff, sizeof(at_rb_buff));
}

