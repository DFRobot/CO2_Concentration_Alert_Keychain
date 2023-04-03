/*!
 * @file  periodMeasure.ino
 * @brief  This sample shows how to configure period measurement mode, compensation and calibration.
 * @note  The actual compensation and calibration parameter should be changed according to the operating environment
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license  The MIT License (MIT)
 * @author  [qsjhyy](yihuan.huang@dfrobot.com)
 * @version  V1.0
 * @date  2022-05-11
 * @url  https://github.com/DFRobot/DFRobot_SCD4X
 */
#include <DFRobot_SCD4X.h>

/**
 * @brief Constructor
 * @param pWire - Wire object is defined in Wire.h, so just use &Wire and the methods in Wire can be pointed to and used
 * @param i2cAddr - SCD4X I2C address.
 */
DFRobot_SCD4X SCD4X(&Wire, /*i2cAddr = */SCD4X_I2C_ADDR);

void setup(void)
{
  Serial.begin(115200);

  // Init the sensor
  while( !SCD4X.begin() ){
    Serial.println("Communication with device failed, please check connection");
    delay(3000);
  }
  Serial.println("Begin ok!");

  /**
   * @brief set periodic measurement mode
   * @param mode - periodic measurement mode:
   * @n       SCD4X_START_PERIODIC_MEASURE : start periodic measurement, signal update interval is 5 seconds.
   * @n       SCD4X_STOP_PERIODIC_MEASURE : stop periodic measurement command
   * @n       SCD4X_START_LOW_POWER_MEASURE :  start low power periodic measurement, signal update interval is approximately 30 seconds.
   * @return None
   * @note The measurement mode must be disabled when configuring the sensor; after giving the stop_periodic_measurement command, the sensor needs to wait 500ms before responding to other commands.
   */
  SCD4X.enablePeriodMeasure(SCD4X_STOP_PERIODIC_MEASURE);

  /**
   * @brief set temperature offset
   * @details T(offset_actual) = T(SCD4X) - T(reference) + T(offset_previous)
   * @n T(offset_actual): the calculated actual temperature offset that is required
   * @n T(SCD4X): the temperature measured by the sensor (wait for a period of time to get steady readings)
   * @n T(reference): the standard reference value of the current ambient temperature
   * @n T(offset_previous): the previously set temperature offset
   * @n For example : 32(T(SCD4X)) - 30(T(reference)) + 2(T(offset_previous)) = 4(T(offset_actual))
   * @param tempComp - temperature offset value, unit ℃
   * @return None
   * @note When executing the command, the sensor can't be in period measurement mode
   */
  SCD4X.setTempComp(4.0);

  /**
   * @brief get temperature offset
   * @return The current set temp compensation value, unit ℃
   * @note When executing the command, the sensor can't be in period measurement mode
   */
  float temp = 0;
  temp = SCD4X.getTempComp();
  Serial.print("The current temperature compensation value : ");
  Serial.print(temp);
  Serial.println(" C");

  /**
   * @brief set sensor altitude
   * @param altitude - the current ambient altitude, unit m
   * @return None
   * @note When executing the command, the sensor can't be in period measurement mode
   */
  SCD4X.setSensorAltitude(540);

  /**
   * @brief get sensor altitude
   * @return The current set ambient altitude, unit m
   * @note When executing the command, the sensor can't be in period measurement mode
   */
  uint16_t altitude = 0;
  altitude = SCD4X.getSensorAltitude();
  Serial.print("Set the current environment altitude : ");
  Serial.print(altitude);
  Serial.println(" m");

  /**
   * @brief set automatic self calibration enabled
   * @param mode - automatic self-calibration mode:
   * @n       true : enable automatic self-calibration
   * @n       false : disable automatic self-calibration
   * @return None
   * @note When executing the command, the sensor can't be in period measurement mode
   */
  // SCD4X.setAutoCalibMode(true);

  /**
   * @brief get automatic self calibration enabled
   * @return Automatic self-calibration mode:
   * @n        true : enable automatic self-calibration
   * @n        false : disable automatic self-calibration
   * @note When executing the command, the sensor can't be in period measurement mode
   */
  // if(SCD4X.getAutoCalibMode()) {
  //   Serial.println("Automatic calibration on!");
  // } else {
  //   Serial.println("Automatic calibration off!");
  // }

  /**
   * @brief persist settings
   * @details Configuration settings such as the temperature offset, sensor altitude and the ASC enabled/disabled 
   * @n  parameter are by default stored in the volatile memory (RAM) only and will be lost after a power-cycle.
   * @return None
   * @note To avoid unnecessary wear of the EEPROM, the persist_settings command should only be sent 
   * @n  when persistence is required and if actual changes to the configuration have been made. 
   * @n  The EEPROM is guaranteed to endure at least 2000 write cycles before failure.
   * @note Command execution time : 800 ms
   * @n When executing the command, the sensor can't be in period measurement mode
   */
  // SCD4X.persistSettings();

  /**
   * @brief reinit reinit
   * @details  The reinit command reinitializes the sensor by reloading user settings from EEPROM.
   * @return None
   * @note Before sending the reinit command, the stop measurement command must be issued.
   * @n  If the reinit command does not trigger the desired re-initialization, 
   * @n  a power-cycle should be applied to the SCD4x.
   * @n  Command execution time : 20 ms
   * @n When executing the command, the sensor can't be in period measurement mode
   */
  //SCD4X.moduleReinit();

  /**
   * @brief set periodic measurement mode
   * @param mode - periodic measurement mode:
   * @n       SCD4X_START_PERIODIC_MEASURE : start periodic measurement, signal update interval is 5 seconds.
   * @n       SCD4X_STOP_PERIODIC_MEASURE : stop periodic measurement command
   * @n       SCD4X_START_LOW_POWER_MEASURE :  start low power periodic measurement, signal update interval is approximately 30 seconds.
   * @return None
   * @note The measurement mode must be disabled when changing the sensor settings; after giving the stop_periodic_measurement command, the sensor needs to wait 500ms before responding to other commands.
   */
  SCD4X.enablePeriodMeasure(SCD4X_START_PERIODIC_MEASURE);

  Serial.println();
}

void loop()
{
  /**
   * @brief get data ready status
   * @return data ready status:
   * @n        true : data ready
   * @n        false : data not ready
   */
  if(SCD4X.getDataReadyStatus()) {
    /**
     * @brief set ambient pressure
     * @param ambientPressure - the current ambient pressure, unit Pa
     * @return None
     */
    // SCD4X.setAmbientPressure(96000);

    /**
     * @brief Read the measured data
     * @param data - sSensorMeasurement_t, the values measured by the sensor, including CO2 concentration (ppm), temperature (℃) and humidity (RH)
     * @n  typedef struct {
     * @n    uint16_t   CO2ppm;
     * @n    float   temp;
     * @n    float   humidity;
     * @n  } sSensorMeasurement_t;
     * @return None
     * @note CO2 measurement range: 0~40000 ppm; temperature measurement range: -10~60 ℃; humidity measurement range: 0~100 %RH.
     */
    DFRobot_SCD4X::sSensorMeasurement_t data;
    SCD4X.readMeasurement(&data);

    Serial.print("Carbon dioxide concentration : ");
    Serial.print(data.CO2ppm);
    Serial.println(" ppm");

    Serial.print("Environment temperature : ");
    Serial.print(data.temp);
    Serial.println(" C");

    Serial.print("Relative humidity : ");
    Serial.print(data.humidity);
    Serial.println(" RH");

    Serial.println();
  }
  delay(1000);
}
