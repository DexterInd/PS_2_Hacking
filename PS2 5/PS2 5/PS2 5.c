// ATmega32U4 Controlling a PS2 Controller
/* Assumes that fuses are set to:
	- Extended	FB
	- High		D8
	- Low		DE
	
*/
/*
// Helpful links:
// http://www.larsivar.com/cp/comments.php?y=10&m=09&entry=entry100923-223328
// http://store.curiousinventor.com/guides/PS2  // Breakout pictures, byte by byte analysis of data
// 

*/
# define F_CPU 16000000		// NOTE: This is actually redefined in the Project Box (PS25 --> Toolchain --> AVR/GNU C Compiler --> Symbols) with the definition "F_CPU=16000000UL"
							// Defining it there is the secret to getting it defined for the entire project.  
#define DDR_SPI DDRB
#define DD_MOSI DDB2
#define DD_SCK DDB1
#define DD_SS DDB0
#define DD_MISO DDB3


#include <avr/wdt.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <inttypes.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include "interrupt.h"
#include "delay.h"

#include "larslib.h"
#include "spi.h"
#include "uart.h"
#include "ps2.h"

// Test Functions to see where your pins are located.
void test_pins(){
	PORTB &= ~(1<<DD_MOSI); 
	PORTB &= ~(1<<DD_SCK);
	PORTB &= ~(1<<DD_SS);
	PORTB &= ~(1<<DD_MISO); 
}

uint8_t select_button = 0x00;	// Hold select button state
uint8_t start_button =	0x00;	// Hold start button state

uint8_t UP_button =		0x00;	// Up button of the start pad
uint8_t DOWN_button =	0x00;	
uint8_t LEFT_button =	0x00;	
uint8_t RIGHT_button =	0x00;

uint8_t L3 =			0x00;	// Holds state of Left Button 3
uint8_t R3 =			0x00;	
uint8_t L2 =			0x00;
uint8_t R2 =			0x00;
uint8_t L1 =			0x00;
uint8_t R1 =			0x00;

uint8_t TRIANGLE =		0x00;	// Holds states of the Shapes pad
uint8_t O_Button =		0x00;
uint8_t X_Button =		0x00;
uint8_t SQUARE =		0x00;


// Set Button Values
// This takes the values returned, and sets the values to what has that been returned.
// These are the digital values.
void set_button_values(){
	
	// Byte 4 contains select, start, L3 and R3, as well as the button arrows
	// if (rx_buffer[3] & (1<<0)) { select_button == 0x00; } else { select_button == 0x01;}	
	select_button = (!(rx_buffer[3] & (1<<0)));	
				
	L3 = (!(rx_buffer[3] & (1<<1))); // { L3== true; } else { L3 == false;}
	R3 = (!(rx_buffer[3] & (1<<2))); // { R3 == true; } else { R3 == false;}			
	start_button = (!(rx_buffer[3] & (1<<3))); // { start_button == true; } else { start_button == false;}			
	UP_button = (!(rx_buffer[3] & (1<<4)));		// { UP_button == true; } else { UP_button == false;}													
	RIGHT_button  = (!(rx_buffer[3] & (1<<5)));	// { RIGHT_button == true; } else { RIGHT_button == false;}			
	DOWN_button = (!(rx_buffer[3] & (1<<6))); //{ DOWN_button == true; } else { DOWN_button == false;}							
	LEFT_button = (!(rx_buffer[3] & (1<<7))); //{ LEFT_button == true; } else { LEFT_button == false;}
			
	// Byte 5 contains the Left and right buttons, as well as the shapes buttons
	L2 = (!(rx_buffer[4] & (1<<0))); // { L2 == true; } else { L2 == false;}
	R2 = (!(rx_buffer[4] & (1<<1))); // { R2 == true; } else { R2 == false;}
	L1 = (!(rx_buffer[4] & (1<<2))); // { L1 == true; } else { L1 == false;}			
	R1 = (!(rx_buffer[4] & (1<<3))); // { R1 == true; } else { R1 == false;}			
	TRIANGLE = (!(rx_buffer[4] & (1<<4))); // { TRIANGLE == true; } else { TRIANGLE == false;}													
	O_Button = (!(rx_buffer[4] & (1<<5))); // { O_Button == true; } else { O_Button == false;}			
	X_Button = (!(rx_buffer[4] & (1<<6))); // { X_Button == true; } else { X_Button == false;}							
	SQUARE = (!(rx_buffer[4] & (1<<7))); // { SQUARE == true; } else { SQUARE == false;}
	
}

// Function prints out when a key is hit.
void test_button_analysis_and_output(){
	refresh_button_values();
	if (select_button)	{uart_putc('S'); uart_putc('E'); uart_putc(0x0D);}
	if (L3)				{uart_putc('L'); uart_putc('3'); uart_putc(0x0D);}
	if (R3)				{uart_putc('R'); uart_putc('3'); uart_putc(0x0D);}	
	if (start_button)	{uart_putc('S'); uart_putc('T'); uart_putc(0x0D);}			
	if (UP_button)		{uart_putc('U'); uart_putc('P'); uart_putc(0x0D);}
	if (RIGHT_button)	{uart_putc('R'); uart_putc(' '); uart_putc(0x0D);}
	if (DOWN_button)	{uart_putc('D'); uart_putc(' '); uart_putc(0x0D);}
	if (LEFT_button)	{uart_putc('L'); uart_putc(' '); uart_putc(0x0D);}
					
	if (L2)				{uart_putc('L'); uart_putc('2'); uart_putc(0x0D);}
	if (R2)				{uart_putc('R'); uart_putc('2'); uart_putc(0x0D);}
	if (L1)				{uart_putc('L'); uart_putc('1'); uart_putc(0x0D);}
	if (R1)				{uart_putc('R'); uart_putc('1'); uart_putc(0x0D);}
	if (TRIANGLE)		{uart_putc('T'); uart_putc(' '); uart_putc(0x0D);}
	if (O_Button)		{uart_putc('O'); uart_putc(' '); uart_putc(0x0D);}
	if (X_Button)		{uart_putc('X'); uart_putc(' '); uart_putc(0x0D);}
	if (SQUARE)			{uart_putc('S'); uart_putc('Q'); uart_putc(0x0D);}
}

// Refresh Button Values Function
// Refreshes the button values, sets the booleans
void refresh_button_values(){
	// First, refresh the values of the buttons, poll the controller.
	ps2_poll(ps2.pressure[5],PS2_MOTOR_OFF);
		
	// Now, update the controllers
	set_button_values();		
}

// Tests and prints on the UART line the raw bytes of feedback.
void test_and_print_raw_data(){
	// Poll the controller
	ps2_poll(ps2.pressure[5],PS2_MOTOR_OFF);
		
	// This is where the pressure of the buttons should be output.
	// Does seem to work with our wired controller.
	// 		for (int x = 0; x < 12; x++)
	// 		{
		// 			uart_putc((ps2.pressure[x]));
	// 		}
	
	// Outputs the first few bytes returned.
	uart_putc(rx_buffer[0]);
	uart_putc(rx_buffer[1]);
	uart_putc(rx_buffer[2]);
	uart_putc(rx_buffer[3]);
	uart_putc(rx_buffer[4]);
	
	// Outputs analog values of the two joysticks.
	uart_putc(ps2.rx);
	uart_putc(ps2.ry);
	uart_putc(ps2.lx);
	uart_putc(ps2.ly);
	
	uart_putc(0x0D);
}

void led_init(){
	/*PF5 -> LED_Analog
	PF4 -> LED_PS2
	PF1 -> LED_Data
	PF0 -> LED_Power
	*/
	DDRF = 0xFF;	// Set All DDRF to outputs.
}

void led_on(){
	/*PF5 -> LED_Analog
	PF4 -> LED_PS2
	PF1 -> LED_Data
	PF0 -> LED_Power
	*/

// 	PORTF |=(1<<5);	// LED Analog On
// 	_delay_ms(35);
// 	PORTF |=(1<<4);	// LED PS2 On
// 	_delay_ms(35);	
// 	PORTF |=(1<<1);	// LED Data On
// 	_delay_ms(35);	
// 	PORTF |=(1<<0);	// LED Power On
// 	_delay_ms(35);
// 	
// 	PORTF &= ~(1 << 5);	
// 	_delay_ms(35);
// 	PORTF &= ~(1 << 4);
// 	_delay_ms(35);
// 	PORTF &= ~(1 << 1);
// 	_delay_ms(35);		
// 	PORTF &= ~(1 << 0);
// 	_delay_ms(35);	
	PORTF ^= 0xFF;
	_delay_ms(35);
}

int main()
{
	uint8_t mcusr_at_start = MCUSR;
	MCUSR = 0;	// enable WDT disable
	wdt_disable();
		
	_delay_s(1);
	PORTB |= (1<<DD_MOSI);
	SPI_init();
	uart_init();
	ps2_init();
	// led_init();
	sei();

	test_and_print_raw_data();
	_delay_s(1);
	while(1)
	{
		test_button_analysis_and_output();	
		uart_putc(rx_buffer[3]);	
		uart_putc(rx_buffer[4]);
		uart_putc(0x0D);
		_delay_s(1);
		// led_on();
	}

	return 0;
}
