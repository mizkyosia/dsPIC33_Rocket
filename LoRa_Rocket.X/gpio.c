#include "gpio.h"
#include "uart.h"
#include "lora.h"
#include "spi.h"
#include "i2c.h"
#include <xc.h>

void System_Init(void)
{
    // All pins digital
    AD1PCFGL = 0xFFFF;
    
    // PPS : Peripheral Pin Select
    RPINR20bits.SDI1R = 14;   // SDI1    : input
    RPOR7bits.RP15R = 7;      // SDO1    : output
    RPOR6bits.RP13R = 8;      // SCK1OUT : output
    
    TRISBbits.TRISB15 = OUTP;   // SDO1 output
    TRISBbits.TRISB13 = OUTP;   // SCK1 output
    TRISBbits.TRISB14 = INP;   // SDI1 input
    
    // Initialize UART
    UART1_Init(115200);
    
    // Configure RF control pins
    // LoRa_InitPins();

    // Initialize SPI
    SPI1_Init();
    
    // Initialize I2C
    I2C1_Init();

    // Reset SX1272
    // LoRa_Reset();
}
