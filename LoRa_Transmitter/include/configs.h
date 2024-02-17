/**
 * @file config.h
 * @author Vu Thanh Trung
 * @brief 
 * @version 0.1
 * @date 2024-01-17
 * 
 * 
*/

#ifndef CONFIGS_H
#define CONFIGS_H

#include <Arduino.h>

#define NAME_DEVICE "SPARC-1"

#define SERIAL_DEBUG_BAUDRATE   (9600)
#define DEVICE_DATA_SAVE_INTERVAL 5000
#define VREF 3300
#define ADC_RES 4096 // ADC Resolution
#define ERROR_CODE int
#define ERROR_NONE (0x00)
#define MAC_ADDRESS "D4:8A:FC:CE:EB:90"
enum status_et
{
    DISCONNECTED,
    CONNECTED,
    CONNECTION_LOST,
    SCANNING,
    SENDING_DATA,
    READING_DATA,
    WRITING_DATA
};

struct connectionStatus
{
    status_et sdCardStatus;
    status_et ds3231Status;
    status_et rfStatus;
    status_et gpsStatus;
};

extern struct connectionStatus connectionStatus_st;
//========================== DEFINE FOR LOGGING ========================

#define USING_SDCARD
#define USING_RTC

//========================== DEFINE FOR POWER CONTROL ========================

#define PIN_NUM_12V_CTRL   (25)
#define PIN_NUM_5V_CTRL   (26)

//========================== DEFINE FOR SPI  ========================

#define PIN_NUM_MISO    (19)
#define PIN_NUM_MOSI    (23)
#define PIN_NUM_CLK     (18)
#define PIN_CS_SD_CARD  (5)

#define PIN_CS_RF_MODULE   (4)

//========================== DEFINE FOR I2C ========================

#define PIN_SDA_GPIO    21
#define PIN_SCL_GPIO    22
#define I2C_CLOCK_SPEED 100000U
#define RTC_I2C_ADDRESS (0x68)

//========================== DEFINE FOR UART ========================

#define PIN_NUM_GPS_RX   (17)  //It is TX2 on ESP32
#define PIN_NUM_GPS_TX   (16)  //It is RX2 on ESP32

//========================== DEFINE FOR ONE WIRE ========================

#define PIN_NUM_ONE_WIRE_BUS (15)

//========================== DEFINE FOR ANALOG ========================

#define PIN_NUM_PRESSURE_SENSOR  (32)
#define PIN_NUM_DO_SENSOR   (33)

#endif // CONFIGS_H