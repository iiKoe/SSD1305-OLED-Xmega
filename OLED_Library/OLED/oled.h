/************************************************************************************
 Title	:   C include file for the SSD1305 OLED / graphical LCD library (oled.c)
 Author:    Vito
 File:	    oled.h, version 1.0, 7/12/2012
 Software:  AVR-GCC 4.6.2
 Hardware:  any AVR XMEGA
 Code:		include oled.h in main code
************************************************************************************/

/**
 DESCRIPTION
	This library operates in 8-bit I/O mode
	To control the functions, 5 I/O ports are used. 
	This library does not support SPI or I2C mode
*/

#ifndef OLED_H_
#define OLED_H_

#define F_CPU 2000000UL
#include <util/delay.h>
#include "font.h"

/**
 *  @name     Pin definitions for SSD1305 control
 */
#define OLED_DATA_PORT				PORTC_OUT
#define OLED_DATA_DIR				PORTC_DIR
#define OLED_DATA_PIN				PORTC_IN

#define OLED_CONTROL_PORT			PORTD_OUT
#define OLED_CONTROL_DIR			PORTD_DIR

#define OLED_RD						PIN4_bm	//Read
#define OLED_WR						PIN2_bm	//Write
#define OLED_DC						PIN3_bm	//Data Or Command
#define OLED_RES					PIN1_bm	//Reset
#define OLED_CS						PIN0_bm	//Chip select


/**
 *  @name     Screen Width
 */
#define SCREEN_WIDTH				128
#define SCREEN_HIGHT				64


/**
 *  @name     Fundamental Commands
 */
#define OLED_SETLOWCOLUMN			0x00
#define OLED_SETHIGHCOLUMN		    0x10
#define OLED_MEMORYMODE				0x20
#define OLED_SETCOLUMNADDRESS		0x21
#define OLED_SETPAGEADDRESS			0x22
#define OLED_SETSTARTLINE			0x40
#define OLED_SETCONTRAST			0x81
#define OLED_SETBRIHTNESS			0x82
#define OLED_LOOKUP					0x91
#define OLED_BANKCOLOR_PG0			0x92
#define OLED_BANKCOLOR_PG1			0x93
#define OLED_SEGREMAP_NORM			0xA0
#define OLED_SEGREMAP_REV			0xA1
#define OLED_DISPLAYALLON_RESUME	0xA4
#define OLED_DISPLAYALLON			0xA5
#define OLED_SETDISPLAY_NORM		0xA6
#define OLED_SETDISPLAY_INVERT		0xA7
#define OLED_SETMULTIPLEX			0xA8
#define OLED_DIMMODE				0xAB
#define OLED_MASTER_CONF			0xAD
#define OLED_DISPLAYON_DIM			0xAC
#define OLED_DISPLAYOFF				0xAE
#define OLED_DISPLAYON				0xAF
#define OLED_SET_PAGE_START			0xB0
#define OLED_COMSCANDIR_NORM		0xC0
#define OLED_COMSCANDIR_REMAP		0xC8
#define OLED_SETDISPLAYOFFSET	    0xD3
#define OLED_SETDISPLAYCLOCKDIV		0xD5
#define OLED_SETAREACOLORMODE		0xD8
#define OLED_SETPRECHARGE			0xD9
#define OLED_SETCOMPINS				0xDA
#define OLED_SETVCOMDESEL			0xDB
#define OLED_ENTER_RDMODIFYWR		0xE0
#define OLED_NOP					0xE3
#define OLED_EXIT_RDMODIFYWR		0xEE



/**
 @brief    Display character to the screen on current cursor position
 @param    character to write                                        
 @return   none
*/
void oled_putc(char x);


/**
 @brief    Display character array / string on the screen on current cursor position
 @param    string to write                                       
 @return   none
*/
void oled_puts(const char * s);


/**
 @brief    Display bitmap image on the screen starting at beginning of the screen
 @param    bitmap character array                                        
 @return   none
*/
void oled_bitmap(char bmp[]);


/**
 @brief    Set cursor to specified position
 @param    x for horizontal and y vertical position, starting at 0,0                            
 @return   none
*/
void oled_gotoxy(uint8_t x,uint8_t y);


/**
 @brief    Set cursor to specified line number
 @param    x for horizontal and y vertical line position, starting at 0                                        
 @return   none
*/
void oled_gotoline (uint8_t y);


/**
 @brief    clear a individual pixel on the display
 @param    x for horizontal and y vertical position, starting at 0,0                                       
 @return   none
*/
void oled_clearpixel(unsigned char x, unsigned char y);


/**
 @brief    Clear the display
 @param    void                                        
 @return   none
*/
void oled_clear (void);


/**
 @brief    Write 8-bit data to the display SSD1305 controller
 @param    data to be send                                        
 @return   none
*/
void oled_wr_data(uint8_t data);


/**
 @brief    write 5-bit command tot the display SSD1305 controller
 @param    command to be send                                        
 @return   none
*/
void oled_wr_command(uint8_t command);


/**
 @brief    Read the data of current cursor position
 @param    void                                        
 @return   none
*/
uint8_t oled_rd_data(void);


/**
 @brief    Initialize the display ports 
 @param    void                                        
 @return   none
*/
void oled_init_ports(void);


/**
 @brief    Initialize the display
 @param    void                                        
 @return   none
*/
void oled_init(void);






#endif /* OLED_H_ */