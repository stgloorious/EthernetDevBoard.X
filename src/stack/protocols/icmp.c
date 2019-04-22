/**
 * \file icmp.c
 * \brief Internet control message protocol
 * \author Stefan Gloor
 * \version 1.0
 * \date 21. April 2019
 * \ingroup protocols
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

#include "icmp.h"
#include <stdlib.h>

stack_t extern stack;

error_t icmp_handleNewPacket(ipv4_header_t ipHeader, memoryField_t ipPayload) {
    icmpHeader_t icmpHeader;
    icmp_parseHeader(ipPayload, &icmpHeader);

    UARTTransmitText("\033[40;33;10m"); //bright yellow color foreground, black blackground,  Primary font
    UARTTransmitText("[");
    UARTTransmitText(icmpTxtMessages[icmpHeader.typeOfMessage]);
    UARTTransmitText("]");
    UARTTransmitText("\033[0m");

    if (icmp_calculateChecksum(ipPayload) != icmpHeader.checksum) {
        UARTTransmitText("\033[41;10;10m"); //Red color, Primary font
        UARTTransmitText("[BAD CHECKSUM]");
        UARTTransmitText("\033[0m");
    }

    if (icmpHeader.typeOfMessage == ICMP_ECHO_REQUEST) {
        UARTTransmitText("[id=");
        UARTTransmitText(intToString((icmpHeader.headerData & 0xffff0000) >> 16));
        UARTTransmitText("][seq=");
        UARTTransmitText(intToString(icmpHeader.headerData & 0x0000ffff));
        UARTTransmitText("]");

        icmp_sendEchoReply(ipHeader, icmpHeader, ipPayload);
    }

}

error_t icmp_parseHeader(memoryField_t ipPayload, icmpHeader_t *icmpHeader) {
    ethernetController_streamFromRXBuffer(0, ipPayload.start); //Open stream
    icmpHeader->typeOfMessage = ethernetController_streamFromRXBuffer(1, ipPayload.start);
    icmpHeader->code = ethernetController_streamFromRXBuffer(1, ipPayload.start);
    icmpHeader->checksum = ((uint32_t) ethernetController_streamFromRXBuffer(1, ipPayload.start) << 8);
    icmpHeader->checksum |= ethernetController_streamFromRXBuffer(1, ipPayload.start);

    icmpHeader->headerData = ((uint32_t) ethernetController_streamFromRXBuffer(1, ipPayload.start) << 24);
    icmpHeader->headerData |= ((uint32_t) ethernetController_streamFromRXBuffer(1, ipPayload.start) << 16);
    icmpHeader->headerData |= ((uint32_t) ethernetController_streamFromRXBuffer(1, ipPayload.start) << 8);
    icmpHeader->headerData |= ethernetController_streamFromRXBuffer(1, ipPayload.start);

    ethernetController_streamFromRXBuffer(2, ipPayload.start); //end stream
}

uint16_t static icmp_calculateChecksum(memoryField_t ipPayload) {
    ipPayload.length += 4; //bodge fix for some other error
    uint32_t sum = 0;

    ethernetController_streamFromRXBuffer(0, ipPayload.start); //Open stream
    for (uint16_t i = 0; i < ipPayload.length / 2; i++) {
        uint16_t foo = 0;
        foo = ((uint16_t) ethernetController_streamFromRXBuffer(1, ipPayload.start) << 8);
        foo |= ethernetController_streamFromRXBuffer(1, ipPayload.start);
        if (i == 1) {
            foo = 0x0000; //replace the checksum field with 0x0000 for calculation
        }
        sum += foo;
    }
    ethernetController_streamFromRXBuffer(2, ipPayload.start); //end stream

    uint8_t carry = (sum & 0xffff0000) >> 16;
    sum &= 0xffff;
    sum += carry;
    if (sum > 0xffff) {
        sum += ((sum & 0xffff0000) >> 16);
        sum &= 0xffff;
    }

    return ~sum;
}

uint16_t static icmp_calculateChecksumBuf(uint8_t *buf, uint16_t ipPayloadLength) {
    ipPayloadLength += 4;
    uint32_t sum = 0;

    for (uint16_t i = 0; i < ipPayloadLength / 2; i++) {
        uint16_t foo = 0;
        foo = (uint16_t) (*(buf++) << 8);
        foo |= *(buf++);
        if (i == 1) {
            foo = 0x0000; //replace the checksum field with 0x0000 for calculation
        }
        sum += foo;
    }

    uint8_t carry = (sum & 0xffff0000) >> 16;
    sum &= 0xffff;
    sum += carry;
    if (sum > 0xffff) {
        sum += ((sum & 0xffff0000) >> 16);
        sum &= 0xffff;
    }

    return ~sum;
}

void icmp_sendEchoReply(ipv4_header_t ipHeader, icmpHeader_t icmpRequestHeader, memoryField_t ipPayload) {
    ipv4_packet_t ipReply;
    icmpHeader_t icmpHeader;

    uint8_t headerBuf[32];
    ipReply.ipv4Header.headerLength = 5;
    ipReply.ipv4Header.dscp = 0x00;
    ipReply.ipv4Header.ecn = 0x00;
    ipReply.ipv4Header.flags = 0x00;
    ipReply.ipv4Header.fragmentOffset = 0x0000;
    ipReply.ipv4Header.protocol = IPv4_PROTOCOL_ICMP;
    ipReply.ipv4Header.destination = ipHeader.source;
    ipReply.ipv4Header.source = ipv4_getIPSourceAddress();
    ipReply.ipv4Header.totalLength = ipHeader.totalLength;
    ipReply.ipv4Header.timeToLive = 255;
    ipReply.ipv4Header.version = 4;

    ipv4_calculateHeaderChecksum(&ipReply.ipv4Header);
    ipv4_writeHeaderIntoBuffer(ipReply.ipv4Header, &headerBuf);

    icmpHeader.typeOfMessage = ICMP_ECHO_REPLY;
    icmpHeader.code = 0x00;
    icmpHeader.headerData = icmpRequestHeader.headerData;
    icmpHeader.checksum = icmpRequestHeader.checksum;

    uint8_t icmpMessage[64];
    icmp_writeHeaderIntoBuffer(&icmpMessage[0], icmpHeader);

    ethernetController_streamFromRXBuffer(0, ipPayload.start + 8);
    for (uint8_t i = 8; i < ipPayload.length + 4; i++) {
        icmpMessage[i] = ethernetController_streamFromRXBuffer(1, ipPayload.start + 8);
    }
    ethernetController_streamFromRXBuffer(2, ipPayload.start + 8);

    uint16_t checksum = icmp_calculateChecksumBuf(&icmpMessage[0], ipPayload.length);
    icmpMessage [2] = (checksum & 0xff00) >> 8;
    icmpMessage [3] = checksum & 0x00ff;

    ipv4_txFrameRequest(&ipReply);

    for (uint16_t i = 0; i < ipReply.ipv4Header.totalLength; i++) {
        if (i < ipReply.ipv4Header.headerLength * 4) {
            ipv4_streamToTransmissionBuffer(headerBuf[i], ipReply);
        } else {
            ipv4_streamToTransmissionBuffer(icmpMessage[i - (ipReply.ipv4Header.headerLength * 4)], ipReply);
        }
    }

    UARTTransmitText("\033[40;33;10m"); //bright yellow color foreground, black blackground,  Primary font
    UARTTransmitText("[ECHO REPLY SENT TO ");
    UARTTransmitText(ipAdressToString(ipHeader.source));
    UARTTransmitText("]");
    UARTTransmitText("\033[0m");

    stack.pendingPacketToSend = ipReply;
    stack.background.fPacketPending = true;
}

void icmp_writeHeaderIntoBuffer(uint8_t *buf, icmpHeader_t header) {
    *buf = header.typeOfMessage;
    *(buf + 1) = header.code;
    *(buf + 2) = (header.checksum & 0xff00) >> 8;
    *(buf + 3) = header.checksum & 0xff;
    *(buf + 4) = (header.headerData & 0xff000000) >> 24;
    *(buf + 5) = (header.headerData & 0x00ff0000) >> 16;
    *(buf + 6) = (header.headerData & 0x0000ff00) >> 8;
    *(buf + 7) = (header.headerData & 0x000000ff);
}