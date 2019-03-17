/** 
 * \file arp.c
 * \brief Function definitions for IPv4
 * \author Stefan Gloor
 * \version 1.0
 * \date 1. Februar 2019
 * \ingroup arp
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


arp_tableEntry_t static arp_table [ARP_TABLE_LENGTH];

/* =======================  Receive  ======================= */

void arp_handleNewPacket(ethernetFrame_t *frame) {
    arp_message_t arp;
    if (frame->ethertype != ETHERTYPE_ARP) {
        ethernetController_dropPacket(frame);
        return;
    }

    arp = arp_parseFromRXBuffer(frame);
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

    arp_sendReply(arp);
    arp_setNewEntry(arp.senderMACAddress, arp.senderIPAddress, getMillis());
}

arp_message_t static arp_parseFromRXBuffer(ethernetFrame_t *frame) {
    arp_message_t arp;
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

void static arp_send(arp_message_t arp) {
    memoryField_t field;
    ethernetFrame_t ethFrame;

#if ARP_LINK_BROADCAST == true
    macaddress_t destination;
    mac_setToBroadcast(&destination);
    ethFrame.destination = destination; //broadcast address
#else
    ethFrame.destination = arp.targetMACAddress;
#endif
    ethFrame.length = 36;
    ethFrame.source = ethernetController_getSourceMACAddress(field);
    ethFrame.ethertype = ETHERTYPE_ARP;

    ethernet_txFrameRequest(&ethFrame);

    if (ethFrame.memory.fOutOfMemory) {
        return;
    }

    field.start = ethFrame.memory.start + 8;
    field.end = ethFrame.memory.end;
    field.length = 28;

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
void static arp_sendRequest(ipv4_address_t ipSender, ipv4_address_t ipTarget) {
    arp_message_t request;
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

    arp_send(request);
}

void static arp_sendReply(arp_message_t request) {
    if (request.operation != ARP_REQUEST)//is it a request?
        return;

    ipv4_address_t myIP = ipv4_getIPSourceAddress();
    if (!(ipv4_cmp(&request.targetIPAddress, &myIP))) //For my IP Address?
        return;

    arp_message_t reply;
    ipv4_address_t senderIP;
    ipv4_address_t targetIP;
    macaddress_t senderMAC;
    macaddress_t targetMAC;

    senderMAC = ethernetController_getMacAddress();
    targetMAC = request.senderMACAddress; //send it back
    senderIP = ipv4_getIPSourceAddress();
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

    arp_send(reply);

    UARTTransmitText("[Reply sent to ");
    UARTTransmitText(macToString(reply.targetMACAddress));
    UARTTransmitText(", ");
    UARTTransmitText(ipAdressToString(reply.targetIPAddress));
    UARTTransmitText("]");
}

/* =======================  Collision Check  ======================= */
error_t static arp_probe(ipv4_address_t ipTarget) {
    time_t timeStart; // Used to calculate the time passed
    time_t waitingTime; // Specifies the number of milliseconds which have to pass before doing something
    ipv4_address_t ipSender;
    uint8_t static probeCounter = 0; //counts how many probes already have been sent
    error_t err;

    enum states {
        PREPARING_PROBE, WAITING_TO_SEND, SENDING_PROBE, ARP_ENTRY_FOUND
    };
    uint8_t static state = PREPARING_PROBE;
    uint8_t index;
    err.module = ERROR_MODULE_ARP;

    if (arp_checkForEntry(ipTarget, &index)) {
        UARTTransmitText("ARP: Found a valid entry for ");
        UARTTransmitText(ipAdressToString(ipTarget));
        UARTTransmitText(".\n\r");
        state = ARP_ENTRY_FOUND;
    }

    switch (state) {//State Machine
        case PREPARING_PROBE:
            UARTTransmitText("ARP: Preparing probe...\n\r");
            ipv4_setToAllZero(&ipSender); //"The 'sender IP address' field MUST be set to all zeroes" - RFC 5227
            timeStart = getMillis(); //keep track when we started counting the waitingTime
            waitingTime = rand() % ARP_PROBE_WAIT; //"[...] then wait for a random time interval selected uniformly in the range zero to PROBE_WAIT" - RFC 5227
            state = WAITING_TO_SEND;
            err.code = ERROR_ARP_WAITING_FOR_REPLY;
            UARTTransmitText("ARP: Waiting to send probe.\n\r");
            break;
        case WAITING_TO_SEND:
            if (getMillis() - timeStart >= waitingTime) {//Are we done waiting for the first probe?
                UARTTransmitText("ARP: Waited ");
                UARTTransmitText(intToString(waitingTime));
                UARTTransmitText(" ms.\n\r");
                state = SENDING_PROBE;
                //"[...] each of these probe packets spaced randomly and uniformly, PROBE_MIN to PROBE_MAX seconds apart." - RFC 5227
                waitingTime = (rand() % (ARP_PROBE_MAX - ARP_PROBE_MIN)) + ARP_PROBE_MIN;

                UARTTransmitText("ARP: Waiting time between the probes is ");
                UARTTransmitText(intToString(waitingTime));
                UARTTransmitText(" ms.\n\r");
            }
            err.code = ERROR_ARP_WAITING_FOR_REPLY;
            break;
        case SENDING_PROBE:
            if (probeCounter == 0 || (getMillis() - timeStart) > waitingTime) {
                timeStart = getMillis(); //remember time the last packet was sent

                if (probeCounter < ARP_PROBE_NUM) {
                    arp_sendRequest(ipSender, ipTarget);
                    UARTTransmitText("ARP: Sending probe #");
                    UARTTransmitText(intToString(probeCounter));
                    UARTTransmitText(": Who has ");
                    UARTTransmitText(ipAdressToString(ipv4_getPreliminaryIPSourceAddress()));
                    UARTTransmitText("?\n\r");
                }
                if (probeCounter >= ARP_PROBE_NUM) {//RFC suggests to wait *after* the transmission
                    probeCounter = 0;
                    state = PREPARING_PROBE;
                    err.code = ERROR_ARP_MAXIMUM_NUMBER_OF_PROBES_REACHED; //That would be the "normal" case; the probing was negative
                } else {
                    err.code = ERROR_ARP_WAITING_FOR_REPLY; //there are more probes to send
                    probeCounter++;
                }

            }
            break;
        case ARP_ENTRY_FOUND:
            probeCounter = 0;
            state = PREPARING_PROBE;
            err.code = ERROR_ARP_IPv4_ADDRESS_CONFLICT;
    }
    return err;
}

/* =======================  Background work ======================= */

error_t arp_background() {
    error_t static err;
    err.code = ERROR_CODE_UNDEFINED;
    err.module = ERROR_MODULE_ARP;

    ipv4_address_t ipPreliminary = ipv4_getPreliminaryIPSourceAddress();

    if (ipv4_SrcAddrChanged(&err)) {//The IPv4 module wants to use a different source address than before
        /*   UARTTransmitText("ARP: IPv4 Src Addr should be changed to ");
           UARTTransmitText(ipAdressToString(ipPreliminary));
           UARTTransmitText(".\n\r");*/

        err.code = arp_probe(ipPreliminary).code;

        switch (err.code) {
            case ERROR_ARP_IPv4_ADDRESS_CONFLICT:
                UARTTransmitText("ARP: IPv4 address already in use!\n\r");
                break;
            case ERROR_ARP_MAXIMUM_NUMBER_OF_PROBES_REACHED:
                UARTTransmitText("ARP: Nobody answered.\n\r");
                break;
            case ERROR_ARP_WAITING_FOR_REPLY:
                break;
            default:
                UARTTransmitText("ARP: An unknown error occured.\n\r");
        }
    }
    return err;
}

/* =======================  Table  ======================= */

uint8_t static arp_checkForEntry(ipv4_address_t ip, uint8_t * index) {
    for (uint8_t i = 0; i < ARP_TABLE_LENGTH; i++) {
        if (ipv4_cmp(&arp_table[i].ip, &ip)) {
            if (getMillis() - arp_table[i].timeCreated < ARP_TABLE_ENTRY_TTL) {//not expired?
                //if an entry is expired it is just ignored; old entries are overwritten anyways when writing new ones.
                *index = i; //save where the entry is

                return 1; //don't check further
            }
        }
    }
    return 0;
}

macaddress_t static arp_getEntryFromTable(uint8_t index) {
    return arp_table[index].mac;
}

void static arp_setNewEntry(macaddress_t mac, ipv4_address_t ip, uint32_t timestamp) {
    uint32_t maxSeconds = 0;
    uint8_t oldestIndex = 0;
    //Loop through table to find oldest entry
    for (uint8_t i = 0; i < ARP_TABLE_LENGTH; i++) {
        if (arp_table[i].timeCreated > maxSeconds) {

            maxSeconds = arp_table[i].timeCreated;
            oldestIndex = i;
        }
    }
    //Create new entry, replacing the oldest one
    arp_table[oldestIndex].ip = ip;
    arp_table[oldestIndex].mac = mac;
    arp_table[oldestIndex].timeCreated = timestamp;
}

void arp_init() {
    macaddress_t mac;
    ipv4_address_t ip;
    mac_setAllZero(&mac);
    ipv4_setToAllZero(&ip);

    for (uint8_t i = 0; i < ARP_TABLE_LENGTH; i++)
        arp_setNewEntry(mac, ip, 0xffffffff); //timestamp is set to maximum
}

void printArpTable() {
    for (uint8_t i = 0; i < ARP_TABLE_LENGTH; i++) {
        UARTTransmitText(arpEntryToString(arp_table[i]));
        UARTTransmitText("\n\r");
    }
}