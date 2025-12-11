#include "gsm_entity.h"

sim_entity_t sim_entity = {
    .state = SIM_UNKNOW,
    .creg = CREG_UNKNOWN
};

http_action_entity_t http_action_entity = {
    .ready = 0,
    .status_code = 0,
    .data_length = 0,
    .method = 0,

    .read_done = 0,
    .post_done = 0
};

gsm_sim_state_t get_sim_state(void)
{
    return sim_entity.state;
}

gsm_sim_creg_state_t get_sim_creg_state(void)
{
    return sim_entity.creg;
}

void set_sim_state(gsm_sim_state_t state)
{
    sim_entity.state = state; 
}

void set_sim_creg_state(gsm_sim_creg_state_t creg)
{
    sim_entity.creg = creg;
}

void set_http_action(uint8_t ready, uint8_t method, uint16_t status, uint32_t data_length)
{
    http_action_entity.ready = ready;
    http_action_entity.method = method;
    http_action_entity.status_code = status;
    http_action_entity.data_length = data_length;
}

