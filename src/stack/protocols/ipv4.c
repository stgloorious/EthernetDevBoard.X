/**
 * \file ipv4.c
 * \brief Function definitions for IPv4
 * \author Stefan Gloor
 * \version 1.0
 * \date 29. Januar 2019
 * \ingroup ipv4
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

#include "../../system/system.h"

#include "ipv4.h"
#include "arpSettings.h"
#include "../bool.h"
#include "ipv4Settings.h"
#include "icmp.h"

ipv4_address_t static ipSource; ///< Source IP address of the stack
ipv4_address_t static ipPreliminarySource; ///< Future source IP address of the stack (needs to be verified by collision detection first)
bool_t static fSrcAddrChanged; ///< used as an indicator when to check for collisions
error_t *arpStatus;

/* =======================  Transmit packet ======================= */

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
    err.code = ERROR_ARP_WAITING;
    uint8_t index;
    uint8_t static failedResolveCounter = 0;
    time_t static oldTime = 0;

    /* ========  Resolve address ======== */
    //First let's check the ARP table for a valid entry
    if (arp_checkForEntry(ipPacket.ipv4Header.destination, &index)) {
        //There is a non-expired entry of the IP address
        ipPacket.ethernet.destination = arp_getEntryFromTable(index);
#if IPv4_DEBUG_MESSAGES==true
        UARTTransmitText("[IPv4]: ");
        UARTTransmitText(ipAdressToString(ipPacket.ipv4Header.destination));
        UARTTransmitText(" is at ");
        UARTTransmitText(macToString(ipPacket.ethernet.destination));
        UARTTransmitText("\n\r");
#endif
        //The source address is always stored in the Ethernet controller:
        ipPacket.ethernet.source = ethernetController_getMacAddress();
        //Fill the new information in memory and send the packet
        ethernetController_writeDestinationMACAddress(ipPacket.ethernet.destination, ipPacket.ethernet.memory);
        ethernetController_sendPacket(ipPacket.ethernet.memory);
        err.code = ERROR_CODE_SUCCESSFUL;
        oldTime = 0;
        failedResolveCounter = 0;
    } else if ((getMillis() - oldTime) > ARP_REQUEST_WAIT || oldTime == 0) {
        failedResolveCounter++;
#if IPv4_DEBUG_MESSAGES==true
        UARTTransmitText("[IPv4]: Trying to resolve ");
        UARTTransmitText(ipAdressToString(ipPacket.ipv4Header.destination));
        UARTTransmitText("\n\r");
#endif
        oldTime = getMillis();
        arp_sendRequest(ipPacket.ipv4Header.source, ipPacket.ipv4Header.destination);
        if (failedResolveCounter >= ARP_REQUEST_NUM) {
            failedResolveCounter = 0;
            err.code = ERROR_ARP_MAXIMUM_NUMBER_OF_REQUESTS_REACHED;
        }
    }
    return err;
}

void ipv4_streamToTransmissionBuffer(uint8_t data, ipv4_packet_t packet) {
    ethernetController_streamToTransmitBuffer(data, packet.memory);
}

/* =======================  Header ======================= */

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

void ipv4_calculateHeaderChecksum(ipv4_header_t * header) {
    /**
     * \todo make it look less ugly
     */
    //Sum up all 16-Bit words of the header:
    uint32_t sum = 0;
    uint8_t carry = 0;
    sum = ((header->version << 12) | (header->headerLength << 8) | (header->dscp << 2) | (header->ecn))&0xffff; //Word 0
    if (sum > 0xffff) {//If there is a carry bit, add it to the sum (to keep it within 16 bits)
        sum = sum & 0xffff;
        sum++;
    }
    sum += header->totalLength; //Word 1
    if (sum > 0xffff) {//If there is a carry bit, add it to the sum (to keep it within 16 bits)
        sum = sum & 0xffff;
        sum++;
    }
    sum += header->identification; //Word 2
    if (sum > 0xffff) {//If there is a carry bit, add it to the sum (to keep it within 16 bits)
        sum = sum & 0xffff;
        sum++;
    }
    sum += ((header->flags << 13) | (header->fragmentOffset))&0xffff; //Word 3
    if (sum > 0xffff) {//If there is a carry bit, add it to the sum (to keep it within 16 bits)
        sum = sum & 0xffff;
        sum++;
    }
    sum += ((header->timeToLive << 8) | (header->protocol))&0xffff; //Word 4
    if (sum > 0xffff) {//If there is a carry bit, add it to the sum (to keep it within 16 bits)
        sum = sum & 0xffff;
        sum++;
    }
    //header checksum is zero for calculation
    sum += ((header->source.address[1] | (header->source.address[0] << 8)))&0xffff; // Word 5
    if (sum > 0xffff) {//If there is a carry bit, add it to the sum (to keep it within 16 bits)
        sum = sum & 0xffff;
        sum++;
    }
    sum += ((header->source.address[3] | (header->source.address[2] << 8)))&0xffff; // Word 6
    if (sum > 0xffff) {//If there is a carry bit, add it to the sum (to keep it within 16 bits)
        sum = sum & 0xffff;
        sum++;
    }
    sum += ((header->destination.address[1] | (header->destination.address[0] << 8)))&0xffff; // Word 7
    if (sum > 0xffff) {//If there is a carry bit, add it to the sum (to keep it within 16 bits)
        sum = sum & 0xffff;
        sum++;
    }
    sum += ((header->destination.address[3] | (header->destination.address[2] << 8)))&0xffff; // Word 8
    if (sum > 0xffff) {//If there is a carry bit, add it to the sum (to keep it within 16 bits)
        sum = sum & 0xffff;
        sum++;
    }
    sum = (sum & 0xffff);
    sum = ~sum; //invert all bits

    header->checksum = sum;
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
    ip.flags = ((temp & 0xE0) >> 5);
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
#if IPv4_DEBUG_MESSAGES==true
    UARTTransmitText("[Expected ");
    UARTTransmitText(hexToString(testHeader.checksum));
    UARTTransmitText(", got ");
    UARTTransmitText(hexToString(header->checksum));
    UARTTransmitText("]");
#endif
    return false;
}

/* =======================  Reception ======================= */

void ipv4_handleNewPacket(ethernetFrame_t *frame) {
    ipv4_header_t header;
    memoryField_t headerField;
    memoryField_t payloadField;
    /* ========  Error check ======== */
    if (frame->ethertype != ETHERTYPE_IPv4) {
        ethernetController_dropPacket(frame);
        return;
    }
    /* ========  Get header info ======== */
    headerField = frame->memory; //Copy memory information
    headerField.start = frame->memory.start + 22; //skip NextPacketPointer (2 Bytes), RSV (6 Bytes), 2 MAC addresses (12 Bytes) and EtherType (2 Bytes)
    //Only headerField.start is needed for parsing
    header = ipv4_parseHeader(&headerField);
    //After parsing, the end address + length can be filled out
    headerField.length = header.headerLength * 4;
    if (headerField.start + headerField.length > END_OF_MEMORY_ADDRESS) {//Wrap around
        headerField.end = headerField.length - (END_OF_MEMORY_ADDRESS - headerField.start) - 1;
    } else
        headerField.end = headerField.start + headerField.length - 1;

    payloadField.start = headerField.end + 1; //the payload starts where the header ends
    payloadField.end = frame->memory.end; //the payload ends where the packet ends
    //Calculate field length, considering wrap around
    payloadField.length = (payloadField.start < payloadField.end) ? (payloadField.end - payloadField.start) : ((END_OF_MEMORY_ADDRESS - payloadField.start) + payloadField.end);
    
   /* UARTTransmitText("\n\rEthernet Frame: ");
    UARTTransmitText(intToString(frame->memory.start));
    UARTTransmitText(" -> ");
    UARTTransmitText(intToString(frame->memory.end));
    UARTTransmitText(" (");
    UARTTransmitText(intToString(frame->memory.length));
    UARTTransmitText(")\n\r");

    UARTTransmitText("IP Header: ");
    UARTTransmitText(intToString(headerField.start));
    UARTTransmitText(" -> ");
    UARTTransmitText(intToString(headerField.end));
    UARTTransmitText(" (");
    UARTTransmitText(intToString(headerField.length));
    UARTTransmitText(")\n\r");

    UARTTransmitText("IP payload: ");
    UARTTransmitText(intToString(payloadField.start));
    UARTTransmitText(" -> ");
    UARTTransmitText(intToString(payloadField.end));
    UARTTransmitText(" (");
    UARTTransmitText(intToString(payloadField.length));
    UARTTransmitText(")\n\r");*/

    /* ========  Check header ======== */
    if (!ipv4_checkHeaderChecksum(&header)) {
#if IPv4_DEBUG_MESSAGES==true
        UARTTransmitText("\033[41;10;10m"); //Red color, Primary font
        UARTTransmitText("[Invalid checksum]");
        UARTTransmitText("\033[0m");
#endif
        ethernetController_dropPacket(frame);
    }
    if (header.version != 4) {
        ethernetController_dropPacket(frame);
#if IPv4_DEBUG_MESSAGES==true
        UARTTransmitText("\033[41;10;10m"); //Red color, Primary font
        UARTTransmitText("[Bogus version]");
        UARTTransmitText("\033[0m");
#endif
    }
    if (header.timeToLive == 0) {
        ethernetController_dropPacket(frame);
#if IPv4_DEBUG_MESSAGES==true
        UARTTransmitText("\033[41;10;10m"); //Red color, Primary font
        UARTTransmitText("[TTL=0]");
        UARTTransmitText("\033[0m");
#endif
    }
    if (header.totalLength == 0) {
        ethernetController_dropPacket(frame);
#if IPv4_DEBUG_MESSAGES==true
        UARTTransmitText("\033[41;10;10m"); //Red color, Primary font
        UARTTransmitText("[Length=0]");
        UARTTransmitText("\033[0m");
#endif
    }
#if IPv4_DEBUG_MESSAGES==true
    if (header.flags & IPv4_FLAG_DONT_FRAGMENT) {
        UARTTransmitText("[DF]");
    }
    if (header.flags & IPv4_FLAG_MORE_FRAGMENTS) {
        UARTTransmitText("[MF]");
    }
#endif

    /* ========  Give to higher protocol ======== */
    switch (header.protocol) {
        case IPv4_PROTOCOL_ICMP:
#if IPv4_DEBUG_MESSAGES==true
            UARTTransmitText("\033[103;30;10m"); //bright yellow color, black foreground,  Primary font
            UARTTransmitText("[");
            UARTTransmitText((ipProtocolToString(header.protocol)));
            UARTTransmitText("]");
            UARTTransmitText("\033[0m");
#endif
            icmp_handleNewPacket(header, payloadField);
            break;
        case IPv4_PROTOCOL_TCP:
#if IPv4_DEBUG_MESSAGES==true
            UARTTransmitText("\033[42;30;10m"); //green color, black foreground,  Primary font
            UARTTransmitText("[");
            UARTTransmitText((ipProtocolToString(header.protocol)));
            UARTTransmitText("]");
            UARTTransmitText("\033[0m");
#endif
            break;
        case IPv4_PROTOCOL_UDP:
#if IPv4_DEBUG_MESSAGES==true
            UARTTransmitText("\033[46;30;10m"); //cyan color, black foreground,  Primary font
            UARTTransmitText("[");
            UARTTransmitText((ipProtocolToString(header.protocol)));
            UARTTransmitText("]");
            UARTTransmitText("\033[0m");
#endif
            break;
        default:
#if IPv4_DEBUG_MESSAGES==true
            UARTTransmitText("\033[41;10;10m"); //Red color, Primary font
            UARTTransmitText("[Unknown Protocol]");
            UARTTransmitText("\033[0m");

#endif
            break;
    }
    ethernetController_dropPacket(frame);

#if IPv4_DEBUG_MESSAGES==true
    UARTTransmitText("[");
    UARTTransmitText(ipAdressToString(header.source));
    UARTTransmitText(" -> ");
    UARTTransmitText(ipAdressToString(header.destination));
    UARTTransmitText("]");
#endif
}

ipv4_address_t ipv4_getIPSourceAddress() {
    return ipSource;
}

ipv4_address_t ipv4_getPreliminaryIPSourceAddress() {
    return ipPreliminarySource;
}

void ipv4_setIPSourceAddress(ipv4_address_t ip) {
    ipPreliminarySource = ip;
    fSrcAddrChanged = true; //used as an indicator when to check for collisions
}

/* =======================  Background work ======================= */

error_t ipv4_background(linkState_t link) {
    error_t err;
    err.module = ERROR_MODULE_IPv4;
    err.code = ERROR_CODE_SUCCESSFUL;
    if ((*arpStatus).module == ERROR_MODULE_ARP && fSrcAddrChanged) {
        switch ((*arpStatus).code) {
            case ERROR_CODE_SUCCESSFUL: //ARP process completed successfully
                err = (*arpStatus);
#if IPv4_DEBUG_MESSAGES==true
                UARTTransmitText("[IPv4]: ARP process completed.\n\r");
#endif
                ipSource = ipPreliminarySource;
                fSrcAddrChanged = false;
                return err;
            case ERROR_ARP_WAITING: //Not all probes/announcement have been sent yet
                err = (*arpStatus);
                return err;
            case ERROR_ARP_IPv4_ADDRESS_CONFLICT: //There is someone on the network with the same address
                err.code = ERROR_IPv4_ADDRESS_ALREADY_IN_USE;
                fSrcAddrChanged = false;
                return err;
            case ERROR_ARP_MAXIMUM_NUMBER_OF_PROBES_REACHED: //Nobody answered ARP probes
                //this state indicates that ARP probing has been finished
                //probing is started after that automatically by arp
                return err;
            case ERROR_ARP_CONNECTION_FAILED://link disconnected during arp operation
                err = (*arpStatus);
                fSrcAddrChanged = false;
                return err;
            default:
            case ERROR_ARP_UNKNOWN:
                fSrcAddrChanged = false;
                err = (*arpStatus);
                return err;
        }
    } else
        err.code = ERROR_IPv4_UNKNOWN;
    return err;
}

bool_t ipv4_SrcAddrChanged(error_t *err) {
    arpStatus = err;
    return fSrcAddrChanged;
}

/* ======================= Address operations  ======================= */

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
        ip->address[i] = 0x00;
    }
}

/* ======================= Init ======================= */
void ipv4_init() {
    fSrcAddrChanged = false;
    ipv4_setToAllZero(&ipSource);
    ipv4_setToAllZero(&ipPreliminarySource);
}

ipv4_address_t ipv4_generateAutoIP() {
    ipv4_address_t ip;
    //Find a random address between 169.254.1.0 and 169.254.254.255
    ip.address[0] = 169;
    ip.address[1] = 254;
    ip.address[2] = (rand() % 253) + 1;
    ip.address[3] = (rand() % 256);
    return ip;
}