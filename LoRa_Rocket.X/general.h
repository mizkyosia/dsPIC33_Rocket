/*
 * File:   general.h
 * Author: lal
 *
 * Created on 13 mars 2015, 14:48
 */

#include <xc.h>
#include <stdint.h>

#ifndef _GENERAL_H
#define _GENERAL_H

// Booleans
#define TRUE            1           // allow TRUE to equal 1
#define FALSE           0           // allow FALSE to equal 0
#define forever         while(TRUE) // endless loop

// Following definitions are equivalent (depending on the context), but SET or CLEAR are sufficient
#define SET             1           // allow SET to equal 1
#define CLEAR           0           // allow CLEAR to equal 0

#define ENABLE          SET         // allow ENABLE to equal 1
#define DISABLE         CLEAR       // allow DISABLE to equal 0

#define READ            SET         // allow READ to equal 1
#define WRITE           CLEAR       // allow WRITE to equal 0

#define ON              SET         // allow ON to equal 1
#define OFF             CLEAR       // allow OFF to equal 0

#define INP             SET         // PORT bit is in input mode
#define OUTP            CLEAR       // PORT bit is in output mode

#define OUTP_HIGH	SET         // high level
#define OUTP_LOW	CLEAR       // low level

#define FCY 3685000UL  // Base frequency without PLL = Fosc / 2

#include <libpic30.h>

#endif /* _GENERAL_H */
