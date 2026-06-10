/*
 * File:   general.h
 * Author: lal
 *
 * Created on 13 mars 2015, 14:48
 */

#include <xc.h>

#ifndef _GENERAL_H
#define _GENERAL_H

extern uint8_t lora_mode;

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

#define PAYLOAD_LENGTH 7                             // for transmission: number of bytes to transmit


// =================== PINS CONFIG ===================
// ===== LoRa module =====

// ===== UART =====
#define UART_TX         U1TXREG     // pin RC6 is used for UART transmission
#define UART_RX         U1RXREG     // pin RC7 is used for UART reception

// New typedefs
typedef unsigned char	uint8_t;    // uint8_t means byte (8-bit)
typedef unsigned int	UINT16_T;   // UINT16_t means word(16-bit)
typedef unsigned char	BOOL;       // BOOL means byte (8-bit)
typedef char            BYTE;
typedef char            INT8_T;     // INT8_T means 2's complement (1 sign bit + 7-bit)
typedef int             INT16_T;    // INT16_T means 2's complement (1 sign bit + 15-bit)

typedef struct {
    uint16_t x;          // x coordinate
    uint16_t y;          // y coordinate
    uint16_t z;          // z coordinate
} Vec3_f16_t;          // 3-dimensional vector, 16-bit blocks

typedef struct {
    float x;          // x coordinate
    float y;          // y coordinate
    float z;          // z coordinate
} Vec3_f32_t;

typedef struct {
    Vec3_f16_t acc;        // acceleration
    Vec3_f16_t gyr;        // angular velocity
    Vec3_f32_t pos;        // position
    uint16_t temp;    // temperature (in �C)
    uint16_t pres;     // pressure (in hPa)
    uint16_t time;     // time since launch (in ms)
    char battery;     // battery level (%)
} PAYLOAD;       // data from the Inertial Measurement Unit (IMU)

#endif /* _GENERAL_H */
