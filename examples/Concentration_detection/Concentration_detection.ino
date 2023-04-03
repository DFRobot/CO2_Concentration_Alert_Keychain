/*!
 * @file Concentration_detection.ino
 * @brief CO2 concentration alert keychain
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @author Pai Da Xing
 * @version  V1.0
 * @date  2023-3-20
 * @url 
 */
#include <DFRobot_SCD4X.h>
#include "FastLED.h"
#define NUM_LEDS 10
#define DATA_PIN 20
#define LED_TYPE WS2812
#define COLOR_ORDER GRB
#include "DFRobot_GDL.h"
#define TFT_DC 1
#define TFT_CS 7
#define TFT_RST 6

DFRobot_ST7789_172x320_HW_SPI screen(/*dc=*/TFT_DC, /*cs=*/TFT_CS, /*rst=*/TFT_RST);
CRGB leds[NUM_LEDS];
DFRobot_SCD4X SCD4X(&Wire, /*i2cAddr = */ SCD4X_I2C_ADDR);
int value = 0;  
int Mapping, Mapping0, Mapping1, Mapping2, Mapping3, Mapping4, Mapping5, Mapping6, Mapping7;
int frequency = 0;
int quantity=1;


void setup(void) {
  Serial.begin(115200);
  pinMode(DATA_PIN, OUTPUT);
  LEDS.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  Mapping0 = Mapping1 = Mapping2 = Mapping3 = Mapping4 = Mapping5 = Mapping6 = Mapping7 = 315;
  screen.begin();

  while (!SCD4X.begin()) {
    Serial.println("Communication with device failed, please check connection");
    delay(3000);
  }
  Serial.println("Begin ok!");
  SCD4X.enablePeriodMeasure(SCD4X_STOP_PERIODIC_MEASURE);
  SCD4X.setTempComp(4.0);
  float temp = 0;
  temp = SCD4X.getTempComp();
  Serial.print("The current temperature compensation value : ");
  Serial.print(temp);
  Serial.println(" C");
  SCD4X.setSensorAltitude(540);
  uint16_t altitude = 0;
  altitude = SCD4X.getSensorAltitude();
  Serial.print("Set the current environment altitude : ");
  Serial.print(altitude);
  Serial.println(" m");
  SCD4X.enablePeriodMeasure(SCD4X_START_PERIODIC_MEASURE);
  Serial.println();
}

void loop() {
  led();
  if (SCD4X.getDataReadyStatus()) {
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

    Mapping = map(data.CO2ppm, 400, 5000, 310, 190);
    frequency = map(data.CO2ppm, 400, 5000, 50, 0);
    quantity = map(data.CO2ppm, 400, 5000, 1, 10);
    
    Mapping7 = Mapping6;
    Mapping6 = Mapping5;
    Mapping5 = Mapping4;
    Mapping4 = Mapping3;
    Mapping3 = Mapping2;
    Mapping2 = Mapping1;
    Mapping1 = Mapping0;
    Mapping0 = Mapping;

    screen.fillScreen(COLOR_RGB565_BLACK);    
    screen.setTextColor(COLOR_RGB565_LGRAY);  
    screen.setFont(&FreeMono12pt7b);          
    screen.setTextSize(2);
    screen.setCursor(/*x=*/2, /*y=*/30);
    screen.print("CO2:");
    screen.setCursor(/*x=*/0, /*y=*/65);
    screen.print(data.CO2ppm);

    screen.setTextSize(1);
    screen.setFont(&FreeMono12pt7b);  
    screen.setCursor(/*x=*/110, /*y=*/57);
    screen.print(" ppm");

    screen.setCursor(/*x=*/0, /*y=*/90);
    screen.print("Temp:");
    screen.setCursor(/*x=*/0, /*y=*/110);
    screen.print(data.temp);
    screen.setCursor(/*x=*/75, /*y=*/110);
    screen.print(" C");

    screen.setCursor(/*x=*/0, /*y=*/130);
    screen.print("RH:");
    screen.setCursor(/*x=*/0, /*y=*/150);
    screen.print(data.humidity);
    screen.setCursor(/*x=*/75, /*y=*/150);
    screen.print(" %");

    screen.setFont(&FreeMono9pt7b);  
    screen.setCursor(/*x=*/1, /*y=*/190);
    screen.print("5K");

    screen.writeLine(10, 200, 10, 320, COLOR_RGB565_LGRAY);
    screen.writeLine(2, 315, 170, 315, COLOR_RGB565_LGRAY);

    screen.writeLine(26, Mapping6, 10, Mapping7, COLOR_RGB565_LGRAY);
    screen.writeLine(50, Mapping5, 26, Mapping6, COLOR_RGB565_LGRAY);
    screen.writeLine(74, Mapping4, 50, Mapping5, COLOR_RGB565_LGRAY);
    screen.writeLine(98, Mapping3, 74, Mapping4, COLOR_RGB565_LGRAY);
    screen.writeLine(122, Mapping2, 98, Mapping3, COLOR_RGB565_LGRAY);
    screen.writeLine(146, Mapping1, 122, Mapping2, COLOR_RGB565_LGRAY);
    screen.writeLine(170, Mapping0, 146, Mapping1, COLOR_RGB565_LGRAY);
    screen.setTextWrap(true);
  }
}


void led() {
  for (; value < 50; value++) {
    FastLED.setBrightness(value);
    if (frequency > 35) {
      fill_solid(leds, quantity, CRGB::Green);
    } else {
      fill_solid(leds, quantity, CRGB::Red);
    }
    FastLED.show();
    Serial.println(value);
    delay(frequency);
  }
  for (; value > 0; value--) {
    FastLED.setBrightness(value);
    if (frequency > 35) {
      fill_solid(leds, quantity, CRGB::Green);
    } else {
      fill_solid(leds, quantity, CRGB::Red);
    }
    FastLED.show();
    Serial.println(value);
    delay(frequency);
  }
}