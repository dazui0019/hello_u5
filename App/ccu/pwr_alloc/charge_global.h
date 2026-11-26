/**
 * @file    charge_global.h
 * @brief   工程全局公共宏
 * @note    只放与具体模块无关的全局定义（如枪头数量）；
 *          各配置类型的 EEPROM 存储块布局与启动加载入口见
 *          app/storage/config/charge_config_eeprom.h。
 */

#ifndef CHARGE_GLOBAL_H
#define CHARGE_GLOBAL_H

/* 枪头数量，索引 0 代表 1 枪，1 代表 2 枪 */
#ifndef CHARGE_CONNECTOR_NUM
#define CHARGE_CONNECTOR_NUM 2U
#endif

#endif /* CHARGE_GLOBAL_H */
