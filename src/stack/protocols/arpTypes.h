/** 
 * \file arpTypes.h
 * \brief Data types used for the Address Resolution Protocol (ARP)
 * \author Stefan Gloor
 * \version 1.0
 * \date 19. February 2019
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
 * 
 */

#ifndef ARPTYPES_H
#define	ARPTYPES_H

#include <stdint.h>
#include "../../eth/ethernetTypes.h"
#include "../protocols/ipv4Types.h"
#include "../error.h"
#include "../time.h"

/**
 * \ingroup arp
 * \brief Contains all information which makes up an ARP message
 */
typedef struct arp_message {
    uint16_t htype; ///< Hardware yype. Ethernet is 1.
    uint16_t ptype; ///< Protocol type. For IPv4 it's 0x0800.
    uint8_t hlen; ///< Hardware address length in bytes. Ethernet addresses have 6 Bytes.
    uint8_t plen; ///< Protocol address length in bytes. IPv4 has 4 byte wide addresses.
    uint16_t operation; ///< Request or Reply; use \ref arp_operation
    macaddress_t senderMACAddress; ///< Sender hardware address (SHA)
    ipv4_address_t senderIPAddress; ///< Sender protocol address (SPA)
    macaddress_t targetMACAddress; ///< Target hardware address (THA)
    ipv4_address_t targetIPAddress; ///< Target protocol address (TPA)
    uint8_t fIsGratuitous : 1; ///< Flag indicating if the message is gratuitous
    uint8_t fIsProbe : 1; ///< Flag indicating if the message is an announcement
    error_t err; ///< Error code
} arp_message_t;

/**
 * \ingroup arp
 * \brief Data types for \ref arp_message_t.operation
 */
typedef enum arp_operation {
    ARP_REQUEST = 1,
    ARP_REPLY = 2
} arp_operation_t;

/**
 * \ingroup arp
 * \brief Data type for \ref arp_message_t.htype
 */
typedef enum arp_htype {
    ARP_HTYPE_ETHERNET = 1
} arp_htype_t;

/**
 * \ingroup arp
 * \brief Data type for \ref arp_message_t.ptype
 */
typedef enum arp_ptype {
    ARP_PTYPE_IPv4 = 0x0800 ///< The only thing this ARP implementation supports is Internet Protocol Version 4
} arp_ptype_t;

/**
 * \ingroup arp
 * \brief One entry in the ARP table consists out of one hardware and one protocol address and a timestamp
 */
typedef struct arp_tableEntry {
    ipv4_address_t ip; ///< Protocol address
    macaddress_t mac; ///< Corresponding hardware address
    time_t timeCreated; ///< Number of seconds that have elapsed since power up when this entry was created
} arp_tableEntry_t;



#endif	/* ARPTYPES_H */