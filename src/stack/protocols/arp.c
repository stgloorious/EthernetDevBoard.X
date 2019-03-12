/** 
 * \file arp.c
 * \brief Function definitions for IPv4
 * \author Stefan Gloor
 * \version 1.0
 * \date 1. Februar 2019
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

#include <stdlib.h>
#include "arp.h"

ARP_tableEntry_t static ARP_table [ARP_TABLE_LENGTH];

/* =======================  Receive  ======================= */

void ARP_handleNewPacket(ethernetFrame_t *frame) {
    ARP_message_t arp;
    if (frame->ethertype != ETHERTYPE_ARP) {
        ethernetController_dropPacket(frame);
        return;
    }

    arp = ARP_parseFromRXBuffer(frame);
    if (arp.err.code != ERROR_CODE_SUCCESSFUL) {
        ethernetController_dropPacket(frame);
        UARTTransmitText("[Invalid ARP Message was discarded.]\r\n");
        return;
    }
    UARTTransmitText("[");
    UARTTransmitText(macToString(arp.senderMACAddress));
    UARTTransmitText(", ");
    UARTTransmitText(ipAdressToString(arp.senderIPAddress));
    UARTTransmitText("]->[");
    UARTTransmitText(macToString(arp.targetMACAddress));
    UARTTransmitText(", ");
    UARTTransmitText(ipAdressToString(arp.targetIPAddress));
    UARTTransmitText("]");
    if (arp.fIsProbe)
        UARTTransmitText("[IsProbe]");
    if (arp.fIsGratuitous)
        UARTTransmitText("[IsGratuitous]");

    ARP_sendReply(arp);
    ARP_setNewEntry(arp.senderMACAddress, arp.senderIPAddress, getMillis());
    /**
     * \todo where does the tail pointer get updated
     */
}

ARP_message_t static ARP_parseFromRXBuffer(ethernetFrame_t *frame) {
    ARP_message_t arp;
    uint16_t const offset = 22; //skip NextPacketPointer (2 Bytes), RSV (6 Bytes), 2 MAC addresses (12 Bytes) and EtherType (2 Bytes)

    ethernetController_streamFromRXBuffer(0, frame->memory.start + offset); //Prepare stream

    //Word 0
    arp.htype = (ethernetController_streamFromRXBuffer(1, frame->memory.start + offset) << 8);
    arp.htype |= ethernetController_streamFromRXBuffer(1, frame->memory.start + offset);
    //Word 1
    arp.ptype = (ethernetController_streamFromRXBuffer(1, frame->memory.start + offset) << 8);
    arp.ptype |= ethernetController_streamFromRXBuffer(1, frame->memory.start + offset);
    //Word 2
    arp.hlen = ethernetController_streamFromRXBuffer(1, frame->memory.start + offset);
    arp.plen = ethernetController_streamFromRXBuffer(1, frame->memory.start + offset);
    //Word 3
    arp.operation = (ethernetController_streamFromRXBuffer(1, frame->memory.start + offset) << 8);
    arp.operation |= ethernetController_streamFromRXBuffer(1, frame->memory.start + offset);

    // Sender Addresses
    for (uint8_t i = 0; i < arp.hlen; i++)
        arp.senderMACAddress.address[i] = ethernetController_streamFromRXBuffer(1, frame->memory.start + offset);
    for (uint8_t i = 0; i < arp.plen; i++)
        arp.senderIPAddress.address[i] = ethernetController_streamFromRXBuffer(1, frame->memory.start + offset);
    //Target Addresses
    for (uint8_t i = 0; i < arp.hlen; i++)
        arp.targetMACAddress.address[i] = ethernetController_streamFromRXBuffer(1, frame->memory.start + offset);
    for (uint8_t i = 0; i < arp.plen; i++)
        arp.targetIPAddress.address[i] = ethernetController_streamFromRXBuffer(1, frame->memory.start + offset);

    ethernetController_streamFromRXBuffer(2, frame->memory.start + offset); //Close stream

    if (ipv4_isAllZero(&arp.senderIPAddress)) {//Is it an ARP Probe?
        arp.fIsProbe = 1;
        arp.fIsGratuitous = 0;
    } else {//If not it may be gratuitous
        arp.fIsProbe = 0;
        //Is SPA=TPA and THA set to zero in a ARP request ?
        if (ipv4_cmp(&arp.senderIPAddress, &arp.targetIPAddress) &&
                mac_isAllZero(&arp.targetMACAddress) &&
                arp.operation == ARP_REQUEST) {
            arp.fIsGratuitous = 1;
        } else {
            //Is SPA=TPA and THA=SHA in a ARP reply ?
            if (ipv4_cmp(&arp.senderIPAddress, &arp.targetIPAddress) &&
                    mac_cmp(&arp.senderMACAddress, &arp.targetMACAddress) &&
                    arp.operation == ARP_REPLY) {
                arp.fIsGratuitous = 1;
            } else {
                arp.fIsGratuitous = 0;
            }
        }
    }

    //was there anything unexpected?
    if ((arp.htype != ARP_HTYPE_ETHERNET) || //only Ethernet is supported
            (arp.ptype != ARP_PTYPE_IPv4) || //only IPv4
            (arp.operation > 2) || //There is nothing else than REQUEST (1) and REPLY (2)
            (arp.hlen != ARP_ETHERNET_HLEN) ||
            (arp.plen != ARP_IPv4_PLEN)) {
        arp.err.code = ERROR_ARP_INVALID_VALUES;
        arp.err.module = ERROR_MODULE_ARP;
    } else {
        arp.err.code = ERROR_CODE_SUCCESSFUL;
        arp.err.module = ERROR_MODULE_ARP;
    }

    return arp;
}

/* =======================  Transmit  ======================= */

void static ARP_send(ARP_message_t arp) {
    memoryField_t field;
    macaddress_t destination;
    ethernetFrame_t ethFrame;
    mac_setToBroadcast(&destination);

    ethFrame.length = 36;
    ethFrame.destination = arp.targetMACAddress;
    ethFrame.source = ethernetController_getSourceMACAddress(field);
    ethFrame.ethertype = ETHERTYPE_ARP;

    ethernet_txFrameRequest(&ethFrame);

    if (ethFrame.memory.fOutOfMemory) {
        return;
    }

    field.start = ethFrame.memory.start + 8;
    field.end = ethFrame.memory.end;
    field.length = 28;

    UARTTransmitText(hexToString(field.start));
    UARTTransmitText(", ");
    UARTTransmitText(hexToString(field.end));
    UARTTransmitText("\n\r");


    ethernetController_streamToTransmitBuffer((arp.htype & 0xff00) >> 8, field);
    ethernetController_streamToTransmitBuffer(arp.htype & 0x00ff, field);
    ethernetController_streamToTransmitBuffer((arp.ptype & 0xff00) >> 8, field);
    ethernetController_streamToTransmitBuffer(arp.ptype & 0x00ff, field);
    ethernetController_streamToTransmitBuffer(arp.hlen, field);
    ethernetController_streamToTransmitBuffer(arp.plen, field);
    ethernetController_streamToTransmitBuffer((arp.operation & 0xff00) >> 8, field);
    ethernetController_streamToTransmitBuffer(arp.operation & 0x00ff, field);
    for (uint8_t i = 0; i < arp.hlen; i++)
        ethernetController_streamToTransmitBuffer(arp.senderMACAddress.address[i], field);
    for (uint8_t i = 0; i < arp.plen; i++)
        ethernetController_streamToTransmitBuffer(arp.senderIPAddress.address[i], field);
    for (uint8_t i = 0; i < arp.hlen; i++)
        ethernetController_streamToTransmitBuffer(arp.targetMACAddress.address[i], field);

    for (uint8_t i = 0; i < arp.plen; i++)
        ethernetController_streamToTransmitBuffer(arp.targetIPAddress.address[i], field);

    ethernetController_sendPacket(ethFrame.memory);
}

/* =======================  Messages  ======================= */
void ARP_sendRequest(ipv4_address_t ipSender, ipv4_address_t ipTarget) {
    ARP_message_t request;
    macaddress_t senderMAC;
    macaddress_t targetMAC;

    senderMAC = ethernetController_getMacAddress();
    //mac_setToBroadcast(&targetMAC);
    ipSender = ipv4_getIPSourceAddress();
    mac_setAllZero(&targetMAC);

    request.hlen = ARP_ETHERNET_HLEN;
    request.plen = ARP_IPv4_PLEN;
    request.htype = ARP_HTYPE_ETHERNET;
    request.ptype = ARP_PTYPE_IPv4;
    request.operation = ARP_REQUEST;
    request.senderIPAddress = ipSender;
    request.senderMACAddress = senderMAC;
    request.targetIPAddress = ipTarget;
    request.targetMACAddress = targetMAC;

    ARP_send(request);
}

void static ARP_sendReply(ARP_message_t request) {
    if (request.operation != ARP_REQUEST)//is it a request?
        return;

    ipv4_address_t myIP = ipv4_getIPSourceAddress();
    if (!(ipv4_cmp(&request.targetIPAddress, &myIP))) //For my IP Address?
        return;

    ARP_message_t reply;
    ipv4_address_t senderIP;
    ipv4_address_t targetIP;
    macaddress_t senderMAC;
    macaddress_t targetMAC;

    senderMAC = ethernetController_getMacAddress();
    targetMAC = request.senderMACAddress; //send it back
    senderIP = ipv4_getIPSourceAddress(); /** \todo this */
    targetIP = request.senderIPAddress; //send it back

    reply.hlen = ARP_ETHERNET_HLEN;
    reply.plen = ARP_IPv4_PLEN;
    reply.htype = ARP_HTYPE_ETHERNET;
    reply.ptype = ARP_PTYPE_IPv4;
    reply.operation = ARP_REPLY;
    reply.senderIPAddress = senderIP;
    reply.senderMACAddress = senderMAC;
    reply.targetIPAddress = targetIP;
    reply.targetMACAddress = targetMAC;

    ARP_send(reply);

    UARTTransmitText("[Reply sent to ");
    UARTTransmitText(macToString(reply.targetMACAddress));
    UARTTransmitText(", ");
    UARTTransmitText(ipAdressToString(reply.targetIPAddress));
    UARTTransmitText("]");
}

/* =======================  Collision Check  ======================= */
error_t ARP_probe(ipv4_address_t ipTarget) {
    uint8_t static state = 0;
    time_t timeStart;
    time_t waitingTime;
    ipv4_address_t ipSender;
    error_t err;
    err.module = ERROR_MODULE_ARP;

    switch (state) {
        case 0:
            ipv4_setToAllZero(&ipSender);
            timeStart = getMillis();
            waitingTime = rand() % 3000;
            state = 1;
            err.code = ERROR_ARP_WAITING_FOR_REPLY;
            break;
        case 1:
            if (getMillis() - timeStart >= waitingTime) {
                UARTTransmitText("Waited ");
                UARTTransmitText(intToString(waitingTime));
                UARTTransmitText(" ms.\n\r");
                ARP_sendRequest(ipSender, ipTarget);
                state = 2;
            }
            err.code = ERROR_ARP_WAITING_FOR_REPLY;
            break;
        case 2:
            state = 0;
            err.code = ERROR_CODE_SUCCESSFUL;
            break;
    }
    return err;
}

/* =======================  Table  ======================= */

uint8_t ARP_checkForEntry(ipv4_address_t ip, uint8_t *index) {
    for (uint8_t i = 0; i < ARP_TABLE_LENGTH; i++) {
        if (ipv4_cmp(&ARP_table[i].ip, &ip)) {
            if (getMillis() - ARP_table[i].timeCreated < ARP_TABLE_ENTRY_TTL) {//not expired?
                //if an entry is expired it is just ignored; old entries are overwritten anyways when writing new ones.
                *index = i; //save where the entry is
                return 1; //don't check further
            }
        }
    }
    return 0;
}

macaddress_t ARP_getEntryFromTable(uint8_t index) {
    return ARP_table[index].mac;
}

void static ARP_setNewEntry(macaddress_t mac, ipv4_address_t ip, uint32_t timestamp) {
    uint32_t maxSeconds = 0;
    uint8_t oldestIndex = 0;
    //Loop through table to find oldest entry
    for (uint8_t i = 0; i < ARP_TABLE_LENGTH; i++) {
        if (ARP_table[i].timeCreated > maxSeconds) {
            maxSeconds = ARP_table[i].timeCreated;
            oldestIndex = i;
        }
    }
    //Create new entry, replacing the oldest one
    ARP_table[oldestIndex].ip = ip;
    ARP_table[oldestIndex].mac = mac;
    ARP_table[oldestIndex].timeCreated = timestamp;
}

void ARP_initTable() {
    macaddress_t mac;
    ipv4_address_t ip;
    mac_setAllZero(&mac);
    ipv4_setToAllZero(&ip);
    for (uint8_t i = 0; i < ARP_TABLE_LENGTH; i++)
        ARP_setNewEntry(mac, ip, 0xffffffff); //timestamp is set to maximum
}

void printArpTable() {
    for (uint8_t i = 0; i < ARP_TABLE_LENGTH; i++) {
        UARTTransmitText(arpEntryToString(ARP_table[i]));
        UARTTransmitText("\n\r");
    }
}
