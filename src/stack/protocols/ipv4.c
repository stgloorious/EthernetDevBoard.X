/**
 * \file ipv4.c
 * \brief Function definitions for IPv4
 * \author Stefan Gloor
 * \version 1.0
 * \date 29. Januar 2019
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

#include "ipv4.h"

static ipv4_address_t sourceIPAddress;
static ipv4_address_t destinationIPAddress;

void ipv4_txFrameRequest(ipv4_packet_t *packet) {
    packet->ethernet.length = packet->ipv4Header.totalLength + 8;
    packet->ethernet.ethertype = ETHERTYPE_IPv4;
    ethernet_txFrameRequest(&packet->ethernet);

    packet->memory.fIsAssigned = packet->ethernet.memory.fIsAssigned;
    packet->memory.fOutOfMemory = packet->ethernet.memory.fOutOfMemory;
    packet->memory.index = packet->ethernet.memory.index;
    packet->memory.start = packet->ethernet.memory.start + 8;
    packet->memory.end = packet->ethernet.memory.end;
    packet->memory.length = packet->ethernet.memory.length - 8;
}

error_t ipv4_sendFrame(ipv4_packet_t ipPacket) {
    error_t err;
    err.module = ERROR_MODULE_ARP;
    uint8_t index;
    uint8_t static requestCounter = 0;
    uint8_t const numberOfRequests = 3;
    uint8_t const requestTimeout = 2;
    uint32_t static oldTime = 0;

    //First let's check the ARP table for a valid entry
    if (ARP_checkForEntry(ipPacket.ipv4Header.destination, &index)) {
        //There is a non-expired entry of the IP address
        ipPacket.ethernet.destination = ARP_getEntryFromTable(index);
        UARTTransmitText(ipAdressToString(ipPacket.ipv4Header.destination));
        UARTTransmitText(" is at ");
        UARTTransmitText(macToString(ipPacket.ethernet.destination));
        UARTTransmitText(".\n\r");
        //The source address is always stored in the Ethernet controller:
        ipPacket.ethernet.source = ethernetController_getMacAddress(); 
        //Fill the new information in memory and send the packet
        ethernetController_writeDestinationMACAddress(ipPacket.ethernet.destination, ipPacket.ethernet.memory);
        ethernetController_sendPacket(ipPacket.ethernet.memory);
        err.code = ERROR_CODE_SUCCESSFUL;
        return err;
    } else {
        //There was no valid entry in the ARP table, we have to send some requests
        if (getSeconds() - oldTime >= requestTimeout) {//Wait between two requests
            oldTime = getSeconds();
            if (requestCounter < numberOfRequests) {//Send only a limited number of requests and then give up
                requestCounter++;
                ARP_sendRequest(ipPacket.ipv4Header.destination);
            } else {
                //We got no answer to our requests
                requestCounter = 0;
                err.code = ERROR_ARP_MAXIMUM_NUMBER_OF_REQUESTS_REACHED;
                return err;
            }
        }
    }
    err.code = ERROR_ARP_WAITING_FOR_REPLY;
    return err;
}

void ipv4_streamToTransmissionBuffer(uint8_t data, ipv4_packet_t packet) {
    uint16_t ethernetPacketLength = packet.ipv4Header.totalLength;
    uint16_t remainingLength;

    /**\todo Fragmenting */

    uint16_t static dataPointer = 0;
    //ethernetController_streamToTransmitBuffer(data, ethernetPacketLength);
    if (dataPointer == ethernetPacketLength - 1) {
        dataPointer = 0;
    } else {
        dataPointer++;
    }
}

void ipv4_calculateHeaderChecksum(ipv4_header_t * header) {
    header->headerLength = 5;
    header->dscp = 0;
    header->ecn = 0;
    header->flags = 0x00;
    header->fragmentOffset = 0x00;
    header->identification = 0x00;
    //Sum up all 16-Bit words of the header:
    uint32_t sum = 0;
    sum = ((header->version << 12) | (header->headerLength << 8) | (header->dscp << 2) | (header->ecn))&0xffff; //Word 0
    sum += header->totalLength; //Word 1
    sum += header->identification; //Word 2
    sum += ((header->flags << 13) | (header->fragmentOffset))&0xffff; //Word 3
    sum += ((header->timeToLive << 8) | (header->protocol))&0xffff; //Word 4
    //header checksum is zero for calculation
    sum += ((header->source.address[1] | (header->source.address[0] << 8)))&0xffff; // Word 5
    sum += ((header->source.address[3] | (header->source.address[2] << 8)))&0xffff; // Word 6
    sum += ((header->destination.address[1] | (header->destination.address[0] << 8)))&0xffff; // Word 7
    sum += ((header->destination.address[3] | (header->destination.address[2] << 8)))&0xffff; // Word 8

    sum = (sum & 0x0ffff)+((sum & 0xf0000) >> 16);
    sum = ~sum; //invert all bits

    header->checksum = sum;
}

void ipv4_writeHeaderIntoBuffer(ipv4_header_t header, uint8_t* ptr) {
    *ptr = (header.version << 4) | (header.headerLength);
    *(ptr + 1) = (header.dscp << 2) | (header.ecn & 0x03);
    *(ptr + 2) = (header.totalLength & 0xff00) >> 8;
    *(ptr + 3) = header.totalLength & 0x00ff;
    *(ptr + 4) = (header.identification & 0xff00) >> 8;
    *(ptr + 5) = header.identification & 0x00ff;
    *(ptr + 6) = (header.flags << 5) | ((header.fragmentOffset & 0x1f00) >> 8);
    *(ptr + 7) = (header.fragmentOffset & 0x00ff);
    *(ptr + 8) = (header.timeToLive);
    *(ptr + 9) = (header.protocol);
    *(ptr + 10) = (header.checksum & 0xff00) >> 8;
    *(ptr + 11) = (header.checksum & 0x00ff);
    *(ptr + 12) = (header.source.address[0]);
    *(ptr + 13) = (header.source.address[1]);
    *(ptr + 14) = (header.source.address[2]);
    *(ptr + 15) = (header.source.address[3]);
    *(ptr + 16) = (header.destination.address[0]);
    *(ptr + 17) = (header.destination.address[1]);
    *(ptr + 18) = (header.destination.address[2]);
    *(ptr + 19) = (header.destination.address[3]);
}

void ipv4_handleNewPacket(ethernetFrame_t *frame) {
    UARTTransmitText("[PACKET DROPPED]");
    ethernetController_dropPacket(frame);
    /**
     * \todo ipv4 packet reception
     */
}

/*void ipv4_setIPSourceAddress(IPv4Address newSourceAddress) {
    sourceIPAddress = newSourceAddress;
}*/

/*void ipv4_setIPDestinationAddress(IPv4Address newDestinationAddress) {
    destinationIPAddress = newDestinationAddress;
}

IPv4Address ipv4_getIPSourceAddress() {
    return sourceIPAddress;
}

IPv4Address ipv4_getIPDestinationAddress() {
    return destinationIPAddress;
}*/

bool_t ipv4_cmp(ipv4_address_t* a, ipv4_address_t * b) {
    for (uint8_t i = 0; i < 4; i++) {
        if (a->address[i] != b->address[i])
            return 0;
    }
    return 1;
}

bool_t ipv4_isAllZero(ipv4_address_t * ip) {
    for (uint8_t i = 0; i < 4; i++) {
        if (ip->address[i] != 0x00)
            return 0;
    }
    return 1;
}

void ipv4_setToAllZero(ipv4_address_t * ip) {
    for (uint8_t i = 0; i < 4; i++) {
        ip->address[0] = 0x00;
    }
}


