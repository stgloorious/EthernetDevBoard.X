/**
 * \file ethernet.c
 * \brief Acts as interface between hardware/medium and upper laying protocols
 * \author Stefan Gloor
 * \version 1.0
 * \date 16. Februar 2019
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

#include "../eth/ethernet.h"
#include "../enc424j600/ENC424J600.h"
#include "../system/uart.h"

error_t ethernet_txFrameRequest(ethernetFrame_t *frame) {
    // "I want to send an ethernet frame"
    error_t err;
    err.module = ERROR_MODULE_ETHERNET;
    err.code = ERROR_CODE_SUCCESSFUL;
    frame->memory = memory_txFrameRequest(frame->length);

    if (frame->memory.fOutOfMemory) {
        //There is no more memory left
        err.module = ERROR_MODULE_MEMORY;
        err.code = ERROR_MEMORY_OUT_OF_MEMORY;
        return err;
    }
    if (!frame->memory.fIsAssigned) {
        //the fIsAssigned flag should never be cleared if the fOutOfMemory flag is
        err.module = ERROR_MODULE_MEMORY;
        err.code = ERROR_MEMORY_UNKNOWN;
        return err;
    }

    ethernetController_writeDestinationMACAddress(frame->destination, frame->memory);
    ethernetController_writeEtherTypeFieldToBuffer(frame->ethertype, frame->memory);
    return err;
}

error_t ethernet_rxGetNewFrame(ethernetFrame_t *frame) {
    uint32_t static counter = 0;
    error_t err;
    err.module = ERROR_MODULE_ETHERNET;
    err.code = ERROR_CODE_SUCCESSFUL;
    frame->memory.start = ethernetController_getNextPacketPointer(); //Get the address of the packet that should be processed next

    ethernetController_updateNextPacketPointer();


    frame->memory.fIsAssigned = 1; //Not really needed, for completeness
    frame->receiveStatusVector = ethernetController_getRSV(frame->memory.start); //get the RSV for that packet
    frame->memory.length = frame->receiveStatusVector.length;
   
    //Now that we have the packets length we can work out where it ends
    if (frame->memory.start + frame->memory.length > END_OF_MEMORY_ADDRESS) {//Does it wrap around?
        //Work out the wrapped-around address
        frame->memory.end = frame->memory.length - ((END_OF_MEMORY_ADDRESS - RX_DATA_START_ADDRESS) - frame->memory.start) + RX_DATA_START_ADDRESS;
    } else {//Packet does not wrap around in memory
        frame->memory.end = frame->memory.start + frame->memory.length;
    }
    frame->ethertype = ethernetController_getEtherTypeField(frame->memory);

    UARTTransmitText("[");
    UARTTransmitText(intToString(counter++));
    UARTTransmitText("]");

    if (frame->receiveStatusVector.broadcast)
        UARTTransmitText("[Broadcast]");
    if (frame->receiveStatusVector.unicast)
        UARTTransmitText("[Unicast]");
    if (!frame->receiveStatusVector.receivedOk)
        UARTTransmitText("[Symbol Errors]");

    switch (frame->ethertype) {
        case ETHERTYPE_ARP:
            UARTTransmitText("[ARP]");
            ARP_handleNewPacket(frame);
            break;
        case ETHERTYPE_FLOW_CONTROL:
            break;
        case ETHERTYPE_IPv4:
            UARTTransmitText("[IPv4]");
            ipv4_handleNewPacket(frame);
            break;
        case ETHERTYPE_WOL:
            break;
        default:
            UARTTransmitText("[");
            UARTTransmitText(macToString(frame->source));
            UARTTransmitText("->");
            UARTTransmitText(macToString(frame->destination));
            UARTTransmitText("][");
            UARTTransmitText(etherTypeToString(frame->ethertype));
            UARTTransmitText("][L=");
            UARTTransmitInt(frame->length);
            UARTTransmitText("]");
            ethernetController_dropPacket(frame);
            break;
            return err;
    }

    UARTTransmitText("[");
    UARTTransmitText(intToString(frame->memory.start));
    UARTTransmitText(" to ");
    UARTTransmitText(intToString(frame->memory.end));
    UARTTransmitText(" (");
    UARTTransmitText(intToString(frame->memory.length));
    UARTTransmitText(")]");


    UARTTransmitText("\n\r");
}





