/*
 * File:   spi.c
 * Authors: LAL & JMC
 *
 * Created(LaL) on 9 avril 2015
 * Modified (JMC) on 18 mai 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <xc.h>
#include "general.h"
#include "spi.h"

void SPI1_Init(void)
{    
    SPI1STATbits.SPIEN = 0;
    
    // Clock & SDO enabled
    SPI1CON1bits.DISSCK = 0;
    SPI1CON1bits.DISSDO = 0;
    
    // Byte-wide communication (8bits)
    SPI1CON1bits.MODE16 = 0;
    
    // Manual Chip Select
    SPI1CON1bits.SSEN = 0;

    // Master mode
    SPI1CON1bits.MSTEN = 1;

    // Clock polarity/phase
    SPI1CON1bits.CKP = 0;
    SPI1CON1bits.CKE = 1;

    // SPI clock prescaler
    SPI1CON1bits.PPRE = 0b11;
    SPI1CON1bits.SPRE = 0b110;

    // 8-bit mode
    SPI1CON1bits.MODE16 = 0;

    SPI1STATbits.SPIEN = 1;
    
    
}


uint8_t SPI1_Transfer(uint8_t data)
{
    SPI1BUF = data;

    while(!SPI1STATbits.SPIRBF);

    return SPI1BUF;
}