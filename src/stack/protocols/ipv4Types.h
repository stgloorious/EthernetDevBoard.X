/** 
 * \file ipv4Types.h
 * \brief Data types used in IPv4
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

#ifndef IPV4TYPES_H
#define	IPV4TYPES_H

#include <stdint.h>
#include "../../mem/memoryTypes.h"
#include "../../eth/ethernetTypes.h"

/**
 * \addtogroup ipv4
 * \ingroup protocols
 * \{
 */

/**
 * \brief Currrently supported protocols embedded in IPv4
 */
typedef enum ipv4_protocols {
    IPv4_PROTOCOL_ICMP = 1, ///< Internet Control Message Protocol
    IPv4_PROTOCOL_TCP = 6, ///< Transmission Control Protocol
    IPv4_PROTOCOL_UDP = 17///< User Datagram Protocol

} ipv4_protocol_t;

/**
 * \brief IPv4 Address type
 * \details Address is stored in an array of bytes, the first element being the first byte if read from left to right.
 * 
 * \code
 * // This sets an IPv4 Address to 192.168.0.1
 * ipv4_address_t ip;
 * ip[0] = 192;
 * ip[1] = 168;
 * ip[2] = 0;
 * ip[3] = 1;
 * \endcode
 */
typedef struct ipv4_address {
    uint8_t address[4]; ///< 4 Address bytes
} ipv4_address_t;

/**
 * \brief The header field of every IPv4 packet
 */
typedef struct ipv4_header {
    /**
     * \name Byte 0
     * \{
     */
    uint8_t version; ///< [4 Bits wide] Normally 4.
    uint8_t headerLength; ///< [4 Bits wide] Length in 32 bit words; Normally 5.
    /**\}*/

    /**
     * \name Byte 1
     * \{
     */
    uint8_t dscp; ///< [6 Bits wide] Differentiated Services Code Point
    uint8_t ecn; ///< [2 Bits wide] Explicit Congestion Notification
    /**\}*/

    /**
     * \name Byte 2 & 3
     * \{
     */
    uint16_t totalLength; ///< [16 Bits wide] Total length of the packet (including header) 
    /**\}*/

    /**
     * \name Byte 4 & 5
     * \{
     */
    uint16_t identification; ///< [16 Bits wide] Unique identification of a packet, used for reassembly after fragmenting
    /**\}*/

    /**
     * \name Byte 6 & 7
     * \{
     */
    uint8_t flags; ///< [3 Bits wide]
    uint16_t fragmentOffset; ///< [13 Bits wide] 
    /**\}*/

    /**
     * \name Byte 8
     * \{
     */
    uint8_t timeToLive; ///< [8 Bits wide]
    /**\}*/

    /**
     * \name Byte 9
     * \{
     */
    ipv4_protocol_t protocol; ///< [8 Bits wide]
    /**\}*/

    /**
     * \name Byte 10 & 11
     * \{
     */
    uint16_t checksum; ///< [16 Bits wide]
    /**\}*/

    /**
     * \name Byte 12..15
     * \{
     */
    ipv4_address_t source; ///< Address from where this came from
    /**\}*/

    /**
     * \name Byte 16..19
     * \{
     */
    ipv4_address_t destination; ///< Address to where this is going
    /**\}*/

} ipv4_header_t;

#define IPv4_FLAG_DONT_FRAGMENT     0b010
#define IPv4_FLAG_MORE_FRAGMENTS    0b001

/**
 * \brief Represantation of an IPv4 packet
 */
typedef struct ipv4_packet {
    /**
     * \brief Contains information about the header of this packet
     */
    ipv4_header_t ipv4Header;
    /**
     * \brief Where this ipv4 packet is stored in memory
     */
    memoryField_t memory;
    /**
     * \brief Information about the ethernet frame that was used to transmit/receive this ip packet
     */
    ethernetFrame_t ethernet;
} ipv4_packet_t;

ipv4_address_t ipSource;

/**\}*/

#endif	/* IPV4TYPES_H */