/**
 * @file DataStructure.h
 * @author Vu Thanh Trung
 * @brief 
 * @version 0.1
 * @date 2024-01-17
 * 
 * 
*/

#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

#include <Arduino.h>
#include <string>
#include "esp_log.h"
#include "configs.h"
#include <ArduinoJson.h>
#include <NTPClient.h>

#define ERROR_DATA_CREATE_STRINGDATA_FAILED 0x91




struct calibData {
    uint32_t temperature_calibInt_u32;
    uint32_t depth_calibInt_u32;
    uint32_t DO_value_calibInt_u32;


    calibData() {     // initialation function for struct (C++ style)
        this->temperature_calibInt_u32      = 0;
        this->depth_calibInt_u32              = 0;
        this->DO_value_calibInt_u32             = 0;
    }
};



/**
 * @brief Create a string from sensor data for storing in the SD card
 * 
 * @param[out] _calibDataString: pointer char*, use to store string
 * @param[in]  _calibData: struct calibdata
 * @return ERROR_CODE 
 */
ERROR_CODE creatCalibDataString(char *_calibDataString, struct calibData _calibData);

struct sensorData {
    float temperature; 
    float pressCurrent;
    uint32_t pressVoltage;
    float depth; // Get depth through pressure sensor
    uint32_t DO_voltage;
    uint16_t DO_value;
    double       lon_f;
    double       lat_f;

    sensorData() 
    {
        this->temperature    = 0;
        this->pressCurrent   = 0;
        this->pressVoltage   = 0;
        this->depth		     = 0;
        this->DO_voltage     = 0;
        this->DO_value       = 0;
        this->lon_f          = 0;
        this->lat_f          = 0;
        
    };

    sensorData operator=(const sensorData _sensorData_temp)
    {
        this->temperature	 = _sensorData_temp.temperature;
        this->pressCurrent   = _sensorData_temp.pressCurrent;
        this->pressVoltage   = _sensorData_temp.pressVoltage;
        this->depth		     = _sensorData_temp.depth;
        this->DO_voltage     = _sensorData_temp.DO_voltage;
        this->DO_value	     = _sensorData_temp.DO_value;
        this->lon_f          = _sensorData_temp.lon_f;
        this->lat_f          = _sensorData_temp.lat_f;
        
        return _sensorData_temp;
    }
};


/**
 * @brief Create a string
 * 
 * @param[out] _senorDataString: Pointer char*, use to store DataString
 * @param[in]  _nameDevice: Device's name
 * @param[in] _dateTimeString: Pointer char*, use to store DateTime
 * @param[in] _sensorData_st: struct sensor data
 * @return ERROR_CODE 
 */
ERROR_CODE createSensorDataString(char *_sensorDataString,
                                  const char *_nameDevice,
                                  const char *_dateTimeString,
                                  struct sensorData _sensorData_st);



#endif