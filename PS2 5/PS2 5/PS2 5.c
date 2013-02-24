//http://www.larsivar.com/cp/comments.php?y=10&m=09&entry=entry100923-223328

#define DDR_SPI DDRB
#define DD_MOSI DDB2
#define DD_SCK DDB1
#define DD_SS DDB0
#define DD_MISO DDB3

# define F_CPU 16000000UL

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <inttypes.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include "interrupt.h"
#include "delay.h"
//#include "uart.h"
//#include "PS2_Library.h"

#include "larslib.h"
#include "spi.h"
#include "uart.h"
#include "ps2.h"
// #include "servo.h"


void test_pins(){
	PORTB &= ~(1<<DD_MOSI); 
	PORTB &= ~(1<<DD_SCK);
	PORTB &= ~(1<<DD_SS);
	PORTB &= ~(1<<DD_MISO); 
}

int main()
{
	SPI_init();
	uart_init();
	ps2_init();
	sei();

	while(1)
	{
		//ps2_init();
		// _delay_ms(100);
		//test_pins();
		//PORTB &= ~(1<<DD_SS);
		ps2_poll(ps2.pressure[5],PS2_MOTOR_OFF);
		// ps2.pressure[5] = ps2.pressure[5] + 0x30;
		// char a_out = 'a';
		// uint8_t a_out = 'A';
		
		// uart_putc(ps2.rx);
// 		for (int x = 0; x < 12; x++)
// 		{
// 			//ps2.pressure[x] = 0x30;
// 			uart_putc((ps2.pressure[x]));	
// 		}
		uart_putc(ps2.rx);
		uart_putc(ps2.ry);
		uart_putc(ps2.lx);
		uart_putc(ps2.ly);		
		uart_putc(rx_buffer[0]);
		uart_putc(rx_buffer[1]);
		uart_putc(rx_buffer[2]);	
		uart_putc(rx_buffer[3]);
		uart_putc(rx_buffer[4]);					
		uart_putc(0x69);
		uart_putc(0x0D);
		
		//PORTB |= (1<<DD_SS); // Attention off
	}

	return 0;
}
