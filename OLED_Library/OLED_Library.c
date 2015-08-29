/*
 * OLED_Library.c
 *
 * Created: 30-11-2012 20:10:19
 *  Author: Vito
 */ 


#include <avr/io.h>
#include <stdio.h>
#include "OLED/oled.h"
#define F_CPU 2000000UL
#include <util/delay.h>

int main(void)
{
	
	oled_init();
	oled_clear();
	printf("TEST\n");
	printf("%d", 42);
	
	while(1)
	{
		// code
	}
}