
/* 
 * SPI on Atmel MCU
 * 
 * V1.0  
 * 
 * By: Lars Ivar Miljeteig, September 2010
 * Platform: AVR ATmega168 (avr-gcc)
 * Dependencies: avr/io delay.h stdint.h larslib.h
 *
 * Reading data from SPI as master
 */

#include "larslib.h"
#include "spi.h"

// extern DDR_SPI;
// extern DD_MOSI;
// extern DD_SCK;
// extern DD_SS;
// extern DD_MISO;

// #define SPI_PORT PORTB
// #define SPI_DDR  DDRB
// #define SPI_CS   PB2


#define DDR_SPI DDRB
#define DD_MOSI PINB2
#define DD_SCK PINB1
#define DD_SS PINB4
#define DD_MISO PINB3

void SPI_init()
{

//     // Set MOSI and SCK as output, others as input
	DDR_SPI = (1<<DD_MOSI)|(1<<DD_SCK)|(1<<DD_SS);						// P. 180 of the Documentation.
     //SPI_DDR = (1<<PB3)|(1<<PB5)|(1<<PB2);
// 
     // Enable SPI, Master, set clock rate fck/64, high-when-idle (CPOL), sample on trail (CPHA), LSB (DORD)
    SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR1)|(0<<SPR0)|(1<<CPOL)|(1<<DORD)|(1<<CPHA);  // SPI Control Register --> P. 182 // 125 khz
	//SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR1)|(1<<CPOL)|(1<<DORD)|(1<<CPHA);  // SPI Control Register --> P. 182 // 250 khz

// 	/* Set MOSI and SCK output, all others input */
// 	DDR_SPI = (1<<DD_MOSI)|(1<<DD_SCK)|(1<<DD_SS);						// P. 180 of the Documentation.
// 	/* Enable SPI, Master, set clock rate fck/16 */
// 	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR1);			
}

uint8_t SPI_MTx(uint8_t dataout)
{
    uint8_t datain;

    // Start transmission (MOSI)
    SPDR = dataout;

    // Wait for transmission complete
    while(!(SPSR & (1<<SPIF)));

    // wait and get return value
    _delay_us(30);  
    datain = SPDR;

    return datain;
}

