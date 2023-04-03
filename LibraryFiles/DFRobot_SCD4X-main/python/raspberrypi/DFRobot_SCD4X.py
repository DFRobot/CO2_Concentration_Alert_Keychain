# -*- coding: utf-8 -*
'''!
  @file  DFRobot_SCD4X.py
  @brief  Define infrastructure of DFRobot_SCD4X class
  @details  Configure the sensor with single-measurement mode, period measurement mode and compensation and calibration using I2C communication.
  @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  @license  The MIT License (MIT)
  @author  [qsjhyy](yihuan.huang@dfrobot.com)
  @version  V1.0
  @date  2022-05-11
  @url  https://github.com/DFRobot/DFRobot_SCD4X
'''
import sys
import time

import smbus

import logging
from ctypes import *


logger = logging.getLogger()
# logger.setLevel(logging.INFO)   # Display all print information
logger.setLevel(logging.FATAL)   # If you don’t want to display too many prints, only print errors, please use this option
ph = logging.StreamHandler()
formatter = logging.Formatter("%(asctime)s - [%(filename)s %(funcName)s]:%(lineno)d - %(levelname)s: %(message)s")
ph.setFormatter(formatter) 
logger.addHandler(ph)


## SCD4X I2C address
SCD4X_I2C_ADDR            = 0x62

## SCD4X serial number 0
SCD4X_SERIAL_NUMBER_WORD0 = 0xbe02
## SCD4X serial number 1
SCD4X_SERIAL_NUMBER_WORD1 = 0x7f07
## SCD4X serial number 2
SCD4X_SERIAL_NUMBER_WORD2 = 0x3bfb

SCD4X_CRC8_INIT           = 0xFF
SCD4X_CRC8_POLYNOMIAL     = 0x31

''' SCD4X Basic Commands '''
## start periodic measurement, signal update interval is 5 seconds.
SCD4X_START_PERIODIC_MEASURE = 0x21b1
## read measurement
SCD4X_READ_MEASUREMENT       = 0xec05
## stop periodic measurement command
SCD4X_STOP_PERIODIC_MEASURE  = 0x3f86

''' SCD4X On-chip output signal compensation '''
## set temperature offset
SCD4X_SET_TEMPERATURE_OFFSET = 0x241d
## get temperature offset
SCD4X_GET_TEMPERATURE_OFFSET = 0x2318
## set sensor altitude
SCD4X_SET_SENSOR_ALTITUDE    = 0x2427
## get sensor altitude
SCD4X_GET_SENSOR_ALTITUDE    = 0x2322
## set ambient pressure
SCD4X_SET_AMBIENT_PRESSURE   = 0xe000

''' SCD4X Field calibration '''
## perform forced recalibration
SCD4X_PERFORM_FORCED_RECALIB = 0x362f
## set automatic self calibration enabled
SCD4X_SET_AUTOMATIC_CALIB    = 0x2416
## get automatic self calibration enabled
SCD4X_GET_AUTOMATIC_CALIB    = 0x2313

''' SCD4X Low power '''
## start low power periodic measurement, signal update interval is approximately 30 seconds.
SCD4X_START_LOW_POWER_MEASURE = 0x21ac
## get data ready status
SCD4X_GET_DATA_READY_STATUS   = 0xe4b8

''' SCD4X Advanced features '''
## persist settings
SCD4X_PERSIST_SETTINGS        = 0x3615
## get serial number
SCD4X_GET_SERIAL_NUMBER       = 0x3682
## perform self test
SCD4X_PERFORM_SELF_TEST       = 0x3639
## perform factory reset
SCD4X_PERFORM_FACTORY_RESET   = 0x3632
## reinit
SCD4X_REINIT                  = 0x3646

''' SCD4X Low power single shot '''
## measure single shot
SCD4X_MEASURE_SINGLE_SHOT          = 0x219d
## measure single shot rht only
SCD4X_MEASURE_SINGLE_SHOT_RHT_ONLY = 0x2196
## put the sensor from idle to sleep to reduce current consumption.
SCD4X_POWER_DOWN                   = 0x36e0
## wake up the sensor from sleep mode into idle mode.
SCD4X_WAKE_UP                      = 0x36f6


class DFRobot_SCD4X(object):
  '''!
    @brief Define DFRobot_SCD4X basic class
    @details Drive the CO2 sensor
  '''

  ''''''''''''''''''''''''''' Init and reset '''''''''''''''''''''''''''

  def __init__(self, i2c_addr=SCD4X_I2C_ADDR, bus=1):
    '''!
      @brief Module I2C communication init
      @param i2c_addr I2C communication address
      @param bus I2C bus
    '''
    self._addr = i2c_addr
    self._i2c = smbus.SMBus(bus)

  @property
  def begin(self):
    '''!
      @brief Initialize sensor
      @return  Return init status
      @retval True indicate initialization succeed
      @retval False indicate initialization failed
    '''
    self.enable_period_measure(SCD4X_STOP_PERIODIC_MEASURE)
    ret = True
    chip_id = self._get_serial_number
    logger.info("%#x" %chip_id[0] )
    logger.info("%#x" %chip_id[1] )
    logger.info("%#x" %chip_id[2] )
    # if SCD4X_SERIAL_NUMBER_WORD0 != chip_id[0]:
    #   ret = False
    return ret

  def set_sleep_mode(self, mode):
    '''!
      @brief Set the sensor as sleep or wake-up mode (SCD41 only)
      @param mode - sleep and wake-up mode:
      @n       SCD4X_POWER_DOWN : Put the sensor from idle to sleep to reduce current consumption.
      @n       SCD4X_WAKE_UP : Wake up the sensor from sleep mode into idle mode.
      @note Note that the SCD4x does not acknowledge the wake_up command. Command execution time : 20 ms
      @n When executing the command, the sensor can't be in period measurement mode
    '''
    self._write_data(mode, [])
    if(SCD4X_WAKE_UP == mode):
      time.sleep(0.02)

  @property
  def perform_self_test(self):
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
    self._write_data(SCD4X_PERFORM_SELF_TEST, [])
    time.sleep(10)
    buf = self._read_data(3)
    return (buf[0] << 8) | buf[1]

  @property
  def module_reinit(self):
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
    self._write_data(SCD4X_REINIT, [])
    time.sleep(0.02)

  @property
  def perform_factory_reset(self):
    '''!
      @brief perform factory reset
      @details The perform_factory_reset command resets all configuration settings stored 
      @n  in the EEPROM and erases the FRC and ASC algorithm history.
      @return None
      @note Command execution time : 1200 ms
      @n When executing the command, the sensor can't be in period measurement mode
    '''
    self._write_data(SCD4X_PERFORM_FACTORY_RESET, [])
    time.sleep(1.2)

  ''''''''''''''''''''''''''' Measurement Function '''''''''''''''''''''''''''

  def measure_single_shot(self, mode):
    '''!
      @brief measure single shot(SCD41 only)
      @details  On-demand measurement of CO2 concentration, relative humidity and temperature.
      @n  Get the measured data through readMeasurement(sSensorMeasurement_t data) interface
      @param mode - Single-measurement mode:
      @n       SCD4X_MEASURE_SINGLE_SHOT : On-demand measurement of CO2 concentration, relative humidity and temperature. 
      @n                                   Max command duration 5000 [ms].
      @n       SCD4X_MEASURE_SINGLE_SHOT_RHT_ONLY : On-demand measurement of relative humidity and temperature only.
      @n                                            Max command duration 50 [ms].
      @return None
      @note In SCD4X_MEASURE_SINGLE_SHOT_RHT_ONLY mode,  CO2 output is returned as 0 ppm.
      @n When executing the command, the sensor can't be in period measurement mode
    '''
    self._write_data(mode, [])
    if(SCD4X_MEASURE_SINGLE_SHOT == mode):
      time.sleep(5)
    elif(SCD4X_MEASURE_SINGLE_SHOT_RHT_ONLY == mode):
      time.sleep(0.05)

  def enable_period_measure(self, mode):
    '''!
      @brief set periodic measurement mode
      @param mode - periodic measurement mode:
      @n       SCD4X_START_PERIODIC_MEASURE : start periodic measurement, signal update interval is 5 seconds.
      @n       SCD4X_STOP_PERIODIC_MEASURE : stop periodic measurement command
      @n       SCD4X_START_LOW_POWER_MEASURE :  start low power periodic measurement, signal update interval is approximately 30 seconds.
      @return None
      @note The measurement mode must be disabled when changing the sensor settings; after giving the stop_periodic_measurement command, the sensor needs to wait 500ms before responding to other commands.
    '''
    self._write_data(mode, [])
    if(SCD4X_STOP_PERIODIC_MEASURE == mode):
      time.sleep(0.5)

  @property
  def read_measurement(self):
    '''!
      @brief Read the measured data
      @return The measured values from the sensor
      @retval CO2ppm - CO2 concentration (ppm)
      @retval temp - temperature (C)
      @retval humidity - humidity (RH)
      @note CO2 measurement range: 0~40000 ppm; temperature measurement range: -10~60 ℃; humidity measurement range: 0~100 %RH.
    '''
    self._write_data(SCD4X_READ_MEASUREMENT, [])
    buf = self._read_data(9)
    CO2ppm = (buf[0] << 8) | buf[1]

    temp = -45 + 175 * (float)((buf[3] << 8) | buf[4]) / (1 << 16)

    humidity = 100 * (float)((buf[6] << 8) | buf[7]) / (1 << 16)
    return CO2ppm, temp, humidity

  @property
  def get_data_ready_status(self):
    '''!
      @brief get data ready status
      @return data ready status:
      @n        True : data ready
      @n        False : data not ready
    '''
    self._write_data(SCD4X_GET_DATA_READY_STATUS, [])
    buf = self._read_data(3)
    if( 0x0000 == ( ((buf[0] << 8) | buf[1]) & 0x7FF ) ):
      return False
    return True

  ''''''''''''''''''''''''''' compensation and calibration '''''''''''''''''''''''''''

  def set_temp_comp(self, temp_comp):
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
    data = (int)(temp_comp * (1 << 16) / 175)
    send_pack = self._pack(data)
    self._write_data(SCD4X_SET_TEMPERATURE_OFFSET, send_pack)

  @property
  def get_temp_comp(self):
    '''!
      @brief get temperature offset
      @return The current set temp compensation value, unit ℃
      @note When executing the command, the sensor can't be in period measurement mode
    '''
    self._write_data(SCD4X_GET_TEMPERATURE_OFFSET, [])
    buf = self._read_data(3)
    if( buf[2] != self._calc_CRC((buf[0] << 8) | buf[1]) ):
      logger.info("The crc failed!")
    return 175 * (float)( (buf[0] << 8) | buf[1] ) / (1 << 16)

  def set_sensor_altitude(self, altitude):
    '''!
      @brief set sensor altitude
      @param altitude - the current ambient altitude, unit m
      @note When executing the command, the sensor can't be in period measurement mode
    '''
    send_pack = self._pack(altitude)
    self._write_data(SCD4X_SET_SENSOR_ALTITUDE, send_pack)

  @property
  def get_sensor_altitude(self):
    '''!
      @brief get sensor altitude
      @return The current set ambient altitude, unit m
      @note When executing the command, the sensor can't be in period measurement mode
    '''
    self._write_data(SCD4X_GET_SENSOR_ALTITUDE, [])
    buf = self._read_data(3)
    return (buf[0] << 8) | buf[1]

  def set_ambient_pressure(self, ambient_pressure):
    '''!
      @brief set ambient pressure
      @param ambient_pressure - the current ambient pressure, unit Pa
      @return None
    '''
    data = (int)(ambient_pressure / 100)
    send_pack = self._pack(data)
    self._write_data(SCD4X_SET_AMBIENT_PRESSURE, send_pack)

  def perform_forced_recalibration(self, CO2ppm):
    '''!
      @brief perform forced recalibration
      @param CO2ppm - Target CO2 concentration, unit ppm
      @return Calibration amplitude, return (int16_t)0x7fff if the calibration failed
      @note Command execution time : 400 ms
      @n When executing the command, the sensor can't be in period measurement mode
    '''
    send_pack = self._pack(CO2ppm)
    self._write_data(SCD4X_PERFORM_FORCED_RECALIB, send_pack)
    time.sleep(0.4)
    buf = self._read_data(3)
    return (int)(((buf[0] << 8) | buf[1]) - 0x8000)

  def set_auto_calib_mode(self, mode):
    '''!
      @brief set automatic self calibration enabled
      @param mode - automatic self-calibration mode:
      @n       True : enable automatic self-calibration
      @n       False : disable automatic self-calibration
      @return None
      @note When executing the command, the sensor can't be in period measurement mode
    '''
    data = 0
    if mode:
      data = 1
    send_pack = self._pack(data)
    self._write_data(SCD4X_SET_AUTOMATIC_CALIB, send_pack)

  @property
  def get_auto_calib_mode(self):
    '''!
      @brief get automatic self calibration enabled
      @return automatic self-calibration mode:
      @n        True : enable automatic self-calibration
      @n        False : disable automatic self-calibration
      @note When executing the command, the sensor can't be in period measurement mode
    '''
    self._write_data(SCD4X_GET_AUTOMATIC_CALIB, [])
    buf = self._read_data(3)
    if(0x0000 == (buf[0] << 8) | buf[1]):
      return False
    return True

  @property
  def persist_settings(self):
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
    self._write_data(SCD4X_PERSIST_SETTINGS, [])
    time.sleep(0.8)

  ''''''''''''''''''''''''''''''' get serial number '''''''''''''''''''''''''''''''

  @property
  def _get_serial_number(self):
    '''!
      @brief Reading out the serial number can be used to identify the chip and to 
      @n  verify the presence of the sensor. Together, the 3 words constitute a 
      @n  unique serial number with a length of 48 bits (big endian format).
      @return serial number
      @note When executing the command, the sensor can't be in period measurement mode
    '''
    self._write_data(SCD4X_GET_SERIAL_NUMBER, [])
    buf = self._read_data(9)
    data = [0] * 3
    data[0] = (buf[0] << 8) | buf[1]
    data[1] = (buf[3] << 8) | buf[4]
    data[2] = (buf[6] << 8) | buf[7]
    return data

  ''''''''''''''''''''''''''''' CRC Check & Sending Data Pack '''''''''''''''''''''''''''''

  def _calc_CRC(self, data):
    '''!
      @brief Calculate the current crc check code to contrast it with the MISR read from the sensor
      @param data - The measured data just obtained from the sensor
      @return The current calculated crc check code
    '''
    crc = SCD4X_CRC8_INIT
    buf = [((data >> 8) & 0xFF), (data & 0xFF)]

    # calculates 8-Bit checksum with given polynomial
    for current_byte in range(0, 2):
      crc ^= (buf[current_byte])
      for crc_bit in range(0, 8):
        # logger.info("---")
        if (crc & 0x80):
          crc = (crc << 1) ^ SCD4X_CRC8_POLYNOMIAL
        else:
          crc = (crc << 1)
    return crc & 0xFF

  def _pack(self, data):
    '''!
      @brief Pack the data to be sent
      @param data - The data to be sent
      @return The packed data to be sent
    '''
    buf = [0] * 3
    buf[0] = ((data >> 8) & 0xFF)
    buf[1] = (data & 0xFF)
    buf[2] = self._calc_CRC(data)
    return buf

  ''''''''''''''''''''''''''''''''''' Read/Write Command Function '''''''''''''''''''''''''''''''''''

  def _write_data(self, cmd, data):
    '''!
      @brief writes data to a register
      @param cmd register address
      @param data written data
    '''
    if isinstance(data, int):
      data = [data]
      #logger.info(data)
    data.insert(0, (cmd & 0xFF))
    # self._i2c.write_i2c_block_data(self._addr, (cmd >> 8) & 0xFF, data)
    try:
      self._i2c.write_i2c_block_data(self._addr, (cmd >> 8) & 0xFF, data)
    except IOError:
      print("[Errno 121] Remote I/O error")

  def _read_data(self, length):
    '''!
      @brief read the data from the register
      @param length read data length
      @return read data list
    '''
    # self._write_data(cmd, [])
    # return self._i2c.read_i2c_block_data(self._addr, 0x00, length)
    try:
      return self._i2c.read_i2c_block_data(self._addr, 0x00, length)
    except IOError:
      print("[Errno 121] Remote I/O error")
      return [0] * length
