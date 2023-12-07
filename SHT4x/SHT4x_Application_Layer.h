#ifndef __SHT4X_APPLICATION_LAYER__
#define __SHT4X_APPLICATION_LAYER__

#include "SHT4x_Driver_Layer.h"
#include "SHT4x_Interface_Layer.h"

#ifdef __cplusplus
extern "C" {
#endif

void sht4x_get_current_relhumidity(uint8_t* relhumidity_integer, uint8_t* relhumidity_decimal);
void sht4x_get_current_reltemp(uint8_t* temp_integer, uint8_t* temp_decimal);
float sht4x_get_current_temperature(void);
float sht4x_get_current_humidity(void);
uint8_t sht4x_sensor_init(void);

#ifdef __cplusplus
}
#endif

#endif /* __SHT4X_APPLICATION_LAYER__ */
