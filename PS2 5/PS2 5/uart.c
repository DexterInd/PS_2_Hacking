/* 
 * UART on Atmel MCU
 * 
 * V2.2  
 * 
 * By: Lars Ivar Miljeteig, September 2010
 * Platform: AVR ATmega168 (avr-gcc)
 * Dependencies: avr/io delay.h stdint.h
 *
 * Reading and writing data by UART
 */

#include "larslib.h"
#include "uart.h"

uint8_t uart_putc(uint8_t data)
{
// 	// Wait for empty transmit buffer
// 	while(!(UCSR0A & (1<<UDRE0) ));
// 	UDR0 = data;
	// wait for empty receive buffer
	while ((UCSR1A & (1<<UDRE1))==0);
	// send
	UDR1 = data;

	return 1;
}

unsigned char uart_getc()
{

    // Wait for data to be received 
    //while ( !(UCSR0A & (1<<RXC0)) );
	while (!(UCSR1A & (1<<RXC1)));

    // Get and return received data from buffer
	char x = UDR1;
	return x;

}

void uart_init()
{
	
	uint16_t ubrr = MYUBRR;

	// Set uart speed 
	UBRR1H = (uint8_t)(ubrr>>8);
	UBRR1L = (uint8_t)ubrr;
	
// 	// Set frame format: 8data, 1stop bit
// 	UCSR0C = 3<<UCSZ00;
// 
// 	// Enable transmitting circuitry 
// 	UCSR0B = (1<<TXEN0)|(1<<RXEN0);//|(1<<RXCIE0);
	UCSR1B = (1<<RXEN1)|(1<<TXEN1);			// enable uart RX and TX
	UCSR1C = (1<<UCSZ11)|(1<<UCSZ10);		// set 8N1 frame format

}

void uart_writestring(char *string)
{
    uint8_t i = 0;
    while(string[i]!=0) uart_putc(string[i++]);

}

void uart_writechar(char c)
{
    uart_putc(c);
}
