# DFRobot_SCD4X
* [中文版](./README_CN.md)

The SCD4x(SCD40/SCD41) is Sensirion’s next generation miniature CO2
sensor. This sensor builds on the photoacoustic sensing 
principle and Sensirion’s patented PAsens® and 
CMOSens® technology to offer high accuracy at an 
unmatched price and smallest form factor. SMD assembly 
allows cost- and space-effective integration of the sensor 
combined with maximal freedom of design. On-chip signal 
compensation is realized with the build-in SHT4x humidity 
and temperature sensor.
CO2 is a key indicator for indoor air quality as high levels 
compromise humans’ cognitive low power
and well-being. The SCD4x enables smart ventilation
systems to regulate ventilation in the most energy-efficient 
and human-friendly way. Moreover, indoor air quality 
monitors and other connected devices based on the 
SCD4x can help maintaining low CO2 concentration for a 
healthy, productive environment.

![Product Photo Show](./resources/images/SCD4X.png)


## Product Link (https://www.dfrobot.com/search-SCD4X.html)
    SKU: SEN0536


## Table of Contents

* [Summary](#summary)
* [Installation](#installation)
* [Methods](#methods)
* [Compatibility](#compatibility)
* [History](#history)
* [Credits](#credits)


## Summary

* Photoacoustic sensor technology PASens®
* Large output range: 0 ppm – 40’000 ppm
* Large supply voltage range: 2.4 – 5.5 V
* High accuracy: ±(40 ppm + 5 %)
* Digital I2C interface
* Integrated temperature and humidity sensor
* Low power operation down to < 0.4 mA avg. @ 5 V, 1 meas. / 5 minutes


## Installation

There two methods:

1. To use this library, first download the library file, paste it into the \Arduino\libraries directory, then open the examples folder and run the demo in the folder.
2. Search the DFRobot_SCD4X library from the Arduino Software Library Manager and download it.


## Methods

```C++

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
   * @brief set the sensor as sleep or wake-up mode (SCD41 only)
   * @param mode - wake-up mode:
   * @n       SCD4X_POWER_DOWN : Put the sensor from idle to sleep to reduce current consumption.
   * @n       SCD4X_WAKE_UP : Wake up the sensor from sleep mode into idle mode.
   * @return None
   * @note Note that the SCD4x does not acknowledge the wake_up command. Command execution time : 20 ms
   * @n When executing this function, the sensor can't be in period measurement mode.
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
   * @n When executing the command, the sensor can't be in period measurement mode.
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
   * @n When executing the command, the sensor can't be in period measurement mode.
   */
  void moduleReinit(void);

  /**
   * @fn performFactoryReset
   * @brief perform factory reset
   * @details The perform_factory_reset command resets all configuration settings stored 
   * @n  in the EEPROM and erases the FRC and ASC algorithm history.
   * @return None
   * @note Command execution time : 1200 ms
   * @n When executing the command, the sensor can't be in period measurement mode.
   */
  void performFactoryReset(void);

/********************************* Measurement Function *************************************/

  /**
   * @fn measureSingleShot
   * @brief measure single shot（SCD41 only）
   * @details  On-demand measurement of CO2 concentration, relative humidity and temperature.
   * @n  Get the measured data through readMeasurement(sSensorMeasurement_t data) interface
   * @param mode -    * @n  Single-measurement mode:
   * @n       SCD4X_MEASURE_SINGLE_SHOT : On-demand measurement of CO2 concentration, relative humidity and temperature. 
   * @n                                   Max command duration 5000 [ms].
   * @n       SCD4X_MEASURE_SINGLE_SHOT_RHT_ONLY : On-demand measurement of relative humidity and temperature only.
   * @n                                            Max command duration 50 [ms].
   * @return None
   * @note In SCD4X_MEASURE_SINGLE_SHOT_RHT_ONLY mode, CO2 output is returned as 0 ppm.
   * @n When executing the command, the sensor can't be in period measurement mode.
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
   * @param data - sSensorMeasurement_t, the values measured by the sensor, including CO2 concentration (ppm), temperature (℃), humidity (RH)
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
   * @n T(reference): the temperature standard reference value of the current ambient 
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
   * @return The current temp compensation value, unit ℃
   * @note When executing the command, the sensor can't be in period measurement mode
   */
  float getTempComp(void);

  /**
   * @fn setSensorAltitude
   * @brief set sensor altitude
   * @param altitude -  the current ambient altitude, unit m
   * @return None
   * @note When executing the command, the sensor can't be in period measurement mode
   */
  void setSensorAltitude(uint16_t altitude);

  /**
   * @fn getSensorAltitude
   * @brief get sensor altitude
   * @return  The current ambient altitude, unit m
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
   * @n Command execution time : 800 ms
   * @n When executing the command, the sensor can't be in period measurement mode
   */
  void persistSettings(void);

```


## Compatibility

MCU                | Work Well    | Work Wrong   | Untested    | Remarks
------------------ | :----------: | :----------: | :---------: | :----:
Arduino Uno        |      √       |              |             |
Arduino MEGA2560   |      √       |              |             |
Arduino Leonardo   |      √       |              |             |
FireBeetle-ESP8266 |      √       |              |             |
FireBeetle-ESP32   |      √       |              |             |
FireBeetle-M0      |      √       |              |             |
Micro:bit          |      √       |              |             |


## History

- 2022/05/11 - Version 1.0.0 released.
- 2022/12/13 - Version 1.0.1 released.


## Credits

Written by qsjhyy(yihuan.huang@dfrobot.com), 2022. (Welcome to our [website](https://www.dfrobot.com/))

