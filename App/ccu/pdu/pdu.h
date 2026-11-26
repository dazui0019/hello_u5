#ifndef _PDU_H
#define _PDU_H

#include <stdbool.h>
#include <stdint.h>

enum device_pdu_voltage_status
{
    DEVICE_PDU_VOLTAGE_WAITING = 0,
    DEVICE_PDU_VOLTAGE_READY,
    DEVICE_PDU_VOLTAGE_FAULT,
};

/* 查询 PDU 是否可参与功率分配，与上电和电压到位状态分开。 */
bool device_pdu_is_available(uint8_t pdu_idx);

/* 请求 PDU 上电或断电，返回 true 表示请求已受理。 */
bool device_pdu_set_power_state(uint8_t pdu_idx, bool power_on);

/* 读取 PDU 的实际电源状态，true 为已上电，false 为已断电。 */
bool device_pdu_get_power_state(uint8_t pdu_idx);

/* 设置整组的目标电压，单位 0.1 V。 */
bool device_pdu_set_output_vol(uint8_t pdu_idx,
                               uint16_t target_voltage_0d1v);

/* 读取整组输出母线的有效实测电压，单位 0.1 V。失败时返回 false，输出参数保持原值。 */
bool device_pdu_get_output_vol(uint8_t pdu_idx, uint16_t *voltage_0d1v);

/* 设置整组的电流预算，单位 0.1 A，由 PDU 分配给组内模块。 */
bool device_pdu_set_output_cur(uint8_t pdu_idx, uint32_t total_current_0d1a);

/* 控制充电电流输出的使能，不切断模块供电。 */
bool device_pdu_set_current_enable(uint8_t pdu_idx, bool enable);

/* 按允许压差检查本次参与的模块是否电压到位，不阻塞等待。 */
enum device_pdu_voltage_status device_pdu_get_voltage_status(uint8_t pdu_idx,
                                                             uint16_t tolerance_0d1v);

#endif /* _PDU_H */
