#include "SHT4x_Test.h"

#if 0

static sht4x_handle_t gs_handle;        /* sht4x handle */

/**
 * @brief     read test
 * @param[in] addr is the iic device address
 * @param[in] times is test times
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t sht4x_read_test(sht4x_address_t addr, uint32_t times)
{
    uint8_t res;
    uint8_t num[4];
    uint32_t i;
    sht4x_info_t info;
    
    /* link functions */
    DRIVER_SHT4X_LINK_INIT(&gs_handle, sht4x_handle_t);
    DRIVER_SHT4X_LINK_IIC_INIT(&gs_handle, sht4x_interface_iic_init);
    DRIVER_SHT4X_LINK_IIC_DEINIT(&gs_handle, sht4x_interface_iic_deinit);
    DRIVER_SHT4X_LINK_IIC_READ_COMMAND(&gs_handle, sht4x_interface_iic_read_cmd);
    DRIVER_SHT4X_LINK_IIC_WRITE_COMMAND(&gs_handle, sht4x_interface_iic_write_cmd);
    DRIVER_SHT4X_LINK_DELAY_MS(&gs_handle, sht4x_interface_delay_ms);
    DRIVER_SHT4X_LINK_DEBUG_PRINT(&gs_handle, sht4x_interface_debug_print);   
    /* sht4x info */
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
    /* start read test */
    NRF_LOG_INFO("sht4x: start read test.");
    /* set address */
    res = sht4x_set_addr(&gs_handle, addr);
    if (res != 0){
        NRF_LOG_INFO("sht4x: set addr failed.\n"); 
        return 1;
    }
    /* sht4x init */
    res = sht4x_init(&gs_handle);
    if (res != 0){
        NRF_LOG_INFO("sht4x: init failed.\n");
        return 1;
    }
    /* get serial number */
    res = sht4x_get_serial_number(&gs_handle, num);
    if (res != 0){
        NRF_LOG_INFO("sht4x: get serial number failed.\n");
        (void)sht4x_deinit(&gs_handle);
        return 1;
    }
    /* output */
    NRF_LOG_INFO("sht4x: serial number is 0x%02X 0x%02X 0x%02X 0x%02X.\n", num[0], num[1], num[2], num[3]);
    /* wait 1000 ms */
    sht4x_interface_delay_ms(1000);
    /* output */
    NRF_LOG_INFO("sht4x: high precision without heater.\n");
    /* loop */
    for (i = 0; i < times; i++)
    {
        uint16_t temperature_raw;
        float temperature_s;
        uint16_t humidity_raw;
        float humidity_s;
        
        /* read data */
        res = sht4x_read(&gs_handle, SHT4X_MODE_HIGH_PRECISION_WITH_NO_HEATER,
                        (uint16_t *)&temperature_raw, (float *)&temperature_s,
                        (uint16_t *)&humidity_raw, (float *)&humidity_s);
        if (res != 0)
        {
            NRF_LOG_INFO("sht4x: read failed.\n");
            (void)sht4x_deinit(&gs_handle);
            
            return 1;
        }
				NRF_LOG_INFO("sht4x: temperature is " NRF_LOG_FLOAT_MARKER " C", NRF_LOG_FLOAT(temperature_s));
        NRF_LOG_INFO("sht4x: humidity is " NRF_LOG_FLOAT_MARKER " %%", NRF_LOG_FLOAT(humidity_s));
        /* wait 1000 ms */
        sht4x_interface_delay_ms(1000);
    }
    /* wait 1000 ms */
    sht4x_interface_delay_ms(1000);
    /* output */
    NRF_LOG_INFO("sht4x: medium precision without heater.\n");
    /* loop */
    for (i = 0; i < times; i++)
    {
        uint16_t temperature_raw;
        float temperature_s;
        uint16_t humidity_raw;
        float humidity_s;
        
        /* read data */
        res = sht4x_read(&gs_handle, SHT4X_MODE_MEDIUM_PRECISION_WITH_NO_HEATER,
                        (uint16_t *)&temperature_raw, (float *)&temperature_s,
                        (uint16_t *)&humidity_raw, (float *)&humidity_s);
        if (res != 0)
        {
            NRF_LOG_INFO("sht4x: read failed.\n");
            (void)sht4x_deinit(&gs_handle);
            
            return 1;
        }
				NRF_LOG_INFO("sht4x: temperature is " NRF_LOG_FLOAT_MARKER " C", NRF_LOG_FLOAT(temperature_s));
        NRF_LOG_INFO("sht4x: humidity is " NRF_LOG_FLOAT_MARKER " %%", NRF_LOG_FLOAT(humidity_s));
        /* wait 1000 ms */
        sht4x_interface_delay_ms(1000);
    } 
    /* wait 1000 ms */
    sht4x_interface_delay_ms(1000); 
    /* output */
    NRF_LOG_INFO("sht4x: lowest precision without heater.\n");   
    /* loop */
    for (i = 0; i < times; i++)
    {
        uint16_t temperature_raw;
        float temperature_s;
        uint16_t humidity_raw;
        float humidity_s;
        
        /* read data */
        res = sht4x_read(&gs_handle, SHT4X_MODE_LOWEST_PRECISION_WITH_NO_HEATER,
                        (uint16_t *)&temperature_raw, (float *)&temperature_s,
                        (uint16_t *)&humidity_raw, (float *)&humidity_s);
        if (res != 0)
        {
            NRF_LOG_INFO("sht4x: read failed.\n");
            (void)sht4x_deinit(&gs_handle);
            
            return 1;
        }
				NRF_LOG_INFO("sht4x: temperature is " NRF_LOG_FLOAT_MARKER " C", NRF_LOG_FLOAT(temperature_s));
        NRF_LOG_INFO("sht4x: humidity is " NRF_LOG_FLOAT_MARKER " %%", NRF_LOG_FLOAT(humidity_s));
        /* wait 1000 ms */
        sht4x_interface_delay_ms(1000);
    }  
    /* wait 1000 ms */
    sht4x_interface_delay_ms(1000);    
    /* output */
    NRF_LOG_INFO("sht4x: activate heater with 200mW for 1s, high precision.\n"); 
    /* loop */
    for (i = 0; i < times; i++)
    {
        uint16_t temperature_raw;
        float temperature_s;
        uint16_t humidity_raw;
        float humidity_s;
        
        /* read data */
        res = sht4x_read(&gs_handle, SHT4X_MODE_HIGH_PRECISION_WITH_HEATER_200MW_1S,
                        (uint16_t *)&temperature_raw, (float *)&temperature_s,
                        (uint16_t *)&humidity_raw, (float *)&humidity_s);
        if (res != 0)
        {
            NRF_LOG_INFO("sht4x: read failed.\n");
            (void)sht4x_deinit(&gs_handle);
            
            return 1;
        }
				NRF_LOG_INFO("sht4x: temperature is " NRF_LOG_FLOAT_MARKER " C", NRF_LOG_FLOAT(temperature_s));
        NRF_LOG_INFO("sht4x: humidity is " NRF_LOG_FLOAT_MARKER " %%", NRF_LOG_FLOAT(humidity_s));
        /* wait 1000 ms */
        sht4x_interface_delay_ms(1000);
    }
    /* wait 1000 ms */
    sht4x_interface_delay_ms(1000);  
    /* output */
    NRF_LOG_INFO("sht4x: activate heater with 200mW for 0.1s, high precision.\n");   
    /* loop */
    for (i = 0; i < times; i++)
    {
        uint16_t temperature_raw;
        float temperature_s;
        uint16_t humidity_raw;
        float humidity_s;
        
        /* read data */
        res = sht4x_read(&gs_handle, SHT4X_MODE_HIGH_PRECISION_WITH_HEATER_200MW_0P1S,
                        (uint16_t *)&temperature_raw, (float *)&temperature_s,
                        (uint16_t *)&humidity_raw, (float *)&humidity_s);
        if (res != 0)
        {
            NRF_LOG_INFO("sht4x: read failed.\n");
            (void)sht4x_deinit(&gs_handle);
            
            return 1;
        }
				NRF_LOG_INFO("sht4x: temperature is " NRF_LOG_FLOAT_MARKER " C", NRF_LOG_FLOAT(temperature_s));
        NRF_LOG_INFO("sht4x: humidity is " NRF_LOG_FLOAT_MARKER " %%", NRF_LOG_FLOAT(humidity_s));
        /* wait 1000 ms */
        sht4x_interface_delay_ms(1000);
    }
    /* wait 1000 ms */
    sht4x_interface_delay_ms(1000);
    /* output */
    NRF_LOG_INFO("sht4x: activate heater with 110mW for 1s, high precision.\n");
    /* loop */
    for (i = 0; i < times; i++)
    {
        uint16_t temperature_raw;
        float temperature_s;
        uint16_t humidity_raw;
        float humidity_s;
        
        /* read data */
        res = sht4x_read(&gs_handle, SHT4X_MODE_HIGH_PRECISION_WITH_HEATER_110MW_1S,
                        (uint16_t *)&temperature_raw, (float *)&temperature_s,
                        (uint16_t *)&humidity_raw, (float *)&humidity_s);
        if (res != 0)
        {
            NRF_LOG_INFO("sht4x: read failed.\n");
            (void)sht4x_deinit(&gs_handle);
            
            return 1;
        }
				NRF_LOG_INFO("sht4x: temperature is " NRF_LOG_FLOAT_MARKER " C", NRF_LOG_FLOAT(temperature_s));
        NRF_LOG_INFO("sht4x: humidity is " NRF_LOG_FLOAT_MARKER " %%", NRF_LOG_FLOAT(humidity_s));    
        /* wait 1000 ms */
        sht4x_interface_delay_ms(1000);
    }
    /* wait 1000 ms */
    sht4x_interface_delay_ms(1000);
    /* output */
    NRF_LOG_INFO("sht4x: activate heater with 110mW for 0.1s, high precision.\n");
    /* loop */
    for (i = 0; i < times; i++)
    {
        uint16_t temperature_raw;
        float temperature_s;
        uint16_t humidity_raw;
        float humidity_s;
        
        /* read data */
        res = sht4x_read(&gs_handle, SHT4X_MODE_HIGH_PRECISION_WITH_HEATER_110MW_0P1S,
                        (uint16_t *)&temperature_raw, (float *)&temperature_s,
                        (uint16_t *)&humidity_raw, (float *)&humidity_s);
        if (res != 0)
        {
            NRF_LOG_INFO("sht4x: read failed.\n");
            (void)sht4x_deinit(&gs_handle);
            
            return 1;
        }
				NRF_LOG_INFO("sht4x: temperature is " NRF_LOG_FLOAT_MARKER " C", NRF_LOG_FLOAT(temperature_s));
        NRF_LOG_INFO("sht4x: humidity is " NRF_LOG_FLOAT_MARKER " %%", NRF_LOG_FLOAT(humidity_s));  
        /* wait 1000 ms */
        sht4x_interface_delay_ms(1000);
    }  
    /* wait 1000 ms */
    sht4x_interface_delay_ms(1000);
    /* output */
    NRF_LOG_INFO("sht4x: activate heater with 20mW for 1s, high precision.\n");  
    /* loop */
    for (i = 0; i < times; i++)
    {
        uint16_t temperature_raw;
        float temperature_s;
        uint16_t humidity_raw;
        float humidity_s;
        
        /* read data */
        res = sht4x_read(&gs_handle, SHT4X_MODE_HIGH_PRECISION_WITH_HEATER_20MW_1S,
                        (uint16_t *)&temperature_raw, (float *)&temperature_s,
                        (uint16_t *)&humidity_raw, (float *)&humidity_s);
        if (res != 0)
        {
            NRF_LOG_INFO("sht4x: read failed.\n");
            (void)sht4x_deinit(&gs_handle);
            
            return 1;
        }
				NRF_LOG_INFO("sht4x: temperature is " NRF_LOG_FLOAT_MARKER " C", NRF_LOG_FLOAT(temperature_s));
        NRF_LOG_INFO("sht4x: humidity is " NRF_LOG_FLOAT_MARKER " %%", NRF_LOG_FLOAT(humidity_s));
        /* wait 1000 ms */
        sht4x_interface_delay_ms(1000);
    }
    /* wait 1000 ms */
    sht4x_interface_delay_ms(1000);
    /* output */
    NRF_LOG_INFO("sht4x: activate heater with 20mW for 0.1s, high precision.\n");
    /* loop */
    for (i = 0; i < times; i++)
    {
        uint16_t temperature_raw;
        float temperature_s;
        uint16_t humidity_raw;
        float humidity_s;
        
        /* read data */
        res = sht4x_read(&gs_handle, SHT4X_MODE_HIGH_PRECISION_WITH_HEATER_20MW_0P1S,
                        (uint16_t *)&temperature_raw, (float *)&temperature_s,
                        (uint16_t *)&humidity_raw, (float *)&humidity_s);
        if (res != 0)
        {
            NRF_LOG_INFO("sht4x: read failed.\n");
            (void)sht4x_deinit(&gs_handle);
            
            return 1;
        }
				NRF_LOG_INFO("sht4x: temperature is " NRF_LOG_FLOAT_MARKER " C", NRF_LOG_FLOAT(temperature_s));
        NRF_LOG_INFO("sht4x: humidity is " NRF_LOG_FLOAT_MARKER " %%", NRF_LOG_FLOAT(humidity_s));    
        /* wait 1000 ms */
        sht4x_interface_delay_ms(1000);
    }  
    /* output */
    NRF_LOG_INFO("sht4x: soft reset.\n");
    /* soft reset */
    res = sht4x_soft_reset(&gs_handle);
    if (res != 0)
    {
        NRF_LOG_INFO("sht4x: soft reset failed.\n");
        (void)sht4x_deinit(&gs_handle);
        
        return 1;
    }
    /* finish read test */
    NRF_LOG_INFO("sht4x: finish read test.\n");
    (void)sht4x_deinit(&gs_handle);

    return 0;
}

void sht4x_test(void) /* sht40¹¦ÄÜ²âÊÔ */
{
		sht4x_read_test(SHT4X_ADDRESS_0, 1);
}

#endif
