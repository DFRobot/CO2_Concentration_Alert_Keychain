# -*- coding: utf-8 -*
'''!
  @file  single_shot_measure.py
  @brief  This sample shows how to set single measurement mode, perform reset operation, and set sleep and wake-up mode.
  @details Get 6 data from single measurement, take the average value, print it, enter sleep mode, wake up the sensor after 5 minutes, and repeat the above measurement process
  @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  @license  The MIT License (MIT)
  @author  [qsjhyy](yihuan.huang@dfrobot.com)
  @version  V1.0
  @date  2022-05-13
  @url  https://github.com/DFRobot/DFRobot_SCD4X
'''
from __future__ import print_function
import sys
import os
sys.path.append(os.path.dirname(os.path.dirname(os.path.realpath(__file__))))

from DFRobot_SCD4X import *


'''!
  @brief Module I2C communication init
  @param i2c_addr I2C communication address
  @param bus I2C bus
'''
sensor = DFRobot_SCD4X(i2c_addr = SCD4X_I2C_ADDR, bus = 1)


def setup():
  while (not sensor.begin):
    print ('Please check that the device is properly connected')
    time.sleep(3)
  print("sensor begin successfully!!!")

  '''!
    @brief set periodic measurement mode
    @param mode - periodic measurement mode:
    @n       SCD4X_START_PERIODIC_MEASURE : start periodic measurement, signal update interval is 5 seconds.
    @n       SCD4X_STOP_PERIODIC_MEASURE : stop periodic measurement command
    @n       SCD4X_START_LOW_POWER_MEASURE :  start low power periodic measurement, signal update interval is approximately 30 seconds.
    @return None
    @note The measurement mode must be disabled when changing the sensor settings; after giving the stop_periodic_measurement command, the sensor needs to wait 500ms before responding to other commands.
  '''
  sensor.enable_period_measure(SCD4X_STOP_PERIODIC_MEASURE)

  '''!
    @brief perform self test
    @details The perform_self_test feature can be used as an end-of-line test to check sensor 
    @n  functionality and the customer power supply to the sensor.
    @return module status:
    @n        0 : no malfunction detected
    @n        other : malfunction detected
    @note Command execution time : 10000 ms
    @n When executing the command, the sensor can't be in period measurement mode
  '''
  if(0 != sensor.perform_self_test):
    print("Malfunction detected!")


def loop():
  '''!
    @brief Set the sensor as sleep or wake-up mode (SCD41 only)
    @param mode - sleep and wake-up mode:
    @n       SCD4X_POWER_DOWN : Put the sensor from idle to sleep to reduce current consumption.
    @n       SCD4X_WAKE_UP : Wake up the sensor from sleep mode into idle mode.
    @note Note that the SCD4x does not acknowledge the wake_up command. Command execution time : 20 ms
    @n When executing the command, the sensor can't be in period measurement mode
  '''
  print("Waking sensor...")
  sensor.set_sleep_mode(SCD4X_WAKE_UP)

  average_CO2ppm = 0
  average_temp = 0
  average_humidity = 0
  print("Measuring...", end="")
  for i in range(0, 6):
    '''!
      @brief measure single shot(SCD41 only)
      @details  On-demand measurement of CO2 concentration, relative humidity and temperature.
      @n  Get the measured data through readMeasurement(sSensorMeasurement_t data) interface
      @param mode - Single-measurement mode:
      @n       SCD4X_MEASURE_SINGLE_SHOT : On-demand measurement of CO2 concentration, relative humidity and temperature. 
      @n                                   Max command duration 5000 [ms].
      @n       SCD4X_MEASURE_SINGLE_SHOT_RHT_ONLY : On-demand measurement of relative humidity and temperature only.
      @n                                            Max command duration 50 [ms].
      @note In SCD4X_MEASURE_SINGLE_SHOT_RHT_ONLY mode,  CO2 output is returned as 0 ppm.
      @return None
      @n When executing the command, the sensor can't be in period measurement mode
    '''
    sensor.measure_single_shot(SCD4X_MEASURE_SINGLE_SHOT)

    '''!
      @brief get data ready status
      @return data ready status:
      @n        True : data ready
      @n        False : data not ready
    '''
    while(not sensor.get_data_ready_status):
      time.sleep(0.1)

    '''!
      @brief Read the measured data
      @return The measured values from the sensor
      @retval CO2ppm - CO2 concentration (ppm)
      @retval temp - temperature (C)
      @retval humidity - humidity (RH)
      @note CO2 measurement range: 0~40000 ppm; temperature measurement range: -10~60 ℃; humidity measurement range: 0~100 %RH.
    '''
    CO2ppm, temp, humidity = sensor.read_measurement
    if 0 != i:   # Discard the first set of data, because the chip datasheet indicates they are invalid
      average_CO2ppm += CO2ppm
      average_temp += temp
      average_humidity += humidity
    print(i, end="")
  print("\nCarbon dioxide concentration : %u ppm" %(average_CO2ppm / 5))
  print("Environment temperature : %0.2f C" %(average_temp / 5))
  print("Relative humidity : %0.2f RH\n" %(average_humidity / 5))

  # Put the sensor from idle to sleep to reduce current consumption.
  print("Sleeping sensor...")
  sensor.set_sleep_mode(SCD4X_POWER_DOWN)
  time.sleep(300)   # Wake up the sensor after 5 minutes, and repeat the above measurement process


if __name__ == "__main__":
  setup()
  while True:
    loop()
