/** 
 * \file tcpTypes.h
 * \brief Transmission Control protocol
 * \author Stefan Gloor
 * \version 1.0
 * \date  19. February 2019
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

#ifndef TCPTYPES_H
#define	TCPTYPES_H

#include <stdint.h>

#include "ipv4Types.h"

/**
 * \brief Flags in the TCP header
 * \see https://en.wikipedia.org/wiki/Transmission_Control_Protocol#TCP_segment_structure
 */
typedef struct tcp_flags {
    uint8_t NS : 1; ///< ECN-nonce - concealment protection (experimental: see RFC 3540).
    uint8_t CWR : 1; ///< Congestion window reduced flag is set by the sending host to indicate that it received a TCP segment with the ECE flag set and had responded in congestion control mechanism (added to header by RFC 3168).
    uint8_t ECE : 1; ///< ECN-Echo has a dual role, depending on the value of the SYN flag.
    uint8_t URG : 1; ///< indicates that the Urgent pointer field is significant;
    uint8_t ACK : 1; ///< indicates that the Acknowledgment field is significant. All packets after the initial SYN packet sent by the client should have this flag set.
    uint8_t PSH : 1; ///< Push function. Asks to push the buffered data to the receiving application.
    uint8_t RST : 1; ///< Reset the connection
    uint8_t SYN : 1; ///<  Synchronize sequence numbers. Only the first packet sent from each end should have this flag set.
    uint8_t FIN : 1; ///< Last packet from sender.
} tcp_flags_t;

typedef struct tcp_header {
    uint16_t srcPort;
    uint16_t dstPort;
    uint32_t seqNumber;
    uint32_t ackNumber;
    uint8_t dataOffset;
    tcp_flags_t flags;
    uint16_t windowSize;
    uint16_t checksum;
    uint16_t urgentPointer;
} tcp_header_t;

typedef struct tcp_connection {
    ipv4_address_t ipServer; ///< IPv4 Address
    ipv4_address_t ipClient; ///< IPv4 Address
    uint16_t srcPort; ///< TCP Source Port
    uint16_t dstPort; ///< TCP Destination Port
    uint32_t seqNumber; ///< Sequence Number
    uint8_t state;///< Current TCP State
} tcp_connection_t;

typedef enum tcp_state {
    TCP_STATE_LISTEN,
    TCP_STATE_SYN_SENT,
    TCP_STATE_SYN_RECEIVED,
    TCP_STATE_ESTABLISHED,
    TCP_STATE_FIN_WAIT1,
    TCP_STATE_FIN_WAIT2,
    TCP_STATE_CLOSE_WAIT,
    TCP_STATE_CLOSING,
    TCP_STATE_LAST_ACK,
    TCP_STATE_TIME_WAIT,
    TCP_STATE_CLOSED
} tcp_state_t;


#endif	/* TCPTYPES_H */

