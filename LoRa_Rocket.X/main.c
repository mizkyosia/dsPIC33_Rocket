/*
 * File:   TX_LoRa.c
 * Authors: BRS & JMC
 *
 * Created on 19 May 2017
 */

#define USE_AND_MASKS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <xc.h>
#include "general.h"
#include "main.h"
#include "uart.h"
#include "spi.h"
#include "lora.h"
#include "gpio.h"
#include "payload.h"
#include "BMP280.h"
#include "LSM6DSL.h"


// =============================================================================
// CONFIGURATIONS 
// =============================================================================

// FBS
#pragma config BWRP = WRPROTECT_OFF     // Boot Segment Write Protect (Boot Segment may be written)
#pragma config BSS = NO_FLASH           // Boot Segment Program Flash Code Protection (No Boot program Flash segment)
#pragma config RBS = NO_RAM             // Boot Segment RAM Protection (No Boot RAM)

// FSS
#pragma config SWRP = WRPROTECT_OFF     // Secure Segment Program Write Protect (Secure segment may be written)
#pragma config SSS = NO_FLASH           // Secure Segment Program Flash Code Protection (No Secure Segment)
#pragma config RSS = NO_RAM             // Secure Segment Data RAM Protection (No Secure RAM)

// FGS
#pragma config GWRP = OFF               // General Code Segment Write Protect (User program memory is not write-protected)
#pragma config GSS = OFF                // General Segment Code Protection (User program memory is not code-protected)

// FOSCSEL
#pragma config FNOSC = FRC           // Oscillator Mode (Internal Fast RC (FRC) w/ PLL)
#pragma config IESO = OFF                // Internal External Switch Over Mode (Start-up device with FRC, then automatically switch to user-selected oscillator source when ready)

// FOSC
#pragma config POSCMD = NONE            // Primary Oscillator Source (Primary Oscillator Disabled)
#pragma config OSCIOFNC = ON            // OSC2 Pin Function (OSC2 pin has digital I/O function)
#pragma config IOL1WAY = ON             // Peripheral Pin Select Configuration (Allow Only One Re-configuration)
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor (Both Clock Switching and Fail-Safe Clock Monitor are disabled)

// FWDT
#pragma config WDTPOST = PS32768        // Watchdog Timer Postscaler (1:32,768)
#pragma config WDTPRE = PR128           // WDT Prescaler (1:128)
#pragma config WINDIS = OFF             // Watchdog Timer Window (Watchdog Timer in Non-Window mode)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (Watchdog timer enabled/disabled by user software)

// FPOR
#pragma config FPWRT = PWR128           // POR Timer Value (128ms)
#pragma config ALTI2C = OFF             // Alternate I2C  pins (I2C mapped to SDA1/SCL1 pins)

// FICD
#pragma config ICS = PGD1               // Comm Channel Select (Communicate on PGC1/EMUC1 and PGD1/EMUD1)
#pragma config JTAGEN = OFF             // JTAG Port Enable (JTAG is Disabled)




uint8_t reg_val, pout;
int16_t i, rssi;
uint8_t txBuffer[256];

uint8_t version;
uint8_t test;

Payload payload;

int main(int argc, char** argv) {

    CLKDIVbits.DOZEN = 0;
    
    System_Init();
    
    //UARTWriteStrLn("System initialization...");
    
    //LoRa_AntennaTX(); // connect antenna to module output

    // put module in LoRa mode (see SX1272 datasheet page 107)
    //UARTWriteStrLn("set mode to LoRa standby");

    //LoRa_WriteRegister(REG_OP_MODE, FSK_SLEEP_MODE); // SLEEP mode required first to change bit n�7
    //LoRa_WriteRegister(REG_OP_MODE, LORA_SLEEP_MODE); // switch from FSK mode to LoRa mode
    //LoRa_WriteRegister(REG_OP_MODE, LORA_STANDBY_MODE); // STANDBY mode required fot FIFO loading
    //__delay_ms(100);
    //GetMode();

    UARTWriteStrLn("initialized module ");

    //LoRa_Init();
    
    BMP280_Init();
    LSM6DSL_Init();

    // version = LoRa_ReadRegister(REG_VERSION);
    
    UARTWriteStrLn("Reading chip IDs & versions...");
    
    //uint8_t capteur_id = I2C1_Read(BME280_ADDR, BME280_REG_CHIP_ID);
    
    //UARTWriteStr("LoRa version : "); UARTWriteByteHex(version); UARTWriteStrLn(" ");
    //UARTWriteStr("BME280 ID : "); UARTWriteByteHex(capteur_id); UARTWriteStrLn(" ");

    loop();

}

void test_LoRa(){
    UARTWriteStrLn("Sending Payload through LoRa module :");

        Payload p = {
            .acc =
            {
                .x = 0,
                .y = 1,
                .z = 2
            },
            .gyr =
            {
                .x = 3,
                .y = 4,
                .z = 5
            },
            .pos =
            {
                .x = 6,
                .y = 7,
                .z = 8
            },
            .deltaTime = 9,
            .pressure = 10,
            .battery = 11,
            .stage = 12
        };

        LoRa_SendPayload(p);
        UARTWriteStrLn("Payload sent. Waiting for next transmission...");
}

void testBME(void){    
    //uint8_t temp_lsb = I2C1_Read(BME280_ADDR, 0xFB);
    BMP280_FetchData(&payload);
}

void testLSM6DSL(void){
    LSM6DSL_FetchData(&payload);
    
    UARTWriteStr("Acceleration X : "); UARTWriteU16(payload.acc.x); UARTWriteStrLn(" ");
}

void loop(void) {
    while (1) {
        BMP280_FetchData(&payload);
        LSM6DSL_FetchData(&payload);
        UARTWritePayloadDebug(&payload);
        __delay_ms(500);
    }
}