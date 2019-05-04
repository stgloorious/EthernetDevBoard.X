/**
 * \file tcp.c
 * \brief Transmission control protocol
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

#include "tcp.h"

stack_t extern stack;

tcp_connection_t static tcpListeners [MAX_LISTENERS];

error_t tcp_handleNewPacket(ipv4_header_t ipHeader, memoryField_t ipPayload) {
    tcp_header_t tcpHeader;
    tcp_parseHeader(ipPayload, &tcpHeader);

    UARTTransmitText("[");
    UARTTransmitText(intToString(tcpHeader.srcPort, 10));
    UARTTransmitText(" -> ");
    UARTTransmitText(intToString(tcpHeader.dstPort, 10));
    UARTTransmitText("][Seq=");
    UARTTransmitText(intToString(tcpHeader.seqNumber, 10));
    UARTTransmitText("]");
    if (tcpHeader.flags.NS)
        UARTTransmitText("[NS]");
    if (tcpHeader.flags.CWR)
        UARTTransmitText("[CWR]");
    if (tcpHeader.flags.ECE)
        UARTTransmitText("[ECE]");
    if (tcpHeader.flags.URG)
        UARTTransmitText("[URG]");
    if (tcpHeader.flags.ACK)
        UARTTransmitText("[ACK]");
    if (tcpHeader.flags.PSH)
        UARTTransmitText("[PSH]");
    if (tcpHeader.flags.RST)
        UARTTransmitText("[RST]");
    if (tcpHeader.flags.SYN)
        UARTTransmitText("[SYN]");
    if (tcpHeader.flags.FIN)
        UARTTransmitText("[FIN]");

    for (uint8_t i = 0; i < MAX_LISTENERS; i++) {
        if (tcpHeader.dstPort == tcpListeners[i].srcPort &&
                tcpListeners[i].state == TCP_STATE_LISTEN) {
            UARTTransmitText("[Am listening on this port]");
            tcpListeners[i].ipClient = ipHeader.source;
            tcpListeners[i].dstPort = tcpHeader.srcPort;
            tcp_stateMachine(&tcpListeners[i], &tcpHeader);
            break;
        }
    }

}

error_t tcp_parseHeader(memoryField_t ipPayload, tcp_header_t *header) {
    uint8_t temp;
    ethernetController_streamFromRXBuffer(0, ipPayload.start); //Open stream
    header->srcPort = ((uint16_t) ethernetController_streamFromRXBuffer(1, ipPayload.start) << 8);
    header->srcPort |= (uint16_t) ethernetController_streamFromRXBuffer(1, ipPayload.start);
    header->dstPort = ((uint16_t) ethernetController_streamFromRXBuffer(1, ipPayload.start) << 8);
    header->dstPort |= (uint16_t) ethernetController_streamFromRXBuffer(1, ipPayload.start);
    header->seqNumber = ((uint32_t) ethernetController_streamFromRXBuffer(1, ipPayload.start) << 24);
    header->seqNumber |= ((uint32_t) ethernetController_streamFromRXBuffer(1, ipPayload.start) << 16);
    header->seqNumber |= ((uint32_t) ethernetController_streamFromRXBuffer(1, ipPayload.start) << 8);
    header->seqNumber |= (uint32_t) ethernetController_streamFromRXBuffer(1, ipPayload.start);
    header->ackNumber = ((uint32_t) ethernetController_streamFromRXBuffer(1, ipPayload.start) << 24);
    header->ackNumber |= ((uint32_t) ethernetController_streamFromRXBuffer(1, ipPayload.start) << 16);
    header->ackNumber |= ((uint32_t) ethernetController_streamFromRXBuffer(1, ipPayload.start) << 8);
    header->ackNumber |= ethernetController_streamFromRXBuffer(1, ipPayload.start);
    temp = ethernetController_streamFromRXBuffer(1, ipPayload.start);
    header->dataOffset = (temp & 0xf0) >> 4;
    header->flags.NS = temp & 0x01;
    temp = ethernetController_streamFromRXBuffer(1, ipPayload.start);
    header->flags.CWR = temp & (0x01 << 7) ? 1 : 0;
    header->flags.ECE = temp & (0x01 << 6) ? 1 : 0;
    header->flags.URG = temp & (0x01 << 5) ? 1 : 0;
    header->flags.ACK = temp & (0x01 << 4) ? 1 : 0;
    header->flags.PSH = temp & (0x01 << 3) ? 1 : 0;
    header->flags.RST = temp & (0x01 << 2) ? 1 : 0;
    header->flags.SYN = temp & (0x01 << 1) ? 1 : 0;
    header->flags.FIN = temp & (0x01 << 0) ? 1 : 0;
    header->windowSize = ((uint16_t) ethernetController_streamFromRXBuffer(1, ipPayload.start) << 8);
    header->windowSize |= (uint16_t) ethernetController_streamFromRXBuffer(1, ipPayload.start);
    header->checksum = ((uint16_t) ethernetController_streamFromRXBuffer(1, ipPayload.start) << 8);
    header->checksum |= (uint16_t) ethernetController_streamFromRXBuffer(1, ipPayload.start);
    header->urgentPointer = ((uint16_t) ethernetController_streamFromRXBuffer(1, ipPayload.start) << 8);
    header->urgentPointer |= (uint16_t) ethernetController_streamFromRXBuffer(1, ipPayload.start);
    ethernetController_streamFromRXBuffer(2, ipPayload.start); //end stream
}

error_t tcp_stateMachine(tcp_connection_t *connection, tcp_header_t *newHeader) {
    switch (connection->state) {
        case TCP_STATE_CLOSED:
            break;
        case TCP_STATE_CLOSE_WAIT:
            break;
        case TCP_STATE_CLOSING:
            break;
        case TCP_STATE_ESTABLISHED:
            break;
        case TCP_STATE_FIN_WAIT1:
            break;
        case TCP_STATE_FIN_WAIT2:
            break;
        case TCP_STATE_LAST_ACK:
            break;
        case TCP_STATE_LISTEN:
            if (newHeader->flags.SYN) {
                connection->state = TCP_STATE_SYN_RECEIVED;
                UARTTransmitText("[SYN Received]");
                tcp_sendSYNACK(connection, newHeader->seqNumber);
            }
            break;
        case TCP_STATE_SYN_RECEIVED:
            break;
        case TCP_STATE_SYN_SENT:
            break;
        case TCP_STATE_TIME_WAIT:
            break;
    }

}

error_t tcp_listen(tcp_connection_t *connection) {
    connection->state = TCP_STATE_LISTEN;
    connection->seqNumber = rand() * rand();
    tcpListeners[0] = *connection;

    UARTTransmitText("[TCP]: Listening on Port ");
    UARTTransmitText(intToString(connection->srcPort, 10));
    UARTTransmitText(" (Seq= ");
    UARTTransmitText(intToString(connection->seqNumber, 10));
    UARTTransmitText(")\n\r");

}

error_t tcp_sendSYNACK(tcp_connection_t *connection, uint32_t seq) {
    tcp_header_t tcpHeader;
    uint8_t headerBuf[64];
    uint8_t ipHeaderBuf[32];

    ipv4_packet_t ipPacket;
    ipPacket.ipv4Header.headerLength = 5;
    ipPacket.ipv4Header.dscp = 0x00;
    ipPacket.ipv4Header.ecn = 0x00;
    ipPacket.ipv4Header.flags = 0x00;
    ipPacket.ipv4Header.fragmentOffset = 0x0000;
    ipPacket.ipv4Header.protocol = IPv4_PROTOCOL_TCP;
    ipPacket.ipv4Header.destination = connection->ipClient;
    ipPacket.ipv4Header.source = ipv4_getIPSourceAddress();
    ipPacket.ipv4Header.totalLength = 40;
    ipPacket.ipv4Header.timeToLive = 255;
    ipPacket.ipv4Header.version = 4;

    ipv4_calculateHeaderChecksum(&ipPacket.ipv4Header);
    ipv4_writeHeaderIntoBuffer(ipPacket.ipv4Header, &ipHeaderBuf);

    ipv4_txFrameRequest(&ipPacket);

    tcpHeader.dataOffset = 5;
    tcpHeader.dstPort = connection->dstPort;
    tcpHeader.srcPort = connection->srcPort;
    tcpHeader.ackNumber = seq + 1;
    tcpHeader.seqNumber = 123;
    tcpHeader.urgentPointer = 0;
    tcpHeader.windowSize = 20;
    tcpHeader.flags.ACK = 1;
    tcpHeader.flags.SYN = 1;
    tcpHeader.flags.NS = 0;
    tcpHeader.flags.CWR = 0;
    tcpHeader.flags.ECE = 0;
    tcpHeader.flags.URG = 0;
    tcpHeader.flags.PSH = 0;
    tcpHeader.flags.RST = 0;
    tcpHeader.flags.FIN = 0;

    tcp_calculateHeaderChecksum(&tcpHeader, ipPacket.ipv4Header);


    UARTTransmitText("[");
    UARTTransmitText(intToString(tcpHeader.srcPort, 10));
    UARTTransmitText(" -> ");
    UARTTransmitText(intToString(tcpHeader.dstPort, 10));
    UARTTransmitText("]");
    headerBuf[0] = (tcpHeader.srcPort & 0xff00) >> 8;
    headerBuf[1] = tcpHeader.srcPort & 0x00ff;
    headerBuf[2] = (tcpHeader.dstPort & 0xff00) >> 8;
    headerBuf[3] = tcpHeader.dstPort & 0x00ff;
    headerBuf[4] = (tcpHeader.seqNumber & 0xff000000) >> 24;
    headerBuf[5] = (tcpHeader.seqNumber & 0x00ff0000) >> 16;
    headerBuf[6] = (tcpHeader.seqNumber & 0x0000ff00) >> 8;
    headerBuf[7] = tcpHeader.seqNumber & 0x000000ff;
    headerBuf[8] = (tcpHeader.ackNumber & 0xff000000) >> 24;
    headerBuf[9] = (tcpHeader.ackNumber & 0x00ff0000) >> 16;
    headerBuf[10] = (tcpHeader.ackNumber & 0x0000ff00) >> 8;
    headerBuf[11] = tcpHeader.ackNumber & 0x000000ff;
    headerBuf[12] = (tcpHeader.dataOffset << 4) | (tcpHeader.flags.NS ? 0x01 : 0x00);
    headerBuf[13] = (tcpHeader.flags.CWR ? 0x80 : 0x00) |
            (tcpHeader.flags.CWR ? 0x40 : 0x00) |
            (tcpHeader.flags.URG ? 0x20 : 0x00) |
            (tcpHeader.flags.ACK ? 0x10 : 0x00) |
            (tcpHeader.flags.PSH ? 0x08 : 0x00) |
            (tcpHeader.flags.RST ? 0x04 : 0x00) |
            (tcpHeader.flags.SYN ? 0x02 : 0x00) |
            (tcpHeader.flags.FIN ? 0x01 : 0x00);
    headerBuf[14] = (tcpHeader.windowSize & 0xff00) >> 8;
    headerBuf[15] = tcpHeader.windowSize & 0x00ff;
    headerBuf[16] = (tcpHeader.checksum & 0xff00) >> 8;
    headerBuf[17] = tcpHeader.checksum & 0x00ff;
    headerBuf[18] = (tcpHeader.urgentPointer & 0xff00) >> 8;
    headerBuf[19] = tcpHeader.urgentPointer & 0x00ff;

    for (uint16_t i = 0; i < ipPacket.ipv4Header.totalLength; i++) {
        if (i < ipPacket.ipv4Header.headerLength * 4) {
            ipv4_streamToTransmissionBuffer(ipHeaderBuf[i], ipPacket);
        } else {
            ipv4_streamToTransmissionBuffer(headerBuf[i - ipPacket.ipv4Header.headerLength * 4], ipPacket);
        }
    }

    stack.pendingPacketToSend = ipPacket;
    stack.background.fPacketPending = true;

}

error_t static tcp_calculateHeaderChecksum(tcp_header_t *tcpHeader, ipv4_header_t ipv4Header) {
    uint32_t sum = 0;

    sum += ((uint16_t) ipv4Header.source.address[0] << 8) | (ipv4Header.source.address[1]);
    sum += ((uint16_t) ipv4Header.source.address[2] << 8) | (ipv4Header.source.address[3]);
    sum += ((uint16_t) ipv4Header.destination.address[0] << 8) | (ipv4Header.destination.address[1]);
    sum += ((uint16_t) ipv4Header.destination.address[2] << 8) | (ipv4Header.destination.address[3]);
    sum += ipv4Header.protocol;
    sum += 20; //tcp length


    sum += (tcpHeader->srcPort);
    sum += (tcpHeader->dstPort);
    sum += (tcpHeader->seqNumber)&0x0000ffff;
    sum += (uint16_t) (((tcpHeader->seqNumber)&0xffff0000) >> 16);
    sum += (tcpHeader->ackNumber)&0x0000ffff;
    sum += (uint16_t) (((tcpHeader->ackNumber)&0xffff0000) >> 16);
    sum += (((uint16_t) tcpHeader->dataOffset & 0x0f) << 12) |
            (tcpHeader->flags.NS ? 0x100 : 0x00) |
            (tcpHeader->flags.CWR ? 0x80 : 0x00) |
            (tcpHeader->flags.ECE ? 0x40 : 0x00) |
            (tcpHeader->flags.URG ? 0x20 : 0x00) |
            (tcpHeader->flags.ACK ? 0x10 : 0x00) |
            (tcpHeader->flags.PSH ? 0x08 : 0x00) |
            (tcpHeader->flags.RST ? 0x04 : 0x00) |
            (tcpHeader->flags.SYN ? 0x02 : 0x00) |
            (tcpHeader->flags.FIN ? 0x01 : 0x00);
    sum += (tcpHeader->windowSize);
    sum += (tcpHeader->urgentPointer);


    uint8_t carry = (sum & 0xffff0000) >> 16;
    sum &= 0xffff;
    sum += carry;
    if (sum > 0xffff) {
        sum += ((sum & 0xffff0000) >> 16);
        sum &= 0xffff;
    }

    tcpHeader->checksum = ~sum;
}

void tcp_init() {
    for (uint8_t i = 0; i < MAX_LISTENERS; i++) {
        tcpListeners[i].seqNumber = 0;
        tcpListeners[i].srcPort = 0;
        tcpListeners[i].dstPort = 0;
        tcpListeners[i].state = TCP_STATE_CLOSED;
    }
}