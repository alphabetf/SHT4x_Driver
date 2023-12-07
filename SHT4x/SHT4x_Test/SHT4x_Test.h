#ifndef __SHT4X_TEST__
#define __SHT4X_TEST__

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "SHT4x_Driver_Layer.h"
#include "SHT4x_Interface_Layer.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief     read test
 * @param[in] addr is the iic device address
 * @param[in] times is test times
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t sht4x_read_test(sht4x_address_t addr, uint32_t times);
void sht4x_test(void);

#ifdef __cplusplus
}
#endif

#endif /* __SHT4X_TEST__ */
