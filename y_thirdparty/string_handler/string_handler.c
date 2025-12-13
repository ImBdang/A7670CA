#include "string_handler.h"

/**
 * @brief   Convert char to uint_t
 * 
 * @param   s: String that you need to convert to uint32_t
 * 
 * @retval  Uint32_t after converted
 */
uint32_t fast_atoi(const char *s) {
    uint32_t v = 0;
    while (*s >= '0' && *s <= '9') {
        v = v * 10 + (*s - '0');
        s++;
    }
    return v;
}

/**
 * @brief   Convert uint32_t to char[11]
 * 
 * @param   value: Number that you want to convert
 * @param   buf:   Pointer to char array hold the converted number in string
 */
void fast_itoa(uint32_t value, char *buf) 
{
    char temp[11]; 
    int i = 0;
    if (value == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }
    while (value > 0) {
        temp[i++] = (value % 10) + '0';
        value /= 10;
    }
    int j = 0;
    while (i > 0) {
        buf[j++] = temp[--i];
    }
    buf[j] = '\0';
}


/**
 * @brief   Parse format OTA_SIZE:<SIZE>
 */
uint32_t parse_ota_size(const char *line) {
    const char *p = strchr(line, ':');
    if (!p) return 0;   
    p++;
    return (uint32_t)fast_atoi(p);
}


void gsm_parse_cpin(const char *line, char *status)
{
    const char *p = strstr(line, "+CPIN:");
    if (!p) return;              

    p += strlen("+CPIN:");  
    while (*p == ' ' || *p == '\t') p++;
    int i = 0;
    while (*p && *p != '\r' && *p != '\n' && i < 11) {
        status[i++] = *p++;
    }
    status[i] = '\0';
}


void gsm_parse_creg(const char* line, char* status)
    {
        const char *p = strstr(line, "+CREG:");
        if (!p) {
            status[0] = '\0';
            return;
        }
        p += 6; 

        while (*p == ' ' || *p == '\t')
            p++;

        while (*p && *p != ',')
            p++;

        if (*p == ',')
            p++; 
        int i = 0;
        while (*p && *p != '\r' && *p != '\n' && *p != ',' && i < 7) {
            status[i++] = *p++;
        }
        status[i] = '\0';
    }

void gsm_parse_httpget(const char *str, uint8_t *method, uint16_t *status, uint32_t *length)
{
    while (*str && *str != ':') str++;
    if (*str == ':') str++;

    while (*str == ' ') str++;

    *method = fast_atoi(str);

    while (*str && *str != ',') str++;
    if (*str == ',') str++;

    *status = fast_atoi(str);

    while (*str && *str != ',') str++;
    if (*str == ',') str++;

    *length = fast_atoi(str);
}

uint32_t gsm_parse_httpread_len(const char *line)
{
    if (strncmp(line, "+HTTPREAD:", 10) != 0)
        return 0;  
    
    line += 10;    

    while (*line == ' ')
        line++;
    return fast_atoi(line);
}

uint8_t new_sms_dispatch(const char *s)
{
    const char *p = strrchr(s, ',');
    if (!p) return 0;
    return (uint8_t)fast_atoi(p + 1);
}


//+CMQTTRXTOPIC: <client>,<len>
uint8_t mqtt_parse_rxtopic_len(const char *line)
{
    const char *p = line;

    const char *prefix = "+CMQTTRXTOPIC:";
    uint8_t prefix_len = strlen(prefix);

    if (strncmp(p, prefix, prefix_len) != 0)
        return 0;

    p += prefix_len;

    while (*p == ' ') p++;
    while (*p >= '0' && *p <= '9')
        p++;

    if (*p != ',')
        return 0;

    p++; 

    uint16_t len = 0;
    while (*p >= '0' && *p <= '9') {
        len = len * 10 + (*p - '0');
        p++;
    }

    return (uint8_t)len;
}


    // format: +CMQTTRXPAYLOAD: <client>,<len>
uint8_t mqtt_parse_rxpayload_len(const char *line)
{
    const char *p = line;

    const char *prefix = "+CMQTTRXPAYLOAD:";
    uint8_t prefix_len = strlen(prefix);

    if (strncmp(p, prefix, prefix_len) != 0)
        return 0;

    p += prefix_len;

    while (*p == ' ') p++;

    while (*p >= '0' && *p <= '9')
        p++;

    if (*p != ',')
        return 0;

    p++;

    uint16_t len = 0;
    while (*p >= '0' && *p <= '9') {
        len = len * 10 + (*p - '0');
        p++;
    }

    return (uint8_t)len;
}



