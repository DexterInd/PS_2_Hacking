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

int main()
{
	SPI_init();
	uart_init();
	ps2_init();
	sei();

	while(1)
	{
		_delay_ms(10);
		ps2_poll(ps2.pressure[5],PS2_MOTOR_OFF);
		// ps2.pressure[5] = ps2.pressure[5] + 0x30;
		uart_putc((ps2.pressure[5]));
	}

	return 0;
}
