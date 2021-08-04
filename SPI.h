#ifndef XC_SPI_H
#define	XC_SPI_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <pic16f887.h>
typedef enum 
{
    SPI_MASTER_OSC_DIV4  = 0b00100000,      //frecOsc/4
    SPI_MASTER_OSC_DIV16 = 0b00100001,      //frecOsc/16
    SPI_MASTER_OSC_DIV64 = 0b00100010,      //frecOsc/64
    SPI_MASTER_TMR2      = 0b00100011,      //Dirigido por el timer 2
    SPI_SLAVE_SS_EN      = 0b00100100,      //Selección de esclavo
    SPI_SLAVE_SS_DIS     = 0b00100101       //Solo hay un esclavo
}Spi_Type;

typedef enum
{
    SPI_DATA_SAMPLE_MIDDLE   = 0b00000000,
    SPI_DATA_SAMPLE_END      = 0b10000000
}Spi_Data_Sample;

typedef enum
{
    SPI_CLOCK_IDLE_HIGH  = 0b00010000,        //transferencia en flanco positivo
    SPI_CLOCK_IDLE_LOW   = 0b00000000         //transferencia en flanco negativo      
}Spi_Clock_Idle;

typedef enum
{
    SPI_IDLE_2_ACTIVE    = 0b00000000,
    SPI_ACTIVE_2_IDLE    = 0b01000000
}Spi_Transmit_Edge;


void spiInit(Spi_Type, Spi_Data_Sample, Spi_Clock_Idle, Spi_Transmit_Edge);
void spiWrite(char);
unsigned spiDataReady();
char spiRead();

#endif	/* SPI_H */

