/**
 * \file udp.c
 * \brief User datagram protocol
 * \author Stefan Gloor
 * \version 1.0
 * \date 4. May 2019
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

#include "udp.h"

stack_t extern stack;

error_t udp_sendPacket(udpHeader_t header, ipv4_address_t dst) {
    uint8_t headerBuf[64];
    uint8_t ipHeaderBuf[32];
    
    uint8_t static start=0;

    ipv4_packet_t ipPacket;
    ipPacket.ipv4Header.headerLength = 5;
    ipPacket.ipv4Header.dscp = 0x00;
    ipPacket.ipv4Header.ecn = 0x00;
    ipPacket.ipv4Header.flags = 0x00;
    ipPacket.ipv4Header.fragmentOffset = 0x0000;
    ipPacket.ipv4Header.protocol = IPv4_PROTOCOL_UDP;
    ipPacket.ipv4Header.destination = dst;
    ipPacket.ipv4Header.source = ipv4_getIPSourceAddress();
    ipPacket.ipv4Header.totalLength = header.length + 20;
    ipPacket.ipv4Header.timeToLive = 255;
    ipPacket.ipv4Header.version = 4;

    ipv4_calculateHeaderChecksum(&ipPacket.ipv4Header);
    ipv4_writeHeaderIntoBuffer(ipPacket.ipv4Header, &ipHeaderBuf);

    ipv4_txFrameRequest(&ipPacket);

    //udp_calculateHeaderChecksum(&header);
    header.checksum = 0x0000;

    headerBuf[0] = (header.srcPort & 0xff00) >> 8;
    headerBuf[1] = (header.srcPort & 0x00ff);
    headerBuf[2] = (header.dstPort & 0xff00) >> 8;
    headerBuf[3] = (header.dstPort & 0x00ff);
    headerBuf[4] = (header.length & 0xff00) >> 8;
    headerBuf[5] = (header.length & 0x00ff);
    headerBuf[6] = (header.checksum & 0xff00) >> 8;
    headerBuf[7] = (header.checksum & 0x00ff);


    uint16_t foo = start++;
    for (uint16_t i = 0; i < ipPacket.ipv4Header.totalLength; i++) {
        if (i < ipPacket.ipv4Header.headerLength * 4) {
            ipv4_streamToTransmissionBuffer(ipHeaderBuf[i], ipPacket);
        } else if (i < ipPacket.ipv4Header.headerLength * 4 + 8) {
            ipv4_streamToTransmissionBuffer(headerBuf[i - ipPacket.ipv4Header.headerLength * 4], ipPacket);
        } else {
            ipv4_streamToTransmissionBuffer(foo++, ipPacket);
        }
    }

    stack.pendingPacketToSend = ipPacket;
    stack.background.fPacketPending = true;
}

void static udp_calculateHeaderChecksum(udpHeader_t* header) {
    uint32_t sum = 0;

    sum += (header->srcPort);
    sum += (header->dstPort);
    sum += (header->length);

    uint8_t carry = (sum & 0xffff0000) >> 16;
    sum &= 0xffff;
    sum += carry;
    if (sum > 0xffff) {
        sum += ((sum & 0xffff0000) >> 16);
        sum &= 0xffff;
    }

    header->checksum = ~sum;
}