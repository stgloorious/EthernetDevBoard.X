/** 
 * \file ethernetTypes.h
 * \brief Data types used for Ethernet
 * \author Stefan Gloor
 * \version 1.0
 * \date 19. February 2019
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

#ifndef ETHERNETTYPES_H
#define	ETHERNETTYPES_H

#include <stdint.h>
#include "../enc424j600/rsv.h"
#include "../mem/memoryTypes.h"
#include "../stack/time.h"

/**
 * \ingroup ethernet
 * \{
 */

#define MAX_DATA_FIELD_LENGTH         1500 ///< Normal value is 1500; Values greater than 1500 are not compliant to the Ethernet standard
#define MAX_FRAME_LENGTH              MAX_DATA_FIELD_LENGTH+18 ///< Source MAC Addr (6 Bytes), Destination MAC Addr (6 Bytes), EtherType (2 Bytes), CRC (4 Bytes)
#define MAX_FRAME_LENGTH_WITH_VLAN    MAX_FRAME_LENGTH+4  ///< Additional 4 Bytes for VLAN:

/**\}*/

/**
 * \ingroup ethernet
 * \{
 */


typedef enum etherTypes {
    ETHERTYPE_IPv4 = 0x0800, ///< Internet Protocol Version 4
    ETHERTYPE_ARP = 0x0806, ///< Address Resolution Protocol
    ETHERTYPE_WOL = 0x0842, ///< Wake-On-LAN
    ETHERTYPE_FLOW_CONTROL = 0x8808///< FLow control
} etherType_t;

/**
 */
typedef struct macaddress {
    uint8_t address[6]; ///< Address bytes
} macaddress_t;

/**
 */
typedef enum linkStates {
    NO_LINK, ///< Nothing is connected
    LINK_ESTABLISHED ///< An Ethernet partner is present
} linkState_t;

/**
 */
typedef enum speeds {
    TEN_MBIT, ///< 10Base-T
    HUNDRED_MBIT ///< 100Base-TX
} speed_t;

/**
 */
typedef enum duplexStates {
    HALF_DUPLEX, ///< One after another
    FULL_DUPLEX ///< Both at the same type
} duplexState_t;

/**
 * \brief Represents a complete Ethernet Frame
 */
typedef struct ethernetFrame {
    uint16_t length; ///< The total length of the Ethernet frame.
    macaddress_t source; ///< Source Hardware address
    macaddress_t destination; ///< Destination Hardware address
    etherType_t ethertype; ///< EtherType field
    memoryField_t memory; ///< Memory field occupied by this frame
    /**
     * \brief The RSV that gets appended to every received frame by the ethernet controller
     * \note The RSV is unused if this structure is used for transmission
     */
    RSV_t receiveStatusVector;
    /**
     * \brief A timestamp which shows the number of milliseconds that have passed since last reset at the time of reception of this frame.
     */
    time_t tReceived;
} ethernetFrame_t;

/**
 * \brief Represents a complete Ethernet connection
 */
typedef struct ethernetConnection {
    linkState_t link; ///< Tells you if there is currently an Ethernet partner connected (Link established)
    speed_t speed; ///< Tells you the current speed of the connection 10M/100M
    duplexState_t duplex; ///< Tells you if the connection is full or half duplex
    macaddress_t source; ///< Hardware source address 
    macaddress_t destination; ///< Hardware destination address
} ethernetConnection_t;

/*\}*/

#endif	/* ETHERNETTYPES_H */