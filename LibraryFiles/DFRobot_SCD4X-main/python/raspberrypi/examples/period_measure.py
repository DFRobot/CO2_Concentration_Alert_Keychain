# -*- coding: utf-8 -*
'''!
  @file  period_measure.py
  @brief  This sample shows how to configure period measurement mode, compensation and calibration.
  @note  The actual compensation and calibration parameter should be changed according to the operating environment of the sensor.
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
    @brief set temperature offset
    @details T(offset_actual) = T(SCD4X) - T(reference) + T(offset_previous)
    @n T(offset_actual): the calculated actual temperature offset that is required
    @n T(SCD4X): the temperature measured by the sensor (the steady reading after a period of time)
    @n T(reference): the standard reference value of the current ambient temperature
    @n T(offset_previous): the previously set temperature offset
    @n For example: 32(T(SCD4X)) - 30(T(reference)) + 2(T(offset_previous)) = 4(T(offset_actual))
    @param temp_comp - temperature offset value, unit ℃
    @return None
    @note When executing the command, the sensor can't be in period measurement mode
  '''
  sensor.set_temp_comp(4.0)

  '''!
    @brief get temperature offset
    @return The current set temp compensation value, unit ℃
    @note When executing the command, the sensor can't be in period measurement mode
  '''
  print("The current temperature compensation value : %0.2f C" %(sensor.get_temp_comp))

  '''!
    @brief set sensor altitude
    @param altitude - the current ambient altitude, unit m
    @note When executing the command, the sensor can't be in period measurement mode
  '''
  sensor.set_sensor_altitude(540)

  '''!
    @brief get sensor altitude
    @return The current set ambient altitude, unit m
    @note When executing the command, the sensor can't be in period measurement mode
  '''
  print("Set the current environment altitude : %u m" %(sensor.get_sensor_altitude))

  '''!
    @brief set automatic self calibration enabled
    @param mode - automatic self-calibration mode:
    @n       True : enable automatic self-calibration
    @n       False : disable automatic self-calibration
    @return None
    @note When executing the command, the sensor can't be in period measurement mode
  '''
  # sensor.set_auto_calib_mode(True)

  '''!
    @brief get automatic self calibration enabled
    @return Automatic self-calibration mode:
    @n        True : enable automatic self-calibration
    @n        False : disable automatic self-calibration
    @note When executing the command, the sensor can't be in period measurement mode
  '''
  # if(sensor.get_auto_calib_mode):
  #   print("Automatic calibration on!")
  # else:
  #   print("Automatic calibration off!")

  '''!
    @brief persist settings
    @details Configuration settings such as the temperature offset, sensor altitude and the ASC enabled/disabled 
    @n  parameter are by default stored in the volatile memory (RAM) only and will be lost after a power-cycle.
    @return None
    @note To avoid unnecessary wear of the EEPROM, the persist_settings command should only be sent 
    @n  when persistence is required and if actual changes to the configuration have been made. 
    @n  The EEPROM is guaranteed to endure at least 2000 write cycles before failure.
    @note Command execution time : 800 ms
    @n When executing the command, the sensor can't be in period measurement mode
  '''
  # sensor.persist_settings

  '''!
    @brief reinit reinit
    @details  The reinit command reinitializes the sensor by reloading user settings from EEPROM.
    @return None
    @note Before sending the reinit command, the stop measurement command must be issued.
    @n  If the reinit command does not trigger the desired re-initialization, 
    @n  a power-cycle should be applied to the SCD4x.
    @n  Command execution time : 20 ms
    @n When executing the command, the sensor can't be in period measurement mode
  '''
  # sensor.module_reinit

  '''!
    @brief set periodic measurement mode
    @param mode - periodic measurement mode:
    @n       SCD4X_START_PERIODIC_MEASURE : start periodic measurement, signal update interval is 5 seconds.
    @n       SCD4X_STOP_PERIODIC_MEASURE : stop periodic measurement command
    @n       SCD4X_START_LOW_POWER_MEASURE :  start low power periodic measurement, signal update interval is approximately 30 seconds.
    @return None
    @note The measurement mode must be disabled when changing the sensor settings; after giving the stop_periodic_measurement command, the sensor needs to wait 500ms before responding to other commands.
  '''
  sensor.enable_period_measure(SCD4X_START_PERIODIC_MEASURE)

  print()

def loop():
  '''!
    @brief get data ready status
    @return data ready status:
    @n        True : data ready
    @n        False : data not ready
  '''
  if(sensor.get_data_ready_status):
    '''!
      @brief set ambient pressure
      @param ambient_pressure - the current ambient pressure, unit Pa
      @return None
    '''
    # sensor.set_ambient_pressure(96000)

    '''!
      @brief Read the measured data
      @return The measured values from the sensor
      @retval CO2ppm - CO2 concentration (ppm)
      @retval temp - temperature (C)
      @retval humidity - humidity (RH)
      @note CO2 measurement range: 0~40000 ppm; temperature measurement range: -10~60 ℃; humidity measurement range: 0~100 %RH.
    '''
    CO2ppm, temp, humidity= sensor.read_measurement
    print("Carbon dioxide concentration : %u ppm" %CO2ppm)
    print("Environment temperature : %0.2f C" %temp)
    print("Relative humidity : %0.2f RH\n" %humidity)

  time.sleep(1)


if __name__ == "__main__":
  setup()
  while True:
    loop()
