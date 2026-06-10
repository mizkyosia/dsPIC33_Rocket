/* 
 * File:   uart.h
 * Author: LL
 *
 * Created on 27 June 2026
 */

#ifndef _UART_H
#define	_UART_H

#include "general.h"


void UART1_Init(unsigned long baudRate);           // init UART with specified baud rate
uint8_t UARTReadByte(void);                 // read a byte from UART
void UARTWriteByte(uint8_t data);           // send a byte to UART
void UARTWriteStr(char *string);               // output strings to UART
void UARTWriteStrLn(char *string);             // output strings to UART
void UARTWriteByteHex(uint8_t data);        // send the hexadecimal value of a byte so that it is readable in a terminal window
void UARTWriteByteDec(uint8_t data);        // send the decimal value of a byte so that it is readable in a terminal window

#endif	/* _UART_H */

