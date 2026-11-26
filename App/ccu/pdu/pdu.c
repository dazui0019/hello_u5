#include "pdu.h"
#include <stdint.h>
#include <stdbool.h>

bool device_pdu_is_available(uint8_t pdu_idx)
{
    (void)pdu_idx;
    return false;
}

bool device_pdu_set_power_state(uint8_t pdu_idx, bool power_on)
{
    (void)pdu_idx;
    (void)power_on;
    return false;
}

bool device_pdu_get_power_state(uint8_t pdu_idx)
{
    (void)pdu_idx;
    return false;
}

bool device_pdu_set_output_vol(uint8_t pdu_idx,
                               uint16_t target_voltage_0d1v)
{
    (void)pdu_idx;
    (void)target_voltage_0d1v;
    return false;
}

bool device_pdu_get_output_vol(uint8_t pdu_idx, uint16_t *voltage_0d1v)
{
    (void)pdu_idx;
    (void)voltage_0d1v;
    return false;
}

bool device_pdu_set_output_cur(uint8_t pdu_idx, uint32_t total_current_0d1a)
{
    (void)pdu_idx;
    (void)total_current_0d1a;
    return false;
}

bool device_pdu_set_current_enable(uint8_t pdu_idx, bool enable)
{
    (void)pdu_idx;
    (void)enable;
    return false;
}

enum device_pdu_voltage_status device_pdu_get_voltage_status(uint8_t pdu_idx,
                                                             uint16_t tolerance_0d1v)
{
    (void)pdu_idx;
    (void)tolerance_0d1v;
    return DEVICE_PDU_VOLTAGE_FAULT;
}
