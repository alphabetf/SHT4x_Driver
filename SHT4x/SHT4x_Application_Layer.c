#include "SHT4x_Application_Layer.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "app_timer.h"

static sht4x_handle_t g_sht4x_handle;        /* sht4x handle */
APP_TIMER_DEF(sht4x_timer_id);							 /* sht4x定时器句柄 */	
static float temperature_s = 0.f;						 /* 当前的温度 */
static float humidity_s = 0.f;							 /* 当前湿度 */

/* 获取当前温度,小数形式 */
float sht4x_get_current_temperature(void)
{
		return temperature_s;
}

/* 获取当前湿度,小数形式 */
float sht4x_get_current_humidity(void)
{
		return humidity_s;
}

/* 这里是定时器到期时要执行的代码 */
static void sht4x_timer_handler(void * p_context) 
{
   uint16_t temperature_raw;
   uint16_t humidity_raw;
	 uint8_t res;
      
		/* read data */
		res = sht4x_read(&g_sht4x_handle, SHT4X_MODE_MEDIUM_PRECISION_WITH_NO_HEATER,
										(uint16_t *)&temperature_raw, (float *)&temperature_s,
										(uint16_t *)&humidity_raw, (float *)&humidity_s);
		if (res != 0){
				NRF_LOG_INFO("sht4x: read failed.\n");
				//(void)sht4x_deinit(&g_sht4x_handle);
		}
		NRF_LOG_INFO("sht4x: temperature is " NRF_LOG_FLOAT_MARKER " C", NRF_LOG_FLOAT(temperature_s));
		NRF_LOG_INFO("sht4x: humidity is " NRF_LOG_FLOAT_MARKER " %%", NRF_LOG_FLOAT(humidity_s));
}

/* 温湿度传感器初始化并每1s更新一次传感器数据 */
uint8_t sht4x_sensor_init(void)
{
		uint8_t res;
		uint8_t num[4];
		sht4x_info_t info;
	
		/* 注册sht4x驱动程序 */
	  DRIVER_SHT4X_LINK_INIT(&g_sht4x_handle, sht4x_handle_t);
    DRIVER_SHT4X_LINK_IIC_INIT(&g_sht4x_handle, sht4x_interface_iic_init);
    DRIVER_SHT4X_LINK_IIC_DEINIT(&g_sht4x_handle, sht4x_interface_iic_deinit);
    DRIVER_SHT4X_LINK_IIC_READ_COMMAND(&g_sht4x_handle, sht4x_interface_iic_read_cmd);
    DRIVER_SHT4X_LINK_IIC_WRITE_COMMAND(&g_sht4x_handle, sht4x_interface_iic_write_cmd);
    DRIVER_SHT4X_LINK_DELAY_MS(&g_sht4x_handle, sht4x_interface_delay_ms);
    DRIVER_SHT4X_LINK_DEBUG_PRINT(&g_sht4x_handle, sht4x_interface_debug_print); 
		/* 获取sht4x设备信息 */
	  res = sht4x_info(&info);
    if (res != 0){
        NRF_LOG_PUSH("sht4x: get info failed.");
        return 1;
    }else{
        /* print chip information */
        NRF_LOG_INFO("sht4x: chip is %s.", NRF_LOG_PUSH(info.chip_name));
        NRF_LOG_INFO("sht4x: manufacturer is %s.", NRF_LOG_PUSH(info.manufacturer_name));
        NRF_LOG_INFO("sht4x: interface is %s.", NRF_LOG_PUSH(info.interface));
        NRF_LOG_INFO("sht4x: driver version is %d.%d.", info.driver_version / 1000, (info.driver_version % 1000) / 100);
				NRF_LOG_INFO("sht4x: min supply voltage is " NRF_LOG_FLOAT_MARKER " v", NRF_LOG_FLOAT(info.supply_voltage_min_v));  
				NRF_LOG_INFO("sht4x: max supply voltage is " NRF_LOG_FLOAT_MARKER " v", NRF_LOG_FLOAT(info.supply_voltage_max_v)); 
				NRF_LOG_INFO("sht4x: max current is " NRF_LOG_FLOAT_MARKER " mA", NRF_LOG_FLOAT(info.max_current_ma)); 
				NRF_LOG_INFO("sht4x: max temperature is " NRF_LOG_FLOAT_MARKER " C", NRF_LOG_FLOAT(info.temperature_max)); 
				NRF_LOG_INFO("sht4x: min temperature is " NRF_LOG_FLOAT_MARKER " C", NRF_LOG_FLOAT(info.temperature_min)); 
    }
    /* 设置sht40的设备地址 */
    res = sht4x_set_addr(&g_sht4x_handle, SHT4X_ADDRESS_0);
    if (res != 0){
        NRF_LOG_INFO("sht4x: set addr failed.\n"); 
        return 1;
    }
    /* 初始化SHT40 */
    res = sht4x_init(&g_sht4x_handle);
    if (res != 0){
        NRF_LOG_INFO("sht4x: init failed.\n");
        return 1;
    }
    /* 获取sht40的序列号 */
    res = sht4x_get_serial_number(&g_sht4x_handle, num);
    if (res != 0){
        NRF_LOG_INFO("sht4x: get serial number failed.\n");
        (void)sht4x_deinit(&g_sht4x_handle);
        return 1;
    }
		/* output */
    NRF_LOG_INFO("sht4x: serial number is 0x%02X 0x%02X 0x%02X 0x%02X.\n", num[0], num[1], num[2], num[3]);
	
		app_timer_create(&sht4x_timer_id, APP_TIMER_MODE_REPEATED, sht4x_timer_handler);
		app_timer_start(sht4x_timer_id, APP_TIMER_TICKS(1000), NULL);
		NRF_LOG_INFO("sht4x init finsh"); 
		
		return 0;
}

/* 获取当前温度数据,并将其分离成整数和小数部分 */
void sht4x_get_current_reltemp(uint8_t* temp_integer, uint8_t* temp_decimal)
{
	//NRF_LOG_INFO("TemperatureCeil:" NRF_LOG_FLOAT_MARKER " C", NRF_LOG_FLOAT(temperature));
  *temp_integer = (uint8_t)(temperature_s);		/* 获取温度整数部分 */
	//NRF_LOG_INFO("temp_integer %d", *temp_integer);  
  *temp_decimal = (uint8_t)(((temperature_s) - (uint8_t)(temperature_s))*10); /* 湿度数据只保留一位小数 */
	//NRF_LOG_INFO("temp_decimal %d", *temp_decimal);  
}

/* 获取当前湿度数据,并将其分离成整数和小数部分 */
void sht4x_get_current_relhumidity(uint8_t* relhumidity_integer, uint8_t* relhumidity_decimal)
{
	//NRF_LOG_INFO("RelHumidity:" NRF_LOG_FLOAT_MARKER " %%", NRF_LOG_FLOAT(Humidity));
	*relhumidity_integer = (uint8_t)(humidity_s);		/* 获取温度整数部分 */
	//NRF_LOG_INFO("relhumidity_integer %d", *relhumidity_integer);  
  *relhumidity_decimal = (uint8_t)(((humidity_s) - (uint8_t)(humidity_s))*10); /* 湿度数据只保留一位小数 */
	//NRF_LOG_INFO("temp_decimal %d", *relhumidity_decimal);  
}
