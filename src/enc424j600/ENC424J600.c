/**
 * \file ENC424J600.c
 * \brief Drives Microchip's Ethernet Controller ENC424J600 via SPI interface. Parallel interfacing is not supported.
 * \author Stefan Gloor
 * \version 1.0
 * \date 31. December 2018
 * \todo Filter settings; not just unicast/broadcast but all packets visible
 * \copyright    
 *  Copyright (C) 2019  Stefan Gloor
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.  
 */

#include "../enc424j600/ENC424J600.h"
#include "../system/uart.h"
#include "../enc424j600/interrupt.h"

static uint16_t nextPacketPointer = 0x0000; //holds address value of received packet

/* ==============================================  MORE GENERIC ETHERNET CONTROLLER  ============================================== */

error_t ethernetController_init() {
    error_t err;
    ENC424J600_initSPI();
    err = ethernetController_softReset();
    if (err.code != ERROR_CODE_SUCCESSFUL) {
        //return err; //abort
    }
    err = ethernetController_checkDeviceId();
    if (err.code != ERROR_CODE_SUCCESSFUL) {
        //return err; //abort
    }

    /* From the datasheet:
    1. Program the ERXST Pointer (low byte first if
    writing a byte at a time) to the first address to be
    used for the receive buffer. This pointer must
    indicate an even address. The Head Pointer,
    ERXHEAD, will automatically be set to the same
    value.
    2. In the host controller application, create a
    variable, NextPacketPointer, to hold the
    address value of the next received packet.
    Initialize this variable to be equal to the current
    value of ERXST.
    3. Program the Tail Pointer, ERXTAIL, to the last
    even address of the buffer or 5FFEh.
    4. Configure interrupts as desired. See
    Section 13.0 ?Interrupts? for more information.
    5. Set RXEN (ECON1<0>) to enable reception. */
    ENC424J600_setRXBufferStartAddress(RX_DATA_START_ADDRESS); // STEP 1
    ENC424J600_setNextPacketPointer(RX_DATA_START_ADDRESS); // STEP 2
    ENC424J600_setRXTailPointer(0x5FFE); // STEP 3
    //STEP 4
    ENC424J600_enableReception(); // STEP 5
    ENC424J600_enableAutoMACInsertion();

    err.module = ERROR_MODULE_ETHERNET_CONTROLLER;
    err.code = ERROR_CODE_SUCCESSFUL;
    return err;
}

error_t ethernetController_softReset() {
    /*	1. Write 1234h to EUDAST.
        2. Read EUDAST to see if it now equals 1234h.If
        it does not, the SPI / PSP interface may not be
        ready yet, so return to step 1 and try again.
        3. Poll CLKRDY(ESTAT<12>) and wait for it to
        become set.
        4. Issue a System Reset command by setting
        ETHRST(ECON2<4>).
        5. In software, wait at least 25 us for the Reset to
        take place and the SPI / PSP interface to begin
        operating again.
        6. Read EUDAST to confirm that the System Reset
        took place.EUDAST should have reverted back
        to its Reset default of 0000h.
        7. Wait at least 256 us for the PHY registers and
        PHY status bits to become available */
#define MAX_COUNT_COMMUNICATION_FAIL	5
    uint8_t comFailCount = 0;
    uint8_t dataToSend[2] = {0x12, 0x34};
    uint8_t receivedData[2] = {0, 0};
    error_t err;
    err.module = ERROR_MODULE_ETHERNET_CONTROLLER;
    err.code = ERROR_CODE_SUCCESSFUL;
    if (comFailCount < MAX_COUNT_COMMUNICATION_FAIL) {//try several times to write and verify 0x1234
        //write 0x1234 to EUDAST
        ENC424J600_writeControlRegisterUnbanked(EUDASTL + BANK_0_OFFSET, &dataToSend[0]);
        ENC424J600_writeControlRegisterUnbanked(EUDASTH + BANK_0_OFFSET, &dataToSend[1]);
        //read from EUDAST
        ENC424J600_readControlRegisterUnbanked(EUDASTL + BANK_0_OFFSET, &receivedData[0]);
        ENC424J600_readControlRegisterUnbanked(EUDASTH + BANK_0_OFFSET, &receivedData[1]);
        //verify 0x1234 has been written to EUDAST
        if ((receivedData[0] == dataToSend[0]) && (receivedData[1] == dataToSend[1])) {
            //it was successful
            ENC424J600_writeSingleByte(SETETHRST); //issue software reset
            __delay_us(25);
            //read from EUDAST
            ENC424J600_readControlRegisterUnbanked(EUDASTL + BANK_0_OFFSET, &receivedData[0]);
            ENC424J600_readControlRegisterUnbanked(EUDASTH + BANK_0_OFFSET, &receivedData[1]);
            //does it equal 0x0000?
            if (!receivedData[0] && !receivedData[1]) {
                //everything's fine
                __delay_us(256);
                return err;
            } else {//reset was not successful because EUDAST doesn't equal 0x0000, issue an error
                err.code = ERROR_ETHERNET_CONTROLLER_UNRESPONSIVE;
                return err;
            }
        }
        comFailCount++; //try several times to write and verify 0x1234
    } else {//0x1234 could not be written/read 
        err.code = ERROR_ETHERNET_CONTROLLER_UNRESPONSIVE;
        return err;
    }
    return err;
}

error_t ethernetController_checkDeviceId() {
    error_t err;
    err.module = ERROR_MODULE_ETHERNET_CONTROLLER;
    err.code = ERROR_CODE_SUCCESSFUL;
    uint8_t data = 0;
    ENC424J600_readControlRegisterUnbanked(EIDLEDL + BANK_3_OFFSET, &data);
    if (((data >> 5)&0x07) == EXPECTED_DEVICE_ID)//check device id bits
        err.code = ERROR_CODE_SUCCESSFUL;
    else
        err.code = ERROR_ETHERNET_CONTROLLER_UNKNOWN_DEVICE_ID;
    return err;
}

char *ethernetController_getDeviceName() {
    const char string [] = DEVICE_NAME;
    return string;
}

uint8_t ethernetController_getSiliconRevision() {
    uint8_t data = 0;
    ENC424J600_readControlRegisterUnbanked(EIDLEDL + BANK_3_OFFSET, &data);
    data &= 0x1F; //mask out unwanted bits
    return data;
}

macaddress_t ethernetController_getMacAddress() {
    macaddress_t mac;
    ENC424J600_readControlRegisterUnbanked(MAADR1L + BANK_3_OFFSET, &mac.address[0]);
    ENC424J600_readControlRegisterUnbanked(MAADR1H + BANK_3_OFFSET, &mac.address[1]);
    ENC424J600_readControlRegisterUnbanked(MAADR2L + BANK_3_OFFSET, &mac.address[2]);
    ENC424J600_readControlRegisterUnbanked(MAADR2H + BANK_3_OFFSET, &mac.address[3]);
    ENC424J600_readControlRegisterUnbanked(MAADR3L + BANK_3_OFFSET, &mac.address[4]);
    ENC424J600_readControlRegisterUnbanked(MAADR3H + BANK_3_OFFSET, &mac.address[5]);
    return mac;
}

uint16_t ethernetController_getMTU() {
    return MAX_DATA_FIELD_LENGTH;
}

void ethernetController_setMacAddress(macaddress_t mac) {
    ENC424J600_writeControlRegisterUnbanked(MAADR1L + BANK_3_OFFSET, &mac.address[0]);
    ENC424J600_writeControlRegisterUnbanked(MAADR1H + BANK_3_OFFSET, &mac.address[1]);
    ENC424J600_writeControlRegisterUnbanked(MAADR2L + BANK_3_OFFSET, &mac.address[2]);
    ENC424J600_writeControlRegisterUnbanked(MAADR2H + BANK_3_OFFSET, &mac.address[3]);
    ENC424J600_writeControlRegisterUnbanked(MAADR3L + BANK_3_OFFSET, &mac.address[4]);
    ENC424J600_writeControlRegisterUnbanked(MAADR3H + BANK_3_OFFSET, &mac.address[5]);
}

void ethernetController_enableReception() {
    ENC424J600_enableReception();
}

void ethernetController_disableReception() {
    ENC424J600_disableReception();
}

void ethernetController_enableTransmission() {

}

void ethernetController_enableEthernet() {

}

void ethernetController_disableEthernet() {

}

void ethernetController_sendPacket(memoryField_t field) {
    ENC424J600_setTXStartAddress(field.start);
    ENC424J600_setTXLength(field.length);
    ENC424J600_writeSingleByte(SETTXRTS);
    memory_txFrameClear(field.index);
}

void ethernetController_streamToTransmitBuffer(uint8_t data, memoryField_t field) {
    uint8_t opcode;
    uint16_t static pointer = 0;
    if (pointer == 0) {//at the first byte
        ENC424J600_setGPDATAWritePointer(field.start); //payload data offset (6 Byte MAC address + 2 Byte EtherType)*/
        opcode = 0x2A; //EGPDATA opcode
        CS_PIN_LOW; //Start transmission
        ENC424J600_writeSPI(&opcode);
    }

    ENC424J600_writeSPI(&data);

    if (pointer == field.length - 1) {
        CS_PIN_HIGH; //Stop transmission
        pointer = 0;
    } else {
        pointer++;
    }
}

void ethernetController_writeDestinationMACAddress(macaddress_t addr, memoryField_t field) {
    ENC424J600_setGPDATAWritePointer(field.start);
    uint8_t data[6];
    for (uint8_t i = 0; i < 6; i++)
        data[i] = addr.address[i];
    uint8_t opcode = 0x2A; //EGPDATA opcode
    CS_PIN_LOW;
    ENC424J600_writeSPI(&opcode);
    for (uint8_t i = 0; i < 6; i++)
        ENC424J600_writeSPI(&data[i]);
    CS_PIN_HIGH;
}

void ethernetController_writeEtherTypeFieldToBuffer(etherType_t ethtype, memoryField_t field) {
    ENC424J600_setGPDATAWritePointer(field.start + 0x0006);
    uint8_t lowByte = ethtype & 0x00ff;
    uint8_t highByte = (ethtype & 0xff00) >> 8;
    uint8_t opcode = 0x2A; //EGPDATA opcode
    CS_PIN_LOW;
    ENC424J600_writeSPI(&opcode);
    ENC424J600_writeSPI(&highByte);
    ENC424J600_writeSPI(&lowByte);
    CS_PIN_HIGH;
}

macaddress_t ethernetController_getDestinationMACAddress(memoryField_t field) {
    macaddress_t destinationAddress;
    ENC424J600_setERXDATAReadPointer(field.start + 8); //Fixed offset: 2 Bytes NextPacketPointer, 6 Bytes RSV
    uint8_t opcode = 0x2C; //ERXDATA Read opcode
    CS_PIN_LOW; //Start the read operation
    ENC424J600_writeSPI(&opcode);
    for (uint8_t i = 0; i < 6; i++) {//MAC address has 6 bytes
        uint8_t temp;
        ENC424J600_readSPI(&temp); //Read a byte
        destinationAddress.address[i] = temp;
    }
    CS_PIN_HIGH; //End the read operation
    return destinationAddress;
}

macaddress_t ethernetController_getSourceMACAddress(memoryField_t field) {
    macaddress_t sourceAddress;
    ENC424J600_setERXDATAReadPointer(field.start + 14); //Fixed offset: 2 Bytes NextPacketPointer, 6 Bytes RSV, 6 Bytes Destination MAC address
    uint8_t opcode = 0x2C; //ERXDATA Read opcode
    CS_PIN_LOW; //Start the read operation
    ENC424J600_writeSPI(&opcode);
    for (uint8_t i = 0; i < 6; i++) {//MAC address has 6 bytes
        uint8_t temp;
        ENC424J600_readSPI(&temp); //Read a byte
        sourceAddress.address[i] = temp;
    }
    CS_PIN_HIGH; //End the read operation
    return sourceAddress;
}

etherType_t ethernetController_getEtherTypeField(memoryField_t field) {
    etherType_t ethertype = 0x0000;
    ENC424J600_setERXDATAReadPointer(field.start + 20); //Fixed offset: 2 Bytes NextPacketPointer, 6 Bytes RSV, 12 Bytes MAC addresses
    uint8_t opcode = 0x2C; //ERXDATA Read opcode
    CS_PIN_LOW; //Start the read operation
    ENC424J600_writeSPI(&opcode);

    uint8_t lowByte;
    uint8_t highByte;
    ENC424J600_readSPI(&highByte); //Read high byte
    ENC424J600_readSPI(&lowByte); //Read low byte
    ethertype = (uint16_t) (lowByte | (highByte << 8));
    CS_PIN_HIGH; //End the read operation
    return ethertype;
}

RSV_t ethernetController_getRSV(uint16_t address) {
    uint8_t static temp[6]; //needs to be static because its address gets returned
    ENC424J600_setERXDATAReadPointer(address + 2); //Fixed offset: 2 Bytes NextPacketPointer
    uint8_t opcode = 0x2C; //ERXDATA Read opcode
    CS_PIN_LOW; //Start the read operation
    ENC424J600_writeSPI(&opcode);
    for (uint8_t i = 0; i < 6; i++) {//RSV has 6 bytes
        ENC424J600_readSPI(&temp[i]); //Read a byte
    }
    CS_PIN_HIGH; //End the read operation
    return ENC424J600_updateReceiveStatusVector(&temp[0]);
}

interruptFlags_t ethernetController_pollInterruptFlags() {
    interruptFlags_t flags;
    uint16_t reg = ENC424J600_getInterruptFlags();
    flags.MODEXIF = (reg & (1 << 14)) != 0 ? true : false;
    flags.HASHIF = (reg & (1 << 13)) != 0 ? true : false;
    flags.AESIF = (reg & (1 << 12)) != 0 ? true : false;
    flags.LINKIF = (reg & (1 << 11)) != 0 ? true : false;
    //Bit 10-7: Reserved
    flags.PKTIF = (reg & (1 << 6)) != 0 ? true : false;
    flags.DMAIF = (reg & (1 << 5)) != 0 ? true : false;
    //Bit 4: Reserved
    flags.TXIF = (reg & (1 << 3)) != 0 ? true : false;
    flags.TXABTIF = (reg & (1 << 2)) != 0 ? true : false;
    flags.RXABTIF = (reg & (1 << 1)) != 0 ? true : false;
    flags.PCFULIF = (reg & (1 << 0)) != 0 ? true : false;
    return flags;
}

void ethernetController_clearInterruptFlag(uint8_t flag) {
    ENC424J600_clearInterruptFlag(flag);
}

void ethernetController_updateNextPacketPointer() {
    uint16_t nextPktPointer;
    uint8_t opcode;
    uint8_t newPointerL, newPointerH;
    nextPktPointer = ENC424J600_getNextPacketPointer();
    ENC424J600_setERXDATAReadPointer(nextPktPointer);
    opcode = 0x2C; //ERXDATA Read opcode
    CS_PIN_LOW; //Start the read operation
    ENC424J600_writeSPI(&opcode);
    ENC424J600_readSPI(&newPointerL); //first field is the new nextPacketPointer
    ENC424J600_readSPI(&newPointerH);
    CS_PIN_HIGH;
    //update nextPacketPointer
    ENC424J600_setNextPacketPointer((uint16_t) ((newPointerL & (unsigned) 0x00ff) | ((unsigned) (newPointerH << 8)&(unsigned) 0xff00)));
}

uint8_t ethernetController_streamFromRXBuffer(uint8_t startEnd, uint16_t startAddress) {
    uint8_t temp;
    uint8_t opcode;
    switch (startEnd) {
        case 0://Begin data streaming
            ENC424J600_setERXDATAReadPointer(startAddress);
            opcode = 0x2C; //ERXDATA Read opcode
            CS_PIN_LOW; //Start the read operation
            ENC424J600_writeSPI(&opcode);
            return 0;
        case 1://Real data is being read
            ENC424J600_readSPI(&temp); //Read the whole frame
            return temp;
        case 2:
            CS_PIN_HIGH; //End the read operation
            //free up memory by updating the tail pointer to the point where the data has already been processed
            if (ethernetController_getNextPacketPointer() == RX_DATA_START_ADDRESS) {//wrap around 
                ENC424J600_setRXTailPointer(0x55FE);
            } else {
                ENC424J600_setRXTailPointer(ethernetController_getNextPacketPointer() - 2);
            }

            //Set PKTDEC bit so the PKTCNT gets decremented
            ENC424J600_writeSingleByte(SETPKTDEC);
            return 0;
    }
    return 0; //never reached (or when an invalid startEnd is passed)
}

void ethernetController_dropPacket(ethernetFrame_t *frame) {
    /*  uint32_t nextPktPointer;
      uint8_t newPointerL, newPointerH;
      uint8_t opcode;
      nextPktPointer = ENC424J600_getNextPacketPointer();
      ENC424J600_setERXDATAReadPointer(nextPktPointer);
      opcode = 0x2C; //ERXDATA Read opcode
      CS_PIN_LOW; //Start the read operation
      ENC424J600_writeSPI(&opcode);
      ENC424J600_readSPI(&newPointerL); //first field is the new nextPacketPointer
      ENC424J600_readSPI(&newPointerH);
      //update nextPacketPointer
      ENC424J600_setNextPacketPointer((uint16_t) ((newPointerL & (unsigned) 0x00ff) | ((unsigned) (newPointerH << 8)&(unsigned) 0xff00)));
      CS_PIN_HIGH; //End the read operation*/
    //free up memory
    if (ethernetController_getNextPacketPointer() == RX_DATA_START_ADDRESS) {//wrap around 
        ENC424J600_setRXTailPointer(END_OF_MEMORY_ADDRESS - 1); //-1 because we need the last *even* memory address
    } else {
        ENC424J600_setRXTailPointer(ethernetController_getNextPacketPointer() - 2);
    }
    //Set PKTDEC bit so the PKTCNT gets decremented
    ENC424J600_writeSingleByte(SETPKTDEC);
}

uint8_t ethernetController_newPacketAvailable() {
    return (unsigned) (ENC424J600_getPacketCount() != 0);
}

void ethernetController_updateLinkStatus(ethernetConnection_t * state) {
    uint8_t temp, temp16;
    ENC424J600_readControlRegisterUnbanked(ESTATH, &temp);
    if (temp & 0x01) {//Bit 8, PHYLNK
        state->link = LINK_ESTABLISHED;
    } else {
        state->link = NO_LINK;
    }

    ENC424J600_readPHYRegister(0x1F, &temp16); //PHSTAT3 Register

    if (temp16 & (1 << 3)) {
        state->speed = HUNDRED_MBIT;
    } else {
        state->speed = TEN_MBIT;
    }

    if (temp16 & (1 << 4)) {
        state->duplex = FULL_DUPLEX;
    } else {
        state->duplex = HALF_DUPLEX;
    }
}

/*void ethernetController_setLEDStatus(uint8_t LED, uint8_t status) {
    uint8_t data;
    ENC424J600_readControlRegisterUnbanked(EIDLEDH + BANK_3_OFFSET, &data);
    if (LED == LEDB) {
        //LED B
        if (status == LED_ON) {
            data &= ~0x0f; //set LBCFG to 0b0000 
        } else if (status == LED_OFF) {
            //set LBCFG to 0b0001
            data &= ~0x0f;
            data |= 0x01;
        }
    } else if (LED == LEDA) {
        //LED A
        if (status == LED_ON) {
            data &= ~(0x0f << 4); //set LACFG to 0b0000 
        } else if (status == LED_OFF) {
            //set LACFG to 0b0001
            data &= ~(0x0f << 4);
            data |= (0x01 << 4);
        }
    }
    ENC424J600_writeControlRegisterUnbanked(EIDLEDH + BANK_3_OFFSET, &data);
}*/

void ethernetController_setLEDConfig(LEDs_t LED, LEDStates_t conf) {
    uint8_t data;
    ENC424J600_readControlRegisterUnbanked(EIDLEDH + BANK_3_OFFSET, &data);
    if (LED == LEDB) {
        //LED B
        switch (conf) {
            default:
            case LED_OFF:
                data &= ~0x0f;
                data |= 0x01;
                break;
            case LED_ON:
                data &= ~0x0f; //set LBCFG to 0b0000 
                break;
            case LED_TRANSMIT_RECEIVE_EVENTS:
                data &= ~0x0f;
                data |= 0x06;
                break;
        }
    } else if (LED == LEDA) {
        switch (conf) {
            default:
            case LED_OFF:
                data &= ~(0x0f << 4);
                data |= (0x01 << 4);
                break;
            case LED_ON:
                data &= ~(0x0f << 4); //set LBCFG to 0b0000 
                break;
            case LED_TRANSMIT_RECEIVE_EVENTS:
                data &= ~(0x0f << 4);
                data |= (0x06 << 4);
                break;
        }
    }
    ENC424J600_writeControlRegisterUnbanked(EIDLEDH + BANK_3_OFFSET, &data);
}

uint8_t ethernetController_getCurrentPacketCount() {
    return ENC424J600_getPacketCount();
}

uint16_t ethernetController_getNextPacketPointer() {
    return ENC424J600_getNextPacketPointer();
}

/* ==============================================  DEVICE SPECIFIC - ETHERNET CONTROLLER  ============================================== */
/* =======================  Initialisation  ======================= */

void static ENC424J600_initSPI() {
    SSP1CON1bits.SSPEN = 0; //disable serial port
    SSP1STATbits.CKE = 1; //clock transition polarity
    //SPI Master mode, clock = Fosc/4 is selected by POR
    TRISBbits.TRISB0 = 1; //MISO
    ANSELBbits.ANSB0 = 0;
    TRISBbits.TRISB1 = 0; //SCK
    ANSELBbits.ANSB1 = 0;
    TRISBbits.TRISB3 = 0; //MOSI
    ANSELBbits.ANSB3 = 0;
    TRISBbits.TRISB4 = 0; //SS
    ANSELBbits.ANSB4 = 0;
    CS_PIN_HIGH;
    SSP1CON1bits.SSPEN = 1; //Enable serial port
}

/* =======================  READ/WRITE REGISTERS  ======================= */

void static ENC424J600_writeSPI(uint8_t *data) {
#define SPI_TIMEOUT		0xfff //max cycle count to wait for transmission of one byte
    uint32_t timeoutCounter = 0;
    SSP1BUF = *data; //writing to buffer starts transmission
    while ((!SSP1STATbits.BF) && (timeoutCounter++ < SPI_TIMEOUT)); //wait for completion of transmission
    if (timeoutCounter >= SPI_TIMEOUT) {//if timeout reached, return
        CS_PIN_HIGH; //abort transmission
        ENC424J600_initSPI(); //reset module
        return;
    }
}

void static ENC424J600_readSPI(uint8_t *data) {
#define SPI_TIMEOUT		0xfff//max cycle count to wait for transmission of one byte
    uint32_t timeoutCounter = 0;
    SSP1BUF = 0x00; //dummy
    while ((!SSP1STATbits.BF) && (timeoutCounter++ < SPI_TIMEOUT)); //wait for completion of transmission
    if (timeoutCounter >= SPI_TIMEOUT) {//if timeout reached, return
        CS_PIN_HIGH; //abort transmission
        ENC424J600_initSPI(); //reset module
        return;
    }
    *data = SSP1BUF;
}

void static ENC424J600_writeSingleByte(uint8_t opcode) {
    CS_PIN_LOW;
    ENC424J600_writeSPI(&opcode);
    CS_PIN_HIGH;
}

void static ENC424J600_writeControlRegisterUnbanked(uint8_t addr, uint8_t *data) {
    uint8_t opcode = 0x22;
    CS_PIN_LOW;
    ENC424J600_writeSPI(&opcode);
    ENC424J600_writeSPI(&addr);
    ENC424J600_writeSPI(data);
    CS_PIN_HIGH;
}

void static ENC424J600_readControlRegisterUnbanked(uint8_t addr, uint8_t *data) {
    uint8_t opcode = 0x20;
    CS_PIN_LOW;
    ENC424J600_writeSPI(&opcode);
    ENC424J600_writeSPI(&addr);
    ENC424J600_readSPI(data);
    CS_PIN_HIGH;
}

/* =======================  PHY REGISTERS  ======================= */

void static ENC424J600_readPHYRegister(uint8_t addr, uint16_t *data) {
#define TIMEOUT 0xfff
    uint8_t PHYaddress = addr & 0x1F;
    uint8_t unusedBits = 0x01;
    uint8_t temp;
    uint8_t lowByte;
    uint8_t highByte;
    uint32_t timeoutCounter = 0;

    ENC424J600_writeControlRegisterUnbanked(MIREGADRL + BANK_2_OFFSET, &PHYaddress); //set PHY address
    ENC424J600_writeControlRegisterUnbanked(MIREGADRH + BANK_2_OFFSET, &unusedBits);

    //init read process by setting MICMD<0>
    ENC424J600_readControlRegisterUnbanked(MICMDL + BANK_2_OFFSET, &temp);
    temp |= 0x01;
    ENC424J600_writeControlRegisterUnbanked(MICMDL + BANK_2_OFFSET, &temp);

    __delay_us(26); //wait at least 25.6us

    do { //poll busy bit
        ENC424J600_readControlRegisterUnbanked(MISTATL + BANK_3_OFFSET, &temp);
        timeoutCounter++;
    } while ((temp & 0x01) && (timeoutCounter < TIMEOUT));
    if (timeoutCounter >= TIMEOUT)//timeout!
        return;

    //finish read process by clearing MICMD<0>
    ENC424J600_readControlRegisterUnbanked(MICMDL + BANK_2_OFFSET, &temp);
    temp &= ~0x01;
    ENC424J600_writeControlRegisterUnbanked(MICMDL + BANK_2_OFFSET, &temp);

    //read register 
    ENC424J600_readControlRegisterUnbanked(MIRDL + BANK_3_OFFSET, &lowByte);
    ENC424J600_readControlRegisterUnbanked(MIRDH + BANK_3_OFFSET, &highByte);
    *data = (lowByte | ((highByte << 8)&0xff00));
}

/* =======================  TRANSMISSION  ======================= */

void static ENC424J600_setTXLength(uint16_t len) {
    uint8_t lowByte = len & 0x00ff;
    uint8_t highByte = len >> 8;
    ENC424J600_writeControlRegisterUnbanked(ETXLENL + BANK_0_OFFSET, &lowByte);
    ENC424J600_writeControlRegisterUnbanked(ETXLENH + BANK_0_OFFSET, &highByte);
}

/* =======================  RECEPTION  ======================= */

void static ENC424J600_setNextPacketPointer(uint16_t ptr) {
    nextPacketPointer = ptr;
}

uint16_t static ENC424J600_getNextPacketPointer() {
    return nextPacketPointer;
}

uint8_t static ENC424J600_getPacketCount() {
    uint8_t lowByte = 0;
    ENC424J600_readControlRegisterUnbanked(ESTATL + BANK_0_OFFSET, &lowByte);
    return (uint32_t) lowByte;
}

/* =======================  POINTER OPERATIONS  ======================= */

void static ENC424J600_setTXStartAddress(uint16_t addr) {
    uint8_t lowByte = addr & 0xff;
    uint8_t highByte = (addr & 0xff00) >> 8;
    ENC424J600_writeControlRegisterUnbanked(ETXSTL + BANK_0_OFFSET, &lowByte);
    ENC424J600_writeControlRegisterUnbanked(ETXSTH + BANK_0_OFFSET, &highByte);
}

void static ENC424J600_setGPDATAWritePointer(uint16_t addr) {
    uint8_t opcode = 0x6C; //write GPWRPT opcode
    uint8_t lowByte = addr & 0x00ff;
    uint8_t highByte = (addr & 0xff00) >> 8;
    CS_PIN_LOW;
    ENC424J600_writeSPI(&opcode);
    ENC424J600_writeSPI(&lowByte);
    ENC424J600_writeSPI(&highByte);
    CS_PIN_HIGH;
}

void static ENC424J600_setGPDATAReadPointer(uint16_t addr) {
    uint8_t opcode = 0x60; //write GPRDPT opcode
    uint8_t lowByte = addr & 0x00ff;
    uint8_t highByte = (addr & 0xff00) >> 8;
    CS_PIN_LOW;
    ENC424J600_writeSPI(&opcode);
    ENC424J600_writeSPI(&lowByte);
    ENC424J600_writeSPI(&highByte);
    CS_PIN_HIGH;
}

void static ENC424J600_setERXDATAReadPointer(uint16_t addr) {
    uint8_t opcode = 0x64; //write ERXRDPT opcode
    uint8_t lowByte = addr & 0x00ff;
    uint8_t highByte = (addr >> 8) & 0xff;
    CS_PIN_LOW;
    ENC424J600_writeSPI(&opcode);
    ENC424J600_writeSPI(&lowByte);
    ENC424J600_writeSPI(&highByte);
    CS_PIN_HIGH;
}

void static ENC424J600_setRXBufferStartAddress(uint16_t addr) {
    uint8_t lowByte = addr & 0x00ff;
    uint8_t highByte = (addr >> 8) & 0xff;
    ENC424J600_writeControlRegisterUnbanked(ERXSTL + BANK_0_OFFSET, &lowByte);
    ENC424J600_writeControlRegisterUnbanked(ERXSTH + BANK_0_OFFSET, &highByte);
}

void static ENC424J600_setRXTailPointer(uint16_t addr) {
    uint8_t lowByte = addr & 0x00ff;
    uint8_t highByte = (addr >> 8) & 0xff;
    ENC424J600_writeControlRegisterUnbanked(ERXTAILL + BANK_0_OFFSET, &lowByte);
    ENC424J600_writeControlRegisterUnbanked(ERXTAILH + BANK_0_OFFSET, &highByte);
}

/* =======================  SETTINGS  ======================= */

void static ENC424J600_enableAutoMACInsertion() {
    uint8_t temp = 0;
    ENC424J600_readControlRegisterUnbanked(ECON2H + BANK_3_OFFSET, &temp);
    temp |= (1 << 5); //enable TXMAC
    ENC424J600_writeControlRegisterUnbanked(ECON2H + BANK_3_OFFSET, &temp);
}

void static ENC424J600_disableAutoMACInsertion() {
    uint8_t temp = 0;
    ENC424J600_readControlRegisterUnbanked(ECON2H + BANK_3_OFFSET, &temp);
    temp &= ~(1 << 5); //disable TXMAC
    ENC424J600_writeControlRegisterUnbanked(ECON2H + BANK_3_OFFSET, &temp);
}

void static ENC424J600_enableReception() {
    ENC424J600_writeSingleByte(ENABLERX);
}

void static ENC424J600_disableReception() {
    ENC424J600_writeSingleByte(DISABLERX);
}

void static ENC424J600_enable() {
    uint8_t temp;
    ENC424J600_readControlRegisterUnbanked(ECON2H + BANK_3_OFFSET, &temp);
    temp |= (1 << 7); //Set the ETHEN bit
    ENC424J600_writeControlRegisterUnbanked(ECON2H + BANK_3_OFFSET, &temp);
}

void static ENC424J600_disable() {
    uint8_t temp;
    ENC424J600_readControlRegisterUnbanked(ECON2H + BANK_3_OFFSET, &temp);
    temp &= ~(1 << 7); //Clear the ETHEN bit
    ENC424J600_writeControlRegisterUnbanked(ECON2H + BANK_3_OFFSET, &temp);
}

/* =======================  INTERRUPTS  ======================= */

uint16_t ENC424J600_getInterruptFlags() {
    uint8_t low, high;
    ENC424J600_readControlRegisterUnbanked(EIRH + BANK_0_OFFSET, &high);
    ENC424J600_readControlRegisterUnbanked(EIRL + BANK_0_OFFSET, &low);
    return (((uint16_t) low & 0x00ff) | (((uint16_t) high << 8)&0xff00));
}

void static ENC424J600_clearInterruptFlag(uint8_t flag) {
    uint8_t temp;
    if (flag < 8) {
        ENC424J600_readControlRegisterUnbanked(EIRL + BANK_0_OFFSET, &temp);
        temp &= ~(1 << flag);
        ENC424J600_writeControlRegisterUnbanked(EIRL + BANK_0_OFFSET, &temp);
    } else {
        flag -= 8;
        ENC424J600_readControlRegisterUnbanked(EIRH + BANK_0_OFFSET, &temp);
        temp &= ~(1 << flag);
        ENC424J600_writeControlRegisterUnbanked(EIRH + BANK_0_OFFSET, &temp);
    }
}

/* =======================  RECEIVE STATUS VECTOR  ======================= */

RSV_t static ENC424J600_updateReceiveStatusVector(uint8_t *rsv) {
    RSV_t receiveStatusVector;
    //BYTE 0    //Length 
    //BYTE 1    //Length
    receiveStatusVector.length = (uint16_t) ((*(rsv + 0)) | (uint16_t) (((*(rsv + 1)) << 8)&0xff00));
    //BYTE 2
    receiveStatusVector.packetPreviouslyIgnored = (uint8_t) (*(rsv + 2) & RSV_BYTE2_PACKET_PREVIOUSLY_IGNORED) != 0 ? true : false;
    receiveStatusVector.carrierEventPreviouslySeen = (uint8_t) (*(rsv + 2) & RSV_BYTE2_CARRIER_EVENT_PREVIOUSLY_SEEN) != 0 ? true : false;
    receiveStatusVector.CRCError = (uint8_t) (*(rsv + 2) & RSV_BYTE2_CRC_ERROR) != 0 ? true : false;
    receiveStatusVector.lengthCheckError = (uint8_t) (*(rsv + 2) & RSV_BYTE2_LENGTH_CHECK_ERROR) != 0 ? true : false;
    receiveStatusVector.lengthOutOfRange = (uint8_t) (*(rsv + 2) & RSV_BYTE2_LENGTH_OUT_OF_RANGE) != 0 ? true : false;
    receiveStatusVector.receivedOk = (uint8_t) (*(rsv + 2) & RSV_BYTE2_RECEIVED_OK) != 0 ? true : false;
    //BYTE 3
    receiveStatusVector.multicast = (uint8_t) (*(rsv + 3) & RSV_BYTE3_RECEIVED_MULTICAST_PACKET) != 0 ? true : false;
    receiveStatusVector.broadcast = (uint8_t) (*(rsv + 3) & RSV_BYTE3_RECEIVED_BROADCAST_PACKET) != 0 ? true : false;
    receiveStatusVector.controlFrame = (uint8_t) (*(rsv + 3) & RSV_BYTE3_RECEIVED_CONTROL_FRAME) != 0 ? true : false;
    receiveStatusVector.pauseFrame = (uint8_t) (*(rsv + 3) & RSV_BYTE3_RECEIVED_PAUSE_CONTROL_FRAME) != 0 ? true : false;
    receiveStatusVector.unknownOpcode = (uint8_t) (*(rsv + 3) & RSV_BYTE3_RECEIVED_UNKNOWN_OPCODE) != 0 ? true : false;
    receiveStatusVector.vlan = (uint8_t) (*(rsv + 3) & RSV_BYTE3_RECEIVED_VLAN) != 0 ? true : false;
    receiveStatusVector.runtFilter = (uint8_t) (*(rsv + 3) & RSV_BYTE3_RUNT_FILTER_MATCH) != 0 ? true : false;
    //BYTE 4
    receiveStatusVector.notMeFilter = (uint8_t) (*(rsv + 4) & RSV_BYTE4_NOT_ME_FILTER_MATCH) != 0 ? true : false;
    receiveStatusVector.hashFilter = (uint8_t) (*(rsv + 4) & RSV_BYTE4_HASH_FILTER_MATCH) != 0 ? true : false;
    receiveStatusVector.magicFilter = (uint8_t) (*(rsv + 4) & RSV_BYTE4_MAGIC_PACKET_FILTER_MATCH) != 0 ? true : false;
    receiveStatusVector.patternMatch = (uint8_t) (*(rsv + 4) & RSV_BYTE4_PATTERN_MATCH_FILTER_MATCH) != 0 ? true : false;
    receiveStatusVector.unicast = (uint8_t) (*(rsv + 4) & RSV_BYTE4_UNICAST_FILTER_MATCH) != 0 ? true : false;

    return receiveStatusVector;
}


