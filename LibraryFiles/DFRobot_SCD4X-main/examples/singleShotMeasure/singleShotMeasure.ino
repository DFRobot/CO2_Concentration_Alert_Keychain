/*!
 * @file  singleShotMeasure.ino
 * @brief  This sample shows how to set single measurement mode, perform reset operation, and set sleep and wake-up mode.
 * @details Get 6 data from single measurement, take the average value, print it, enter sleep mode, wake up the sensor after 5 minutes, and repeat the above measurement process
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
   * @brief perform self test
   * @details The perform_self_test feature can be used as an end-of-line test to check sensor 
   * @n  functionality and the customer power supply to the sensor.
   * @return module status:
   * @n        0 : no malfunction detected
   * @n        other : malfunction detected
   * @note Command execution time : 10000 ms
   * @n When executing the command, the sensor can't be in period measurement mode.
   */
  if(0 != SCD4X.performSelfTest()) {
    Serial.println("Malfunction detected!");
  }

  Serial.println();
}

void loop()
{
  /**
   * @brief Set the sensor as sleep or wake-up mode (SCD41 only)
   * @param mode - Sleep and wake-up mode:
   * @n       SCD4X_POWER_DOWN : Put the sensor from idle to sleep to reduce current consumption.
   * @n       SCD4X_WAKE_UP : Wake up the sensor from sleep mode into idle mode.
   * @return None
   * @note Note that the SCD4x does not acknowledge the wake_up command. Command execution time : 20 ms
   * @n When executing the command, the sensor can't be in period measurement mode.
   */
  Serial.print("Waking sensor...");
  SCD4X.setSleepMode(SCD4X_WAKE_UP);

  DFRobot_SCD4X::sSensorMeasurement_t data[6];
  memset(data, 0, sizeof(data));
  uint32_t averageCO2ppm=0;
  float averageTemp=0.0, averageHumidity=0.0;
  Serial.print("Measuring...");
  for(uint8_t i=0; i<6; i++) {
    /**
     * @brief measure single shot（SCD41 only）
     * @details  On-demand measurement of CO2 concentration, relative humidity and temperature.
     * @n  Get the measured data through readMeasurement(sSensorMeasurement_t data) interface
     * @param mode - Single-measurement mode:
     * @n       SCD4X_MEASURE_SINGLE_SHOT : On-demand measurement of CO2 concentration, relative humidity and temperature. 
     * @n                                   Max command duration 5000 [ms].
     * @n       SCD4X_MEASURE_SINGLE_SHOT_RHT_ONLY : On-demand measurement of relative humidity and temperature only.
     * @n                                            Max command duration 50 [ms].
     * @note In SCD4X_MEASURE_SINGLE_SHOT_RHT_ONLY mode, CO2 output is returned as 0 ppm.
     * @return None
     * @note When executing the command, the sensor can't be in period measurement mode.
     */
    SCD4X.measureSingleShot(SCD4X_MEASURE_SINGLE_SHOT);

    /**
     * @brief get data ready status
     * @return data ready status:
     * @n        true : data ready
     * @n        false : data not ready
     */
    while(!SCD4X.getDataReadyStatus()) {
      delay(100);
    }

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
    SCD4X.readMeasurement(&data[i]);
    if(0 != i) {   // Discard the first set of data, because the chip datasheet indicates the first reading obtained after waking up is invalid
      averageCO2ppm += data[i].CO2ppm;
      averageTemp += data[i].temp;
      averageHumidity += data[i].humidity;
    }
    Serial.print(i);
  }
  Serial.print("\nCarbon dioxide concentration : ");
  Serial.print(averageCO2ppm / 5);
  Serial.println(" ppm");

  Serial.print("Environment temperature : ");
  Serial.print(averageTemp / 5);
  Serial.println(" C");

  Serial.print("Relative humidity : ");
  Serial.print(averageHumidity / 5);
  Serial.println(" RH\n");

  // Put the sensor from idle to sleep to reduce current consumption.
  Serial.print("Sleeping sensor...");
  SCD4X.setSleepMode(SCD4X_POWER_DOWN);
  delay(300000);   // Wake up the sensor after 5 minutes, and repeat the above measurement process
}
