/*
 * File:   main.c
 * Authors: mizkyosia
 *mizkyosia
 * Created on 
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
#include "config.h"


uint8_t reg_val, i, irqFlags, len;

int main(int argc, char** argv) {

    System_Init();

    LoRa_WriteRegister(REG_OP_MODE, FSK_SLEEP_MODE); // SLEEP mode required first to change bit nï¿½7
    LoRa_WriteRegister(REG_OP_MODE, LORA_SLEEP_MODE); // switch from FSK mode to LoRa mode
    LoRa_WriteRegister(REG_OP_MODE, LORA_STANDBY_MODE); // STANDBY mode required fot FIFO loading

    __delay_ms(100);

    LoRa_WriteRegister(REG_FIFO_ADDR_PTR, LoRa_ReadRegister(REG_FIFO_RX_BASE_ADDR)); // FifoAddrPtr takes value of FifoTxBaseAddr

    // put module in LoRa mode (see SX1272 datasheet page 107)
    LoRa_WriteRegister(REG_OP_MODE, LORA_RX_CONTINUOUS_MODE); // Continuous listening
    
    LoRa_Init();

    LoRa_AntennaRX(); // connect antenna to module output
    
    // RxDone flag = bit 6 (0x40)r
    while(1){
        irqFlags = LoRa_ReadRegister(REG_IRQ_FLAGS);
        if (irqFlags & 0x40) {
            // wait for valid header reception
            reg_val = LoRa_ReadRegister(REG_IRQ_FLAGS);
            while ((reg_val & 0x10) == 0x00) { // check Valid Header flag (bit n°4)
                reg_val = LoRa_ReadRegister(REG_IRQ_FLAGS);
            }

            // wait for end of packet reception
            reg_val = LoRa_ReadRegister(REG_IRQ_FLAGS);
            while ((reg_val & 0x40) == 0x00) { // check Packet Reception Complete flag (bit n°6)
                reg_val = LoRa_ReadRegister(REG_IRQ_FLAGS);
            }
            
            // Fetch packet length
            len = LoRa_ReadRegister(REG_RX_NB_BYTES);

            // UART Header (signaling the start of a payload to the computer application)
            UARTWriteByte(0xAA); UARTWriteByte(0xAA);
            for (i = 0; i < len; i++) {
                reg_val = LoRa_ReadRegister(REG_FIFO); // read FIFO
                UARTWriteByteHex(reg_val);
            }

            LoRa_WriteRegister(REG_IRQ_FLAGS, 0xFF); // clear flags: writing 1 clears flag

        }
    }
}
