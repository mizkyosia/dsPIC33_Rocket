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
#include "uart.h"
#include "spi.h"
#include "lora.h"
#include "gpio.h"


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
uint8_t rxBuffer[256];

uint8_t version;
uint8_t test;

int main(int argc, char** argv) {

    const uint8_t txMsg[] = "LoRa_TX";

    System_Init();

    LoRa_AntennaRX(); // connect antenna to module output

    // put module in LoRa mode (see SX1272 datasheet page 107)
    UARTWriteStrLn("set mode to LoRa standby");    

    LoRa_WriteRegister(REG_OP_MODE, FSK_SLEEP_MODE); // SLEEP mode required first to change bit n�7
    LoRa_WriteRegister(REG_OP_MODE, LORA_SLEEP_MODE); // switch from FSK mode to LoRa mode
    LoRa_WriteRegister(REG_OP_MODE, LORA_STANDBY_MODE); // STANDBY mode required fot FIFO loading

    LoRa_WriteRegister(REG_OP_MODE, LORA_RX_CONTINUOUS_MODE); // Continuous listening

    __delay_ms(100);
    GetMode();
    
    version = LoRa_ReadRegister(REG_VERSION);

    UARTWriteStrLn("initialized module ");

    LoRa_Init();

    uint8_t irqFlags;
    uint8_t len;

    // RxDone flag = bit 6 (0x40)r
    forever{
        irqFlags = LoRa_ReadRegister(REG_IRQ_FLAGS);
        UARTWriteStr("Polling...");
        if (irqFlags & 0x40) {
            uint8_t currentAddr = LoRa_ReadRegister(REG_FIFO_RX_CURRENT_ADDR);

            LoRa_WriteRegister(REG_FIFO_ADDR_PTR, currentAddr);

            len = LoRa_ReadRegister(REG_RX_NB_BYTES);

            for (uint8_t i = 0; i < len; i++) {
                rxBuffer[i] = LoRa_ReadRegister(REG_FIFO);
            }

            // Clear IRQ flags
            LoRa_WriteRegister(REG_IRQ_FLAGS, 0xFF);

            // process packet
            UARTWriteStr("Received signal :");

            for(uint8_t i = 0; i < len; i++)
                UARTWriteByteHex(rxBuffer[i]);

            UARTWriteStrLn("Idle mode ----------------------------");
        }
    }
}
