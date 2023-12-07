#ifndef __SHT4X_INTERFACE_LAYER__
#define __SHT4X_INTERFACE_LAYER__

#include "HAL_I2C_Master.h"
#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief  interface iic bus init
 * @return status code
 *         - 0 success
 *         - 1 iic init failed
 * @note   none
 */
uint8_t sht4x_interface_iic_init(void);

/**
 * @brief  interface iic bus deinit
 * @return status code
 *         - 0 success
 *         - 1 iic deinit failed
 * @note   none
 */
uint8_t sht4x_interface_iic_deinit(void);

/**
 * @brief     interface iic bus write command
 * @param[in] addr is the iic device write address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the length of the data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t sht4x_interface_iic_write_cmd(uint8_t addr, uint8_t *buf, uint16_t len);

/**
 * @brief      interface iic bus read command
 * @param[in]  addr is the iic device write address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t sht4x_interface_iic_read_cmd(uint8_t addr, uint8_t *buf, uint16_t len);

/**
 * @brief     interface delay ms
 * @param[in] ms
 * @note      none
 */
void sht4x_interface_delay_ms(uint32_t ms);

/**
 * @brief     interface print format data
 * @param[in] fmt is the format data
 * @note      none
 */
void sht4x_interface_debug_print(const char *const fmt, ...);

#ifdef __cplusplus
}
#endif

#endif /* __SHT4X_INTERFACE_LAYER__ */
