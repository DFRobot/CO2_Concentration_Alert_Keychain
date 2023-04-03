/*!
 * @file DFRobot_SCD4X.cpp
 * @brief  Define the infrastructure DFRobot_SCD4X class
 * @details  Configure the sensor with single-measurement mode, period measurement mode and compensation and calibration using I2C communication.
 * @copyright Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author [qsjhyy](yihuan.huang@dfrobot.com)
 * @version  V1.0
 * @date  2022-05-09
 * @url https://github.com/DFRobot/DFRobot_SCD4X
 */
#include "DFRobot_SCD4X.h"

/**************************** Init and reset ********************************/

DFRobot_SCD4X::DFRobot_SCD4X(TwoWire *pWire, uint8_t i2cAddr)
{
  _deviceAddr = i2cAddr;
  _pWire = pWire;
}

bool DFRobot_SCD4X::begin(void)
{
  _pWire->begin();   // Wire.h(I2C)library function initialize wire library

  enablePeriodMeasure(SCD4X_STOP_PERIODIC_MEASURE);

  uint16_t buf[3];
  if( !getSerialNumber(buf) ) {
    return false;
  }

  DBG("begin ok!");
  return true;
}

void DFRobot_SCD4X::setSleepMode(uint16_t mode)
{
  writeData(mode, NULL, 0);
  if(SCD4X_WAKE_UP == mode)
    delay(20);   // Give it some time to switch mode
}

uint16_t DFRobot_SCD4X::performSelfTest(void)
{
  writeData(SCD4X_PERFORM_SELF_TEST, NULL, 0);

  delay(10000);

  _pWire->requestFrom(_deviceAddr, (uint8_t)3);   // Master device requests size bytes from slave device, which can be accepted by master device with read() or available()

  uint8_t count = 0;
  uint8_t buf[3] = {0};

  while (_pWire->available()) {
    buf[count++] = _pWire->read();   // Use read() to receive and put into buf
  }

  return SCD4X_CONCAT_BYTES(buf[0], buf[1]);
}

void DFRobot_SCD4X::moduleReinit(void)
{
  writeData(SCD4X_REINIT, NULL, 0);

  delay(20);
}

void DFRobot_SCD4X::performFactoryReset(void)
{
  writeData(SCD4X_PERFORM_FACTORY_RESET, NULL, 0);

  delay(1200);
}

/********************************* Measurement Function *************************************/

void DFRobot_SCD4X::measureSingleShot(uint16_t mode)
{
  writeData(mode, NULL, 0);

  if(SCD4X_MEASURE_SINGLE_SHOT == mode) {
    delay(5000);
  } else if(SCD4X_MEASURE_SINGLE_SHOT_RHT_ONLY == mode) {
    delay(50);
  }
}

void DFRobot_SCD4X::enablePeriodMeasure(uint16_t mode)
{
  writeData(mode, NULL, 0);

  if(SCD4X_STOP_PERIODIC_MEASURE == mode)
    delay(500);   // Give it some time to switch mode
}

void DFRobot_SCD4X::readMeasurement(sSensorMeasurement_t * data)
{
  uint8_t buf[9] = {0};
  readData(SCD4X_READ_MEASUREMENT, buf, sizeof(buf));

  data->CO2ppm = SCD4X_CONCAT_BYTES(buf[0], buf[1]);

  data->temp = -45 + 175 * (float)(SCD4X_CONCAT_BYTES(buf[3], buf[4])) / ((uint32_t)1 << 16);
  DBG(SCD4X_CONCAT_BYTES(buf[3], buf[4]));
  DBG((float)(SCD4X_CONCAT_BYTES(buf[3], buf[4])));
  DBG((float)(SCD4X_CONCAT_BYTES(buf[3], buf[4])) / ((uint32_t)1 << 16));
  DBG((uint32_t)1 << 16);
  DBG(175 * (float)(SCD4X_CONCAT_BYTES(buf[3], buf[4])) / ((uint32_t)1 << 16));
  DBG(data->temp);

  data->humidity = 100 * (float)(SCD4X_CONCAT_BYTES(buf[6], buf[7])) / ((uint32_t)1 << 16);
}

bool DFRobot_SCD4X::getDataReadyStatus(void)
{
  uint8_t buf[3] = {0};
  readData(SCD4X_GET_DATA_READY_STATUS, buf, sizeof(buf));
  if(0x0000 == (SCD4X_CONCAT_BYTES(buf[0], buf[1]) & 0x7FF)){
    return false;
  }
  return true;
}

/*************************** compensation and calibration ********************************/

void DFRobot_SCD4X::setTempComp(float tempComp)
{
  uint16_t data = (uint16_t)(tempComp * ((uint32_t)1 << 16) / 175);
  uint8_t * sendPack  = pack(data);

  writeData(SCD4X_SET_TEMPERATURE_OFFSET, sendPack, 3);

  free(sendPack);
}

float DFRobot_SCD4X::getTempComp(void)
{
  uint8_t buf[3] = {0};
  readData(SCD4X_GET_TEMPERATURE_OFFSET, buf, sizeof(buf));
  if(buf[2] != calcCRC(SCD4X_CONCAT_BYTES(buf[0], buf[1]))) {
    DBG("The crc failed!");
  }
  return 175 * (float)( SCD4X_CONCAT_BYTES(buf[0], buf[1]) ) / ((uint32_t)1 << 16);
}

void DFRobot_SCD4X::setSensorAltitude(uint16_t altitude)
{
  uint8_t * sendPack = pack(altitude);

  writeData(SCD4X_SET_SENSOR_ALTITUDE, sendPack, 3);

  free(sendPack);
}

uint16_t DFRobot_SCD4X::getSensorAltitude(void)
{
  uint8_t buf[3] = {0};
  readData(SCD4X_GET_SENSOR_ALTITUDE, buf, sizeof(buf));
  return SCD4X_CONCAT_BYTES(buf[0], buf[1]);
}

void DFRobot_SCD4X::setAmbientPressure(uint32_t ambientPressure)
{
  uint16_t data = (uint16_t)(ambientPressure / 100);
  uint8_t * sendPack = pack(data);

  writeData(SCD4X_SET_AMBIENT_PRESSURE, sendPack, 3);

  free(sendPack);
}

int16_t DFRobot_SCD4X::performForcedRecalibration(uint16_t CO2ppm)
{
  uint8_t * sendPack = pack(CO2ppm);

  writeData(SCD4X_PERFORM_FORCED_RECALIB, sendPack, 3);

  free(sendPack);

  delay(400);   // command execution time

  _pWire->requestFrom(_deviceAddr, (uint8_t)3);   // Master device requests size bytes from slave device, which can be accepted by master device with read() or available()

  uint8_t count = 0;
  uint8_t buf[3] = {0};

  while (_pWire->available()) {
    buf[count++] = _pWire->read();   // Use read() to receive and put into buf
  }

  return (int16_t)(SCD4X_CONCAT_BYTES(buf[0], buf[1]) - 0x8000);
}

void DFRobot_SCD4X::setAutoCalibMode(bool mode)
{
  uint16_t data = 0;
  if(mode) {
    data = 1;
  }
  uint8_t * sendPack = pack(data);

  writeData(SCD4X_SET_AUTOMATIC_CALIB, sendPack, 3);

  free(sendPack);
}

bool DFRobot_SCD4X::getAutoCalibMode(void)
{
  uint8_t buf[3] = {0};
  readData(SCD4X_GET_AUTOMATIC_CALIB, buf, sizeof(buf));
  if(0x0000 == SCD4X_CONCAT_BYTES(buf[0], buf[1])){
    return false;
  }
  return true;
}

void DFRobot_SCD4X::persistSettings(void)
{
  writeData(SCD4X_PERSIST_SETTINGS, NULL, 0);

  delay(800);
}

/*************************** get serial number *****************************/

bool DFRobot_SCD4X::getSerialNumber(uint16_t * wordBuf)
{
  bool ret = true;
  uint8_t buf[9] = {0};
  if(sizeof(buf) != readData(SCD4X_GET_SERIAL_NUMBER, buf, sizeof(buf))) {
    ret = false;
  }
  wordBuf[0] = SCD4X_CONCAT_BYTES(buf[0], buf[1]);
  wordBuf[1] = SCD4X_CONCAT_BYTES(buf[3], buf[4]);
  wordBuf[2] = SCD4X_CONCAT_BYTES(buf[6], buf[7]);
  DBG(wordBuf[0], HEX);
  DBG(wordBuf[1], HEX);
  DBG(wordBuf[2], HEX);
  return ret;
}

/*********************** CRC Check & Sending Data Pack *************************/

uint8_t DFRobot_SCD4X::calcCRC(uint16_t data)
{
  uint16_t current_byte;
  uint8_t crc = SCD4X_CRC8_INIT;
  uint8_t crc_bit;
  uint8_t buf[2] = {(uint8_t)((data >> 8) & 0xFF), (uint8_t)(data & 0xFF)};

  /* calculates 8-Bit checksum with given polynomial */
  for (current_byte = 0; current_byte < 2; ++current_byte) {
    crc ^= (buf[current_byte]);
    for (crc_bit = 8; crc_bit > 0; --crc_bit) {
      if (crc & 0x80)
        crc = (crc << 1) ^ SCD4X_CRC8_POLYNOMIAL;
      else
        crc = (crc << 1);
    }
  }
  return crc;
}

uint8_t * DFRobot_SCD4X::pack(uint16_t data)
{
  uint8_t * pBuf = (uint8_t *)malloc(sizeof(uint8_t) * 3);
  memset(pBuf, 0, sizeof(uint8_t) * 3);
  pBuf[0] = (uint8_t)((data >> 8) & 0xFF);
  pBuf[1] = (uint8_t)(data & 0xFF);
  pBuf[2] = calcCRC(data);

  return pBuf;
}

/****************************** Read/Write Command Function ********************************/

void DFRobot_SCD4X::writeData(uint16_t cmd, const void* pBuf, size_t size)
{
  // if(pBuf == NULL) {
  //   DBG("pBuf ERROR!! : null pointer");
  // }
  uint8_t * _pBuf = (uint8_t *)pBuf;

  _pWire->beginTransmission(_deviceAddr);
  _pWire->write((uint8_t)((cmd >> 8) & 0xFF));
  _pWire->write((uint8_t)(cmd & 0xFF));

  for(size_t i = 0; i < size; i++) {
    // DBG(_pBuf[i]);
    _pWire->write(_pBuf[i]);
  }
  _pWire->endTransmission();
}

size_t DFRobot_SCD4X::readData(uint16_t cmd, void* pBuf, size_t size)
{
  size_t count = 0;
  if(NULL == pBuf) {
    DBG("pBuf ERROR!! : null pointer");
  }
  uint8_t * _pBuf = (uint8_t*)pBuf;

  _pWire->beginTransmission(_deviceAddr);
  _pWire->write((uint8_t)((cmd >> 8) & 0xFF));
  _pWire->write((uint8_t)(cmd & 0xFF));

  if(0 != _pWire->endTransmission()) {   // Used Wire.endTransmission() to end a slave transmission started by beginTransmission() and arranged by write().
    DBG("endTransmission ERROR!!");
  } else {
    _pWire->requestFrom(_deviceAddr, (uint8_t)size);   // Master device requests size bytes from slave device, which can be accepted by master device with read() or available()
    
    while (_pWire->available()) {
      _pBuf[count++] = _pWire->read();   // Use read() to receive and put into buf
    }
    // _pWire->endTransmission();
  }
  return count;
}
