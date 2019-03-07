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
    time_t const requestTimeout = 2000;
    time_t static oldTime = 0;

    //First let's check the ARP table for a valid entry
    if (ARP_checkForEntry(ipPacket.ipv4Header.destination, &index)) {
        //There is a non-expired entry of the IP address
        ipPacket.ethernet.destination = ARP_getEntryFromTable(index);
        //The source address is always stored in the Ethernet controller:
        ipPacket.ethernet.source = ethernetController_getMacAddress();
        //Fill the new information in memory and send the packet
        ethernetController_writeDestinationMACAddress(ipPacket.ethernet.destination, ipPacket.ethernet.memory);
        ethernetController_sendPacket(ipPacket.ethernet.memory);
        err.code = ERROR_CODE_SUCCESSFUL;
        return err;
    } else {
        //There was no valid entry in the ARP table, we have to send some requests
        if (getMillis() - oldTime >= requestTimeout) {//Wait between two requests
            oldTime = getMillis();
            if (requestCounter < numberOfRequests) {//Send only a limited number of requests and then give up
                requestCounter++;
                ARP_sendRequest(ipv4_getIPSourceAddress(), ipPacket.ipv4Header.destination);
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

    ethernetController_streamToTransmitBuffer(data, packet.memory);
}

void ipv4_calculateHeaderChecksum(ipv4_header_t * header) {
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
    ipv4_header_t ip;
    memoryField_t headerField;
    if (frame->ethertype != ETHERTYPE_IPv4) {
        ethernetController_dropPacket(frame);
        return;
    }
    headerField = frame->memory;
    headerField.start = frame->memory.start + 22; //skip NextPacketPointer (2 Bytes), RSV (6 Bytes), 2 MAC addresses (12 Bytes) and EtherType (2 Bytes)

    ip = ipv4_parseHeader(&headerField);
    UARTTransmitText("[");
    UARTTransmitText(ipAdressToString(ip.source));
    UARTTransmitText(" -> ");
    UARTTransmitText(ipAdressToString(ip.destination));
    UARTTransmitText("][");
    UARTTransmitText((ipProtocolToString(ip.protocol)));
    UARTTransmitText("][");
    UARTTransmitText(intToString(ip.totalLength));
    UARTTransmitText("][");
    UARTTransmitText(intToString(ip.version));
    UARTTransmitText(", ");
    UARTTransmitText(intToString(ip.headerLength));
    UARTTransmitText("]");
    if (!ipv4_checkHeaderChecksum(&ip))
        UARTTransmitText("[INVALID CHECKSUM]");


    ethernetController_dropPacket(frame);
}

ipv4_header_t static ipv4_parseHeader(memoryField_t *field) {
    ipv4_header_t ip;
    uint8_t temp;

    ethernetController_streamFromRXBuffer(0, field->start); //prepare stream
    //BYTE 0
    temp = ethernetController_streamFromRXBuffer(1, field->start);
    ip.version = (temp & 0xf0) >> 4;
    ip.headerLength = temp & 0x0f;
    //BYTE 1
    temp = ethernetController_streamFromRXBuffer(1, field->start);
    ip.dscp = (temp & 0x1f);
    ip.ecn = (temp & 0xC0) >> 6;
    //BYTE 2 & 3
    ip.totalLength = (ethernetController_streamFromRXBuffer(1, field->start) << 8);
    ip.totalLength |= ethernetController_streamFromRXBuffer(1, field->start);
    //BYTE 4 & 5
    ip.identification = (ethernetController_streamFromRXBuffer(1, field->start) << 8);
    ip.identification |= ethernetController_streamFromRXBuffer(1, field->start);
    //BYTE 6
    temp = ethernetController_streamFromRXBuffer(1, field->start);
    ip.flags = ((temp & 0xC0) >> 6);
    ip.fragmentOffset = ((temp & 0x1f) << 8);
    //BYTE 7
    ip.fragmentOffset |= ethernetController_streamFromRXBuffer(1, field->start);
    //BYTE 8
    ip.timeToLive = ethernetController_streamFromRXBuffer(1, field->start);
    //BYTE 9
    ip.protocol = ethernetController_streamFromRXBuffer(1, field->start);
    //BYTE 10 & 11
    ip.checksum = (ethernetController_streamFromRXBuffer(1, field->start) << 8);
    ip.checksum |= ethernetController_streamFromRXBuffer(1, field->start);
    //BYTE 12..15
    for (uint8_t i = 0; i < 4; i++)//Number of bytes in an ip address
        ip.source.address[i] = ethernetController_streamFromRXBuffer(1, field->start);
    //BYTE 16..19
    for (uint8_t i = 0; i < 4; i++)
        ip.destination.address[i] = ethernetController_streamFromRXBuffer(1, field->start);

    ethernetController_streamFromRXBuffer(2, field->start); //end stream

    return ip;
}

bool_t static ipv4_checkHeaderChecksum(ipv4_header_t *header) {
    ipv4_header_t testHeader;
    testHeader = *header;
    testHeader.checksum = 0x0000;
    ipv4_calculateHeaderChecksum(&testHeader);
    if (testHeader.checksum == header->checksum) {
        return true;
    }
    UARTTransmitText("[Expected ");
    UARTTransmitText(hexToString(testHeader.checksum));
    UARTTransmitText(", got ");
    UARTTransmitText(hexToString(header->checksum));
    UARTTransmitText("]");
    return false;
}

ipv4_address_t ipv4_getIPSourceAddress() {
    return sourceIPAddress;
}

error_t ipv4_setIPSourceAddress(ipv4_address_t ip) {
    uint8_t static state = 0;
    uint8_t const numberOfProbes = 3; //how many times an arp probe is sent
    uint8_t const timeBetweenProbes = 1; //seconds between arp probes
    uint8_t static probeCounter = 0;
    uint8_t index;
    error_t err;
    err.module = ERROR_MODULE_IPv4;
    switch (state) {
        case 0://Check the ARP table 
            if (ARP_checkForEntry(ip, &index)) {
                if (probeCounter == 0)//Was an arp probe sent?
                    //The address is already in the arp table
                    err.code = ERROR_IPv4_ADDRESS_ALREADY_IN_USE;
                else
                    //The address was just added to the arp table
                    err.code = ERROR_IPv4_ADDRESS_CONFLICT_DETECTED;
                return err;
            }
            state = 1;
            break;
        case 1:
            sourceIPAddress = ip;
            break;
    }

}

error_t ipv4_checkForConflicts(ipv4_address_t ip) {

}

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


