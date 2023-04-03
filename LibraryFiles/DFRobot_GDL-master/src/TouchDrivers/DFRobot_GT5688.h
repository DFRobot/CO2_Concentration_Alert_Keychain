#ifndef __DFROBOT_GT5688_H
#define __DFROBOT_GT5688_H

// GT5688 configuration array table
// Maximum x-coordinate output 0x0320 = 800
// Maximum y-coordinate output 0x01E0 = 480
// 0x8050 ~ 0x813B 236 registers
// 0x813C 0x813D checksum
// 0x813E configuration update flag
// Array organization
// 1. Register start address 0x8050
// 2. Number of data
// 0 register bit, 1 the bit of data stored in each register 16bit-reg 8bit-data
// The first 5 data are control data: register bit, register value bit, start register, configuration data number
static const uint8_t PROGMEM touchGt5688ConfigTable[] = {
2,// 1-> The register is 8 bits, the 2 register is 16 bits, the 4 register is 32 bits    0
1,// 1-> The register value is 8 bits, the 2 register value is 16 and the 4 register value is 32   1
0x80, 0x50,//Register start address 0x8050 2 3
0x00, 0xEF,//Number of configuration data total = 0x00EF 4 5
0x42,0x20,0x03,0xE0,0x01,0x05,0x3D,0x10,0x01,0x00,
	0x08,0x08,0x50,0x3C,0x53,0x11,0x00,0x00,0x00,0x00,
	0x14,0x14,0x14,0x22,0x08,0x04,0x00,0x00,0x00,0x00,
	0x3C,0x00,0x53,0x00,0x14,0x00,0x00,0x84,0x00,0x00,
	0x00,0x00,0x00,0x64,0x1E,0x1E,0x8A,0x2A,0x0C,0x3C,
	0x3E,0xF4,0x0A,0x20,0x33,0x60,0x12,0x02,0x24,0x00,
	0x00,0x32,0x64,0x00,0x14,0x02,0x00,0x00,0x54,0x80,
	0x35,0x7F,0x3D,0x7F,0x46,0x7F,0x51,0x7F,0x5D,0x7F,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x50,0x3C,
	0xBB,0xBB,0x07,0x00,0x00,0x00,0x02,0x0F,0x14,0x03,
	0x04,0x10,0x42,0xF8,0x0F,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x32,0x20,0x50,0x3C,0x3C,0x00,0x00,0x00,0x00,0x00,
	0x0D,0x06,0x0C,0x05,0x0B,0x04,0x0A,0x03,0x09,0x02,
	0x08,0x01,0xFF,0xFF,0x00,0x01,0x02,0x03,0x04,0x05,
	0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
	0x10,0x11,0x12,0x13,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x3C,0x00,0x05,0x1E,0x00,0x02,
	0x2A,0x1E,0x19,0x14,0x02,0x00,0x03,0x0A,0x05,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x04,
	0x22,0x03,0x00,0x00,0x33,0x00,0x14,0x00,0x00,0x28,
	0x46,0x32,0x50,0x00,0x00,0x00,0xF9,0xBD,0x01
};


#endif