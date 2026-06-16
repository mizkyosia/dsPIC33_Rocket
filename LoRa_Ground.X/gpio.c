#include "gpio.h"
#include "uart.h"
#include "lora.h"
#include "spi.h"
#include <xc.h>

void System_Init(void)
{
    // PLL Configuration
    // CLKDIVbits.PLLPRE = 0;      // N1 = 2 (Diviseur d'entrée)
    // PLLFBD = 41;                // M = 43 (Multiplicateur central)
    // CLKDIVbits.PLLPOST = 0;     // N2 = 2 (Diviseur de sortie)
    
    // Waiting for PLL lock 
    //while (OSCCONbits.LOCK != 1);
    
    // All pins digital
    AD1PCFGL = 0xFFFF;
    
    // PPS : Peripheral Pin Select
    RPINR20bits.SDI1R = 14;   // SDI1    : input
    RPOR6bits.RP13R = 7;      // SDO1    : output
    RPOR7bits.RP15R = 8;      // SCK1OUT : output
    
    TRISBbits.TRISB13 = OUTP;   // SDO1 output
    TRISBbits.TRISB15 = OUTP;   // SCK1 output
    TRISBbits.TRISB14 = INP;   // SDI1 input
    
    // Initialize UART
    UART1_Init(115200);
    
    // Configure RF control pins
    LoRa_InitPins();

    // Initialize SPI
    SPI1_Init();

    // Reset SX1272
    LoRa_Reset();
}
