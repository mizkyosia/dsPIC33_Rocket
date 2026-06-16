/*
 * File:   uart.c
 * Author: mizkyosia
 *
 * Created on 6 Juin 2026
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <xc.h>
#include "general.h"
#include "uart.h"

void UART1_Init(unsigned long baudrate)
{
    // Disable UART
    U1MODEbits.UARTEN = 0;
    
    // Unlock pin registers
    __builtin_write_OSCCONL(OSCCON & 0xBF);
    
    // Pin setup
    RPOR2bits.RP4R = 3; // UART1 TX assigned to RP4
    RPINR18bits.U1RXR = 5;  // RP5 assigned to UART1 RX
    
    TRISBbits.TRISB4 = OUTP;
    TRISBbits.TRISB5 = INP;
    
    // Lock pin registers
    __builtin_write_OSCCONL(OSCCON | 0x40);

    // UART1 Configuration
    U1MODEbits.STSEL = 0;   // 1 Stop bit
    U1MODEbits.PDSEL = 0;   // 8-bit data, no parity
    U1MODEbits.ABAUD = 0;   // Auto-Baud disabled
    
    // ENABLE HIGH SPEED MODE FOR BETTER ACCURACY AT 115200 BAUD
    U1MODEbits.BRGH = 1;    // High Speed mode
    
    // Baud Rate Generator Calculation for 115200 baud with BRGH=1
    // U1BRG = (39613750 / (4 * 115200)) - 1 = 84.96 -> 85
    U1BRG = 47;

    // Enable UART and TX
    U1MODEbits.UARTEN = 1;  // Enable UART1
    U1STAbits.UTXEN = 1;    // Enable Transmit
}

void UARTWriteByte(uint8_t data) {

    while (U1STAbits.TRMT == CLEAR);           // wait for transmission idle

    U1TXREG = data;
}

uint8_t UARTReadByte(void)
{
    return(U1RXREG);
}

void UARTWriteStr(char *string)
{
  uint8_t i = 0;

  while (string[i])
    UARTWriteByte(string[i++]);
}

void UARTWriteStrLn(char *string)
{
  uint8_t i = 0;

  while (string[i])
    UARTWriteByte(string[i++]);
  
  UARTWriteByte(0x0D);      // write Carriage Return
  UARTWriteByte(0x0A);      // write Line Feed (New Line)
}

void UARTWriteByteHex(uint8_t data)
{
    char *hexa = "0123456789ABCDEF";
    
    UARTWriteByte(hexa[data / 16]);     // write ASCII value of hexadecimal high nibble
    UARTWriteByte(hexa[data % 16]);     // write ASCII value of hexadecimal low nibble
}

void UARTWriteU8(uint8_t data)
{

    UARTWriteByte((data / 100) + '0');              // write ASCII value of hundreds digit
    UARTWriteByte(((data % 100) / 10) + '0');       // write ASCII value of tenths digit
    UARTWriteByte((data % 10) + '0');       // write ASCII value of tenths digit
}

void UARTWriteU16(uint16_t data)
{

    UARTWriteByte((data / 10000) + '0');            
    UARTWriteByte(((data % 10000) / 1000) + '0');   
    UARTWriteByte(((data % 1000) / 100) + '0');     
    UARTWriteByte(((data % 100) / 10) + '0');       
    UARTWriteByte((data % 10) + '0');       
}

void UARTWriteFloat(float value)
{
    char buffer[20];

    int integerPart;
    int fractionalPart;

    if(value < 0)
    {
        UARTWriteByte('-');
        value = -value;
    }

    integerPart = (int)value;

    fractionalPart = (int)((value - integerPart) * 1000.0f);

    sprintf(buffer, "%d.%03d", integerPart, fractionalPart);

    UARTWriteStr(buffer);
}