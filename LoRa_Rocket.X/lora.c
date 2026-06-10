#include "lora.h"
#include "spi.h"
#include "uart.h"

void LoRa_Init()
{
    uint8_t pout;

    LoRa_WriteRegister(REG_FIFO, 0x00);

    LoRa_WriteRegister(REG_FRF_MSB, 0xD8); // center freq = 865.2MHz
    LoRa_WriteRegister(REG_FRF_MID, 0x4C);
    LoRa_WriteRegister(REG_FRF_LSB, 0xCC);

    // writeRegisterREG_PA_CONFIG when out=RFIO
    //pout = (POUT + 1) & 0x0f;       // compute pout and keep 4 LSBs
    // writeRegister(REG_PA_CONFIG, pout); // out=RFIO
    // writeRegister(REG_PA_CONFIG, 0x01); // out=RFIO, 0 dBm
  
    // writeRegisterREG_PA_CONFIG when out=PA_BOOST
    pout = (POUT - 2) & 0x0F;         // compute pout and keep 4 LSBs
    LoRa_WriteRegister(REG_PA_CONFIG, 0x80 | pout); // out=PA_BOOST

    LoRa_WriteRegister(REG_PA_RAMP, 0x19); // low cons PLL TX&RX, 40us

    LoRa_WriteRegister(REG_OCP, 0b00101011); //OCP enabled, 100mA

    LoRa_WriteRegister(REG_LNA, 0b00100011); // max gain, BOOST on

    LoRa_WriteRegister(REG_FIFO_ADDR_PTR, 0x00);     // pointer to access FIFO through SPI port (read or write)
    LoRa_WriteRegister(REG_FIFO_TX_BASE_ADDR, 0x80);   // top half
    LoRa_WriteRegister(REG_FIFO_RX_BASE_ADDR, 0x00);   // bottom half

    LoRa_WriteRegister(REG_IRQ_FLAGS_MASK, 0x00);  // activate all IRQs

    LoRa_WriteRegister(REG_IRQ_FLAGS, 0xFF);  // clear all IRQs

    // in Explicit Header mode, CRC enable or disable is not relevant in case of RX operation: everything depends on TX configuration
    //LoRa_WriteRegister(REG_MODEM_CONFIG1, 0b10001000); //BW=500k, CR=4/5, explicit header, CRC disable, LDRO disabled
    LoRa_WriteRegister(REG_MODEM_CONFIG1, 0b10001010); //BW=500k, CR=4/5, explicit header, CRC enable, LDRO disabled

    LoRa_WriteRegister(REG_MODEM_CONFIG2, 0b11000111); // SF=12, normal TX mode, AGC auto on, RX timeout MSB = 11

    LoRa_WriteRegister(REG_SYMB_TIMEOUT_LSB, 0xFF);  // max timeout

    LoRa_WriteRegister(REG_PREAMBLE_MSB_LORA, 0x00); // default value
    LoRa_WriteRegister(REG_PREAMBLE_LSB_LORA, 0x08);

    LoRa_WriteRegister(REG_MAX_PAYLOAD_LENGTH, 0x80); // half the FIFO

    LoRa_WriteRegister(REG_HOP_PERIOD, 0x00); // freq hopping disabled

    LoRa_WriteRegister(REG_DETECT_OPTIMIZE, 0xC3); // pour SF=12

    LoRa_WriteRegister(REG_INVERT_IQ, 0x27); // default value, IQ not inverted

    LoRa_WriteRegister(REG_DETECTION_THRESHOLD, 0x0A); // pour SF=12

    LoRa_WriteRegister(REG_SYNC_WORD, 0x12);   // default value
}

void LoRa_InitPins() {
    LORA_CS_LAT = 1;
    LORA_CS_DIR = OUTP;
    
    RF_RESETpin_DIR = INP;

    RF_TXpin = 0;
    RF_TXpin_DIR = OUTP;

    RF_RXpin = 0;
    RF_RXpin_DIR = OUTP;
}

void LoRa_Reset() {          // apply sequence given on SX1272 datasheet page 115:
    RF_RESETpin_DIR = OUTP;         // RESET = High for 50 ms
    RF_RESETpin = 1;
    __delay_ms(50);
    RF_RESETpin_DIR = INP;          // then RESET back to high-imedance
    __delay_ms(50);
}

void LoRa_AntennaTX(void) {              // control antenna swith and connect antenna to RF module output
    RF_RXpin = 0;
    RF_TXpin = 0;
    __delay_ms(10);
    RF_TXpin = 1;
}

void LoRa_AntennaRX(void) {              // control antenna swith and connect antenna to RF module input
    RF_RXpin = 0;
    RF_TXpin = 0;
    __delay_ms(10);
    RF_RXpin = 1;
}



void LoRa_WriteRegister(uint8_t addr, uint8_t value)
{
    LORA_CS_LOW();

    SPI1_Transfer(addr | 0x80);
    SPI1_Transfer(value);

    LORA_CS_HIGH();
}

uint8_t LoRa_ReadRegister(uint8_t addr)
{
    uint8_t value;

    LORA_CS_LOW();

    SPI1_Transfer(addr & 0x7F);
    value = SPI1_Transfer(0x00);

    LORA_CS_HIGH();

    return value;
}

void LoRa_SendPacket(uint8_t *data, uint8_t length)
{
    uint8_t i;

    // Standby mode
    LoRa_WriteRegister(0x01, 0x81);

    // FIFO pointer
    LoRa_WriteRegister(0x0D, 0x00);

    // FIFO TX base address
    LoRa_WriteRegister(0x0E, 0x00);

    // Payload length
    LoRa_WriteRegister(0x22, length);

    // Write payload into FIFO
    for(i = 0; i < length; i++)
    {
        LoRa_WriteRegister(0x00, data[i]);
    }

    // TX mode
    LoRa_WriteRegister(0x01, 0x83);

    // Wait for TX done
    while((LoRa_ReadRegister(0x12) & 0x08) == 0);

    // Clear IRQ flag
    LoRa_WriteRegister(0x12, 0x08);
}









uint8_t lora_mode;

void GetMode(void)
{
    uint8_t reg, masked_reg;
    reg = LoRa_ReadRegister(REG_OP_MODE);

    // for debugging: send reg value to terminal
    UARTWriteStr("REG_OP_MODE = 0x");
    UARTWriteByteHex(reg);

    masked_reg = reg & 0x80; // to check bit n?7
    if (masked_reg)
    {
        // MSB of RegOpMode is 1, so mode = LoRa
        masked_reg = reg & 0x40; // to check bit n?6
        if (!masked_reg)
            UARTWriteStrLn("mode = LoRa");
        else
            UARTWriteStrLn("mode = LoRa with FSK registers access");
    }
    else
        // MSB of RegOpMode is 0, so mode = FSK
        UARTWriteStrLn("mode = FSK");

    masked_reg = reg & 0x07; // test bits 2-0 of RegOpMode
    switch (masked_reg)
    {
    case 0x00:
        UARTWriteStrLn("sleep mode");
        break;
    case 0x01:
        UARTWriteStrLn("standby mode");
        break;
    case 0x02:
        UARTWriteStrLn("frequency synthesis TX");
        break;
    case 0x03:
        UARTWriteStrLn("TX mode");
        break;
    case 0x04:
        UARTWriteStrLn("frequency synthesis RX");
        break;
    case 0x05:
        UARTWriteStrLn("continuous receive mode");
        break;
    case 0x06:
        UARTWriteStrLn("single receive mode");
        break;
    case 0x07:
        UARTWriteStrLn("Channel Activity Detection");
        break;
    }
    
    lora_mode = reg;
}