
/* 
 * SPI on Atmel ATmega32U4
 * 
 */

#include "larslib.h"
#include "spi.h"

#define DDR_SPI DDRB
#define DD_MOSI PINB2	// PinB2 is MOSI
#define DD_SCK PINB1	// PinB1 is the Clock
#define DD_SS PINB4		// PinB4 is the Slave Select
#define DD_MISO PINB3	// PinB3 is the MISO

// Initializes the SPI interface.
// Sets clock to 125 kHz.  
// Sets above pins to functionality.  

void SPI_init()
{
	
    // Set MOSI and SCK as output, others as input
	DDR_SPI = (1<<DD_MOSI)|(1<<DD_SCK)|(1<<DD_SS);						// P. 180 of the Documentation.
    //SPI_DDR = (1<<PB3)|(1<<PB5)|(1<<PB2);
 
    // Enable SPI, Master, set clock rate fck/64, high-when-idle (CPOL), sample on trail (CPHA), LSB (DORD)
    SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR1)|(0<<SPR0)|(1<<CPOL)|(1<<DORD)|(1<<CPHA);  // SPI Control Register --> P. 182 // 125 khz
	//SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR1)|(1<<CPOL)|(1<<DORD)|(1<<CPHA);  // SPI Control Register --> P. 182 // 250 khz
			
}

// 
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

