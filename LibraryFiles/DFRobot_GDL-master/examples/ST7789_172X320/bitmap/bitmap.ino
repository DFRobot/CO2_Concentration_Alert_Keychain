#include "DFRobot_GDL.h"
#include "Bitmap.h"
#include "XBitmap.h"
#include "RGBBitmap.h"

#if defined ARDUINO_SAM_ZERO
#define TFT_DC  7
#define TFT_CS  5
#define TFT_RST 6
#define TFT_BL  9
/*ESP32 and ESP8266*/
#elif defined(ESP32) || defined(ESP8266)
#define TFT_DC  D2
#define TFT_CS  D6
#define TFT_RST D3
#define TFT_BL  D4
/* AVR series mainboard */
#else
#define TFT_DC  2
#define TFT_CS  3
#define TFT_RST 4
#define TFT_BL  5
#endif


DFRobot_ST7789_172x320_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST,/*bl=*/TFT_BL);
/* M0 mainboard DMA transfer */
//DFRobot_ST7789_172x320_DMA_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST,/*bl=*/TFT_BL);

void setup() {
  Serial.begin(115200);
  screen.begin();
  screen.fillScreen(COLOR_RGB565_WHITE );
}

void loop() {
  // /**
  //  * @brief Paint the screen white
  //  * @param 0xFFFF white parameter
  //  */
  // screen.fillScreen(COLOR_RGB565_WHITE );
  // /**
  //  * @brief Draw monochrome pictures with different drawing orders
  //  * @param x 0 Set the starting point to be at the upper left of the screen, near the left side
  //  * @param y 0 Near the upper side 
  //  * @param bitmap gImage_XBitmap The array in the header file XBitmap, the array elements are single bytes
  //  * @param w 240 Picture width
  //  * @param h 240 Picture height
  //  * @param color 0x0000 The black picture part is set to black
  //  */
  screen.drawXBitmap(/*x=*/0,/*y=*/0,/*bitmap gImage_Bitmap=*/gImage_XBitmap,/*w=*/160,/*h=*/160,/*color=*/0x0000);
  // screen.fillScreen(COLOR_RGB565_WHITE);
  // /**
  //  * @brief Draw colorful picture
  //  * @param x 0 Set the starting point to be at the upper left of the screen, near the left side
  //  * @param y 0 Near the upper side
  //  * @param bitmap gImage_RGBBitmap The array of RGBBitmap header files, the array elements are single bytes, here forced to convert to 2 bytes
  //  * @param w 240 picture width
  //  * @param h 240 Picture height
  //  */
  // screen.drawRGBBitmap(/*x=*/(screen.width()-146)/2,/*y=*/(screen.height()-128)/2,/*bitmap gImage_Bitmap=*/(const unsigned uint16_t*)gImage_RGBBitmap,/*w=*/146,/*h=*/128);
  // screen.fillScreen(COLOR_RGB565_BLACK);
  // /**
  //  * @brief Draw a monochrome picture without background circularly, and the color is changing constantly.
  //  * @param x 0 Set the starting point to be at the upper left of the screen, near the left side
  //  * @param y 0 Near the upper side
  //  * @param bitmap gImage_Bitmap The array in the header file Bitmap, the array elements are single bytes
  //  * @param w 240 Picture width
  //  * @param h 240 Picture height
  //  * @param color i Set the black image part to bluish color
  //  */
  // for (int16_t i = 0x00ff; ; i+=0x3300) {
  //   screen.drawBitmap(/*x=*/(screen.width()-146)/2,/*y=*/(screen.height()-128)/2,/*bitmap gImage_Bitmap=*/gImage_Bitmap,/*w=*/146,/*h=*/128,/*color=*/i);
  // }

}
