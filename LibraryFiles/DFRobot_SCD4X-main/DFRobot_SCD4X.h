/*!
 * @file  DFRobot_SCD4X.h
 * @brief  Define infrastructure of DFRobot_SCD4X class
 * @details  Use I2C to configure the sensor parameters like single-measurement mode, period measurement mode and compensation and calibration.
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license  The MIT License (MIT)
 * @author  [qsjhyy](yihuan.huang@dfrobot.com)
 * @version  V1.0
 * @date  2022-05-09
 * @url  https://github.com/DFRobot/DFRobot_SCD4X
 */
#ifndef __DFRobot_SCD4X_H__
#define __DFRobot_SCD4X_H__

#include <Arduino.h>
#include <Wire.h>


// #define ENABLE_DBG   //!< Open this macro and you can see the details of the program
#ifdef ENABLE_DBG
  #define DBG(...) {Serial.print("[");Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
#else
  #define DBG(...)
#endif


#define SCD4X_I2C_ADDR   uint8_t(0x62)   ///< SCD4X I2C address

#define SCD4X_SERIAL_NUMBER_WORD0   uint16_t(0xbe02)   ///< SCD4X serial number 0
#define SCD4X_SERIAL_NUMBER_WORD1   uint16_t(0x7f07)   ///< SCD4X serial number 1
#define SCD4X_SERIAL_NUMBER_WORD2   uint16_t(0x3bfb)   ///< SCD4X serial number 2

#define SCD4X_CRC8_INIT             uint8_t(0xFF)
#define SCD4X_CRC8_POLYNOMIAL       uint8_t(0x31)

/* SCD4X Basic Commands */
#define SCD4X_START_PERIODIC_MEASURE    uint16_t(0x21b1)   ///< start periodic measurement, signal update interval is 5 seconds.
#define SCD4X_READ_MEASUREMENT          uint16_t(0xec05)   ///< read measurement
#define SCD4X_STOP_PERIODIC_MEASURE     uint16_t(0x3f86)   ///< stop periodic measurement command

/* SCD4X On-chip output signal compensation */
#define SCD4X_SET_TEMPERATURE_OFFSET    uint16_t(0x241d)   ///< set temperature offset
#define SCD4X_GET_TEMPERATURE_OFFSET    uint16_t(0x2318)   ///< get temperature offset
#define SCD4X_SET_SENSOR_ALTITUDE       uint16_t(0x2427)   ///< set sensor altitude
#define SCD4X_GET_SENSOR_ALTITUDE       uint16_t(0x2322)   ///< get sensor altitude
#define SCD4X_SET_AMBIENT_PRESSURE      uint16_t(0xe000)   ///< set ambient pressure

/* SCD4X Field calibration */
#define SCD4X_PERFORM_FORCED_RECALIB    uint16_t(0x362f)   ///< perform forced recalibration
#define SCD4X_SET_AUTOMATIC_CALIB       uint16_t(0x2416)   ///< set automatic self calibration enabled
#define SCD4X_GET_AUTOMATIC_CALIB       uint16_t(0x2313)   ///< get automatic self calibration enabled

/* SCD4X Low power */
#define SCD4X_START_LOW_POWER_MEASURE   uint16_t(0x21ac)   ///< start low power periodic measurement, signal update interval is approximately 30 seconds.
#define SCD4X_GET_DATA_READY_STATUS     uint16_t(0xe4b8)   ///< get data ready status

/* SCD4X Advanced features */
#define SCD4X_PERSIST_SETTINGS          uint16_t(0x3615)   ///< persist settings
#define SCD4X_GET_SERIAL_NUMBER         uint16_t(0x3682)   ///< get serial number
#define SCD4X_PERFORM_SELF_TEST         uint16_t(0x3639)   ///< perform self test
#define SCD4X_PERFORM_FACTORY_RESET     uint16_t(0x3632)   ///< perform factory reset
#define SCD4X_REINIT                    uint16_t(0x3646)   ///< reinit

/* SCD4X Low power single shot */
#define SCD4X_MEASURE_SINGLE_SHOT            uint16_t(0x219d)   ///< measure single shot
#define SCD4X_MEASURE_SINGLE_SHOT_RHT_ONLY   uint16_t(0x2196)   ///< measure single shot rht only
#define SCD4X_POWER_DOWN                     uint16_t(0x36e0)   ///< Put the sensor from idle to sleep to reduce current consumption.
#define SCD4X_WAKE_UP                        uint16_t(0x36f6)   ///< Wake up the sensor from sleep mode into idle mode.

/* Convenience Macro */
#define SCD4X_CONCAT_BYTES(msb, lsb)   (((uint16_t)msb << 8) | (uint16_t)lsb)   ///< Macro combines two 8-bit data into one 16-bit data


class DFRobot_SCD4X
{
public:

  /**
   * @struct sSensorMeasurement_t
   * @brief Sensor readings, including CO2 concentration, temperature and humidity
   */
  typedef struct
  {
    uint16_t   CO2ppm;
    float   temp;
    float   humidity;
  } sSensorMeasurement_t;

public:

/**************************** Init and reset ********************************/

  /**
   * @fn DFRobot_SCD4X
   * @brief Constructor
   * @param pWire - Wire object is defined in Wire.h, so just use &Wire and the methods in Wire can be pointed to and used
   * @param i2cAddr - SCD4X I2C address.
   * @return None
   */
  DFRobot_SCD4X(TwoWire *pWire=&Wire, uint8_t i2cAddr=SCD4X_I2C_ADDR);

  /**
   * @fn begin
   * @brief Init function
   * @return bool type, true if successful, false if error
   */
  bool begin(void);

  /**
   * @fn setSleepMode
   * @brief Set the sensor as sleep or wake-up mode (SCD41 only)
   * @param mode - sleep and wake-up mode:
   * @n       SCD4X_POWER_DOWN : Put the sensor from idle to sleep to reduce current consumption.
   * @n       SCD4X_WAKE_UP : Wake up the sensor from sleep mode into idle mode.
   * @return None
   * @note Note that the SCD4x does not acknowledge the wake_up command. Command execution time : 20 ms
   * @n When executing the command, the sensor can't be in period measurement mode
   */
  void setSleepMode(uint16_t mode);

  /**
   * @fn performSelfTest
   * @brief perform self test
   * @details The perform_self_test feature can be used as an end-of-line test to check sensor 
   * @n  functionality and the customer power supply to the sensor.
   * @return module status:
   * @n        0 : no malfunction detected
   * @n        other : malfunction detected
   * @note Command execution time : 10000 ms
   * @n When executing the command, the sensor can't be in period measurement mode
   */
  uint16_t performSelfTest(void);

  /**
   * @fn moduleReinit
   * @brief module reinit
   * @details  The reinit command reinitializes the sensor by reloading user settings from EEPROM.
   * @return None
   * @note Before sending the reinit command, the stop measurement command must be issued.
   * @n  If the reinit command does not trigger the desired re-initialization, 
   * @n  a power-cycle should be applied to the SCD4x.
   * @n  Command execution time : 20 ms
   * @n When executing the command, the sensor can't be in period measurement mode
   */
  void moduleReinit(void);

  /**
   * @fn performFactoryReset
   * @brief perform factory reset
   * @details The perform_factory_reset command resets all configuration settings stored 
   * @n  in the EEPROM and erases the FRC and ASC algorithm history.
   * @return None
   * @note Command execution time : 1200 ms
   * @n When executing the command, the sensor can't be in period measurement mode
   */
  void performFactoryReset(void);

/********************************* Measurement Function *************************************/

  /**
   * @fn measureSingleShot
   * @brief measure single shot（SCD41 only）
   * @details  On-demand measurement of CO2 concentration, relative humidity and temperature.
   * @n  Get the measured data through readMeasurement(sSensorMeasurement_t data) interface
   * @param mode - Single-measurement mode:
   * @n       SCD4X_MEASURE_SINGLE_SHOT : On-demand measurement of CO2 concentration, relative humidity and temperature. 
   * @n                                   Max command duration 5000 [ms].
   * @n       SCD4X_MEASURE_SINGLE_SHOT_RHT_ONLY : On-demand measurement of relative humidity and temperature only.
   * @n                                            Max command duration 50 [ms].
   * @return None
   * @note In SCD4X_MEASURE_SINGLE_SHOT_RHT_ONLY mode, CO2 output is returned as 0 ppm.
   * @n When executing the command, the sensor can't be in period measurement mode
   */
  void measureSingleShot(uint16_t mode);

  /**
   * @fn enablePeriodMeasure
   * @brief set periodic measurement mode
   * @param mode - periodic measurement mode:
   * @n       SCD4X_START_PERIODIC_MEASURE : start periodic measurement, signal update interval is 5 seconds.
   * @n       SCD4X_STOP_PERIODIC_MEASURE : stop periodic measurement command
   * @n       SCD4X_START_LOW_POWER_MEASURE :  start low power periodic measurement, signal update interval is approximately 30 seconds.
   * @return None
   * @note The measurement mode must be disabled when configuring the sensor; after giving the stop_periodic_measurement command, the sensor needs to wait 500ms before responding to other commands.
   */
  void enablePeriodMeasure(uint16_t mode);

  /**
   * @fn readMeasurement
   * @brief Read the measured data
   * @param data - sSensorMeasurement_t, sensor readings, including CO2 concentration (ppm), temperature (℃) and humidity (RH)
   * @return None
   * @note CO2 measurement range: 0~40000 ppm; temperature measurement range: -10~60 ℃; humidity measurement range: 0~100 %RH.
   */
  void readMeasurement(sSensorMeasurement_t * data);

  /**
   * @fn getDataReadyStatus
   * @brief get data ready status
   * @return data ready status:
   * @n        true : data ready
   * @n        false : data not ready
   */
  bool getDataReadyStatus(void);

/*************************** compensation and calibration ********************************/

  /**
   * @fn setTempComp
   * @brief set temperature offset
   * @details T(offset_actual) = T(SCD4X) - T(reference) + T(offset_previous)
   * @n T(offset_actual): the calculated actual temperature offset that is required
   * @n T(SCD4X): the temperature measured by the sensor (wait for a period of time to get steady readings)
   * @n T(reference): the standard reference value of the current ambient temperature
   * @n T(offset_previous): the previously set temperature offset
   * @n For example: 32(T(SCD4X)) - 30(T(reference)) + 2(T(offset_previous)) = 4(T(offset_actual))
   * @param tempComp - temperature offset value, unit ℃
   * @return None
   * @note When executing the command, the sensor can't be in period measurement mode
   */
  void setTempComp(float tempComp);

  /**
   * @fn getTempComp
   * @brief get temperature offset
   * @return The current temp compensated value, unit ℃
   * @note When executing the command, the sensor can't be in period measurement mode
   */
  float getTempComp(void);

  /**
   * @fn setSensorAltitude
   * @brief set sensor altitude
   * @param altitude - the current ambient altitude, unit m
   * @return None
   * @note When executing the command, the sensor can't be in period measurement mode
   */
  void setSensorAltitude(uint16_t altitude);

  /**
   * @fn getSensorAltitude
   * @brief get sensor altitude
   * @return the current ambient altitude, unit m
   * @note When executing the command, the sensor can't be in period measurement mode
   */
  uint16_t getSensorAltitude(void);

  /**
   * @fn setAmbientPressure
   * @brief set ambient pressure
   * @param ambientPressure - the current ambient pressure, unit Pa
   * @return None
   */
  void setAmbientPressure(uint32_t ambientPressure);

  /**
   * @fn performForcedRecalibration
   * @brief perform forced recalibration
   * @param CO2ppm - Target CO2 concentration, unit ppm
   * @return calibration amplitude, return (int16_t)0x7fff if the calibration failed
   * @note Command execution time : 400 ms
   * @n When executing the command, the sensor can't be in period measurement mode
   */
  int16_t performForcedRecalibration(uint16_t CO2ppm);

  /**
   * @fn setAutoCalibMode
   * @brief set automatic self calibration enabled
   * @param mode - automatic self-calibration mode:
   * @n       true : enable automatic self-calibration
   * @n       false : disable automatic self-calibration
   * @return None
   * @note When executing the command, the sensor can't be in period measurement mode
   */
  void setAutoCalibMode(bool mode);

  /**
   * @fn getAutoCalibMode
   * @brief get automatic self calibration enabled
   * @return automatic self-calibration mode:
   * @n        true : enable automatic self-calibration
   * @n        false : disable automatic self-calibration
   * @note When executing the command, the sensor can't be in period measurement mode
   */
  bool getAutoCalibMode(void);

  /**
   * @fn persistSettings
   * @brief Save the settings into EEPROM, default to be in RAM
   * @details Configuration settings such as the temperature offset, sensor altitude and the ASC enabled/disabled 
   * @n  parameter are by default stored in the volatile memory (RAM) only and will be lost after a power-cycle.
   * @return None
   * @note To avoid unnecessary wear of the EEPROM, the persist_settings command should only be sent 
   * @n  when persistence is required and if actual changes to the configuration have been made. 
   * @n  The EEPROM is guaranteed to endure at least 2000 write cycles before failure.
   * @note Command execution time : 800 ms
   * @n When executing the command, the sensor can't be in period measurement mode
   */
  void persistSettings(void);

protected:

/*************************** get serial number *****************************/

  /**
   * @fn getSerialNumber
   * @brief Reading out the serial number can be used to identify the chip and to verify the presence of the sensor.
   * @n  Together, the 3 words constitute a unique serial number with a length of 48 bits (big endian format).
   * @param wordBuf - for buffering the obtained chip serial number
   * @return bool type, true if successful, false if error
   * @note When executing the command, the sensor can't be in period measurement mode
   */
  bool getSerialNumber(uint16_t * wordBuf);

/*********************** CRC Check & Sending Data Pack *************************/

  /**
   * @fn calcCRC
   * @brief Calculate the current crc check code to contrast it with the MISR read from the sensor
   * @param data - The measured data just obtained from the sensor
   * @return The current calculated crc check code
   */
  uint8_t calcCRC(uint16_t data);

  /**
   * @fn pack
   * @brief Pack the data to be sent
   * @param data - The data to be sent
   * @return The packed data to be sent
   */
  uint8_t * pack(uint16_t data);

/****************************** Read/Write Command Function ********************************/

  /**
   * @fn writeData
   * @brief Write register value through I2C bus
   * @param cmd - module commands 16bits
   * @param pBuf - Storage and buffer for data to be written
   * @param size - Length of data to be written
   * @return None
   */
  void writeData(uint16_t cmd, const void* pBuf, size_t size);

  /**
   * @fn readData
   * @brief Read register value through I2C bus
   * @param cmd - module commands 16bits
   * @param pBuf - Storage and buffer for data to be read
   * @param size - Length of data to be read
   * @return Return the read length, returning 0 means reading failed
   */
  size_t readData(uint16_t cmd, void* pBuf, size_t size);

private:
  // Private variables
  TwoWire *_pWire;   // Pointer to I2C communication method
  uint8_t _deviceAddr;   // Address of the device for I2C communication
};

#endif
