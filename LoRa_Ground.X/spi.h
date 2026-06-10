/*
 * File:   spi.h
 * Authors: LAL & JMC
 *
 * Created (LAL) on 9 avril 2015
 * Modified (JMC) on 18 mai 2017
 */

#ifndef _SPI_H
#define	_SPI_H

#include "general.h"
#include <stdint.h>             // with this inclusion, the XC compiler will recognize standard types such as uint8_t or int16_t 
                                // (so, their definition in "general.h" is useless)


void SPI1_Init(void);                                                         // init SPI in master mode
uint8_t SPI1_Transfer (uint8_t data_out);                                        // send a byte and receive the data

#endif	/* _SPI_H */

