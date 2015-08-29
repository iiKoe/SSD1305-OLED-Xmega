/************************************************************************************
 Title	:   C include file for the SSD1305 OLED / graphical LCD library (oled.c)
 Author:    Vito
 File:	    oled.h, version 1.0, 7/12/2012
 Software:  AVR-GCC 4.6.2
 Hardware:  any AVR XMEGA

 DESCRIPTION
       Basic routines for displaying text or bitmaps on SSD1305 based OLED display

	   This library operates in 8-bit I/O mode
	   To control the functions, 5 I/O ports are used.
	   This library does not support SPI or I2C mode

 USAGE
       See the C include oled.h file for a description of each function
       
***********************************************************************************/

#include <stdio.h>
#include "oled.h"


FILE oled_stream = FDEV_SETUP_STREAM(oled_putc, NULL, _FDEV_SETUP_WRITE); 



/*************************************************************************
Display character at current cursor position
Input:    character to be displayed
Returns:  none
*************************************************************************/
void oled_putc(char ascii)
{
	static uint8_t char_num = 0;
	static uint8_t line_num = 0;
	
	if (++char_num > 21 || ascii=='\n')
	{
		char_num = 0;
		if (++line_num>8) line_num = 0;
		oled_gotoline(line_num);
	}
	else
	{
		uint8_t i;
		ascii -= 32;
		for(i = 0; i < 5; i++)
		oled_wr_data(pgm_read_byte(font5x7 + (5 * ascii) + i));
		oled_wr_data(0x00);
	}
}



/*************************************************************************
Display string at current position
Input:    string to be displayed
Returns:  none
*************************************************************************/
void oled_puts(const char * s)
{
	register char c;
	
	while( ( c = *s++))
		oled_putc(c);
}



/*************************************************************************
Display bitmap 
Input:    Bitmap to be displayed
Returns:  none
*************************************************************************/
void oled_bitmap(char bmp[])
{
	uint8_t page, column;
	uint16_t pos = 0;
	oled_gotoxy(0,0);
	for(page=0; page<8; page++) 
	{
		for(column=0; column<SCREEN_WIDTH; column++) 
			oled_wr_data(bmp[pos++]);
	}
}



/*************************************************************************
Set cursor to specified position
Input:    x  horizontal position  (0: left most position)
          y  vertical position    (0: first pixel)
Returns:  none
*************************************************************************/
void oled_gotoxy(unsigned char x,unsigned char y)
{
	if ((x < 0) || (x >= SCREEN_WIDTH) || (y < 0) || (y >= (SCREEN_HIGHT)))
	return;
	
	y=y/8;
	oled_wr_command(OLED_SETCOLUMNADDRESS);
	oled_wr_command(x);
	oled_wr_command(SCREEN_WIDTH-1);
	oled_wr_command(OLED_SET_PAGE_START+y);
}



/*************************************************************************
Set cursor to specified line 
Input:    y  vertical position    (0: first line)
Returns:  none
*************************************************************************/
void oled_gotoline(uint8_t y)
{
	if ((y < 0) || (y >= (SCREEN_HIGHT/8)))
	return;
		
	oled_wr_command(OLED_SETCOLUMNADDRESS);
	oled_wr_command(0);
	oled_wr_command(SCREEN_WIDTH-1);
	oled_wr_command(OLED_SET_PAGE_START+y);
}



/*************************************************************************
Set pixel at specified location
Input:    x  horizontal position  (0: left most pixel)
          y  vertical position    (0: top most pixel)
Returns:  none
*************************************************************************/
void oled_clearpixel(unsigned char x, unsigned char y)
{
	if ((x < 0) || (x >= SCREEN_WIDTH) || (y < 0) || (y >= SCREEN_HIGHT))
	return;
	
	uint8_t temp;
	oled_gotoxy(x, y);
	temp = oled_rd_data();
	oled_gotoxy(x, y);
	temp &= ~(1 << (y % 8));
	oled_wr_data(temp);
}



/*************************************************************************
Clear display and set cursor to home position
Input:    none
Returns:  none
*************************************************************************/
void oled_clear(void) 
{
	uint8_t page, column;
	oled_gotoxy(0,0);
	for(page=0; page<8; page++) 
	{
		for(column=0; column<SCREEN_WIDTH; column++) 
			oled_wr_data(0x00);
	}
}



/*************************************************************************
Write data to the SSD1305 controller
Input:    Data to be send
Returns:  none
*************************************************************************/
void oled_wr_data(uint8_t data)
{
	OLED_DATA_PORT = data;
	OLED_CONTROL_PORT |=  (OLED_RD | OLED_DC);
	OLED_CONTROL_PORT &= ~(OLED_CS);
	OLED_CONTROL_PORT &= ~(OLED_WR);
	OLED_CONTROL_PORT |=  (OLED_WR);
	OLED_CONTROL_PORT |=  (OLED_CS);
}



/*************************************************************************
Write command to the SSD1305 controller
Input:    Command to be send
Returns:  none
*************************************************************************/
void oled_wr_command(uint8_t command)
{
	OLED_DATA_PORT = command;
	OLED_CONTROL_PORT &= ~(OLED_CS | OLED_DC);
	OLED_CONTROL_PORT |=  (OLED_RD);
	OLED_CONTROL_PORT &= ~(OLED_WR);
	OLED_CONTROL_PORT |=  (OLED_WR);
	OLED_CONTROL_PORT |=  (OLED_CS);
}



/*************************************************************************
Read data from the SSD1305 controller
Input:    none
Returns:  8-bit pixel data
*************************************************************************/
uint8_t oled_rd_data(void)
{
	unsigned char temp;
	OLED_DATA_DIR = 0x00;
	OLED_CONTROL_PORT &= ~(OLED_CS);
	OLED_CONTROL_PORT |=  (OLED_DC | OLED_WR);
	OLED_CONTROL_PORT &= ~(OLED_RD);					
	OLED_CONTROL_PORT |=  (OLED_RD);
	OLED_CONTROL_PORT &= ~(OLED_RD);
	OLED_CONTROL_PORT |=  (OLED_RD);
	OLED_CONTROL_PORT |=  (OLED_CS);
	temp =  OLED_DATA_PIN;
	OLED_DATA_DIR =  0xFF;
	return temp;
}



/*************************************************************************
Initialize the ports used for to send data to the OLED controller
The OLED uses 8-bits for data and 5-bits for commands
*************************************************************************/
void oled_init_ports(void)
{
	OLED_CONTROL_DIR  |=	(OLED_DC | OLED_WR | OLED_RD | OLED_CS | OLED_RES);
	OLED_DATA_DIR	   =	 0xFF;
	OLED_CONTROL_PORT &= ~	(OLED_DC | OLED_WR | OLED_RD | OLED_CS | OLED_RES);
	OLED_CONTROL_PORT |=	(OLED_RES);
	_delay_us(15);
	OLED_CONTROL_PORT &= ~	(OLED_RES);
	_delay_us(15);
	OLED_CONTROL_PORT |=	(OLED_RES);
	_delay_us(15);
}



/*************************************************************************
Initialize the SSD1305 settings
	 Source for these settings is the Adafruit SSD1306 library
	 located at: https://github.com/adafruit/Adafruit_SSD1306
	 The Adafruit library is a arduino library for communication with a
	 SSD1306 controller trough SPI or I2C. Initialization of the 
	 SSD1306 is almost similar to that of the SSD1305	 
*************************************************************************/
void oled_init(void)
{
	oled_init_ports();								
	oled_wr_command(OLED_DISPLAYOFF);				// 0xAE
	oled_wr_command(OLED_SETDISPLAYCLOCKDIV);		// 0xD5
	oled_wr_command(0x80);							// the suggested ratio 0x80 
	oled_wr_command(OLED_SETMULTIPLEX);				// 0xA8
	oled_wr_command(0x3F);							// 0x3F 36MUX
	oled_wr_command(OLED_SETDISPLAYOFFSET);			// 0xD3
	oled_wr_command(0x0);							// no offset
	oled_wr_command(OLED_SETSTARTLINE);				// line 0
	//oled_wr_command(OLED_SEGREMAP_NORM);			// 0xA0 - default after reset
	oled_wr_command(OLED_COMSCANDIR_REMAP);			// 0xC8	 remapped mode
	oled_wr_command(OLED_SETCOMPINS);				// 0xDA	COM Pins Hardware Configuration	
	oled_wr_command(0x12);							// disable COM left/right remap
	oled_wr_command(OLED_SETCONTRAST);				// contrast
	oled_wr_command(0xFF);							// full contrast [0-255]
	oled_wr_command(OLED_SETPRECHARGE);				// 0xD9
	oled_wr_command(0xF1);							// phases
	oled_wr_command(OLED_SETVCOMDESEL);				// 0xDB
	oled_wr_command(0x40);							// V COMH = ~ 0.43 x VCC
	oled_wr_command(OLED_DISPLAYALLON_RESUME);		// 0xA4
	oled_wr_command(OLED_SETDISPLAY_NORM);			// 0xA6 - default after reset
	oled_wr_command(OLED_MEMORYMODE);				// 0x20
	oled_wr_command(0x00);							// horizontal mode
	//oled_wr_command(OLED_SETLOWCOLUMN | 0x0);		// low col = 0 - default after reset
	//oled_wr_command(OLED_SETHIGHCOLUMN | 0x0);	// hi col  = 0 - default after reset
	oled_wr_command(OLED_SETCOLUMNADDRESS);			// 0x21
	oled_wr_command(0);								// start address
	oled_wr_command(SCREEN_WIDTH-1);				// end address
	oled_wr_command(OLED_DISPLAYON);				// turn on OLED display
	oled_clear();
	
	stdout = &oled_stream;
}

