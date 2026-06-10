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
uint8_t txBuffer[256];

uint8_t version;
uint8_t test;

int main(int argc, char** argv) {

    const uint8_t txMsg[] = "Poisson_Steve";

    // CLKDIVbits.DOZEN = 0;
    
    System_Init();

    LoRa_AntennaTX(); // connect antenna to module output

    // put module in LoRa mode (see SX1272 datasheet page 107)
    UARTWriteStrLn("set mode to LoRa standby");    
    
    LoRa_WriteRegister(REG_OP_MODE, FSK_SLEEP_MODE); // SLEEP mode required first to change bit n�7
    LoRa_WriteRegister(REG_OP_MODE, LORA_SLEEP_MODE); // switch from FSK mode to LoRa mode
    LoRa_WriteRegister(REG_OP_MODE, LORA_STANDBY_MODE); // STANDBY mode required fot FIFO loading
    __delay_ms(100);
    GetMode();

    UARTWriteStrLn("initialized module ");
    
    LoRa_Init();
    
    version = LoRa_ReadRegister(REG_VERSION);


    strcpy((char*) txBuffer, (char*) txMsg); // load txBuffer with content of txMsg
    // txMsg is a table of constant values, so it is stored in Flash Memory
    // txBuffer is a table of variables, so it is stored in RAM

    // load FIFO with data to transmit
    UARTWriteStrLn(" ");
    UARTWriteStrLn("step 1: load FIFO");
    LoRa_WriteRegister(REG_FIFO_ADDR_PTR, LoRa_ReadRegister(REG_FIFO_TX_BASE_ADDR)); // FifoAddrPtr takes value of FifoTxBaseAddr
    LoRa_WriteRegister(REG_PAYLOAD_LENGTH_LORA, PAYLOAD_LENGTH); // set the number of bytes to transmit (PAYLOAD_LENGTH is defined in RF_LoRa868_SO.h)

    for (i = 0; i < PAYLOAD_LENGTH; i++) {
        LoRa_WriteRegister(REG_FIFO, txBuffer[i]); // load FIFO with data to transmit  
        __delay_ms(100);
    }

    forever{

        test++;
        // set mode to LoRa TX
        UARTWriteStrLn(" ");
        UARTWriteStrLn("step 2: set mode to LoRa TX");
        LoRa_WriteRegister(REG_OP_MODE, LORA_TX_MODE);
        __delay_ms(100); // delay required to start oscillator and PLL
        // GetMode();

        // wait end of transmission
        reg_val = LoRa_ReadRegister(REG_IRQ_FLAGS);
        while ((reg_val & 0x08) == 0x00) { // wait for end of transmission (wait until TxDone is set)
            reg_val = LoRa_ReadRegister(REG_IRQ_FLAGS);
        }
        UARTWriteStrLn(" ");
        UARTWriteStrLn("step 3: TxDone flag set");

        __delay_ms(200); // delay is required before checking mode: it takes some time to go from TX mode to STDBY mode
        GetMode(); // check that mode is back to STDBY

        // reset all IRQs
        UARTWriteStrLn(" ");
        UARTWriteStrLn("step 4: clear flags");
        reg_val = LoRa_ReadRegister(REG_IRQ_FLAGS);
        UARTWriteStr("before clear: REG_IRQ_FLAGS = 0x");
        UARTWriteByteHex(reg_val);

        LoRa_WriteRegister(REG_IRQ_FLAGS, 0xFF); // clear flags: writing 1 clears flag

        // check that flags are actually cleared (useless if not debugging)
        reg_val = LoRa_ReadRegister(REG_IRQ_FLAGS);
        UARTWriteStr("after clear: REG_IRQ_FLAGS = 0x");
        UARTWriteByteHex(reg_val);
        
        UARTWriteStr("step 5: wait until next transmission");

        // wait before next transmission
        for (i = 0; i < 4; i++) {
            __delay_ms(500);
        }

    }

}
