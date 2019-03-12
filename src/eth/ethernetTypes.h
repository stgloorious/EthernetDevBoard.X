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
    ETHERTYPE_IPv4 = 0x0800,
    ETHERTYPE_ARP = 0x0806,
    ETHERTYPE_WOL = 0x0842,
    ETHERTYPE_FLOW_CONTROL = 0x8808
} etherType_t;

/**
 */
typedef struct macaddress {
    uint8_t address[6];
} macaddress_t;

/**
 */
typedef enum linkStates {
    NO_LINK, LINK_ESTABLISHED
} linkState_t;

/**
 */
typedef enum speeds {
    TEN_MBIT, HUNDRED_MBIT
} speed_t;

/**
 */
typedef enum duplexStates {
    HALF_DUPLEX, FULL_DUPLEX
} duplexState_t;

/**
 * \brief Represents a complete Ethernet Frame
 */
typedef struct ethernetFrame {
    uint16_t length;
    macaddress_t source;
    macaddress_t destination;
    etherType_t ethertype;
    memoryField_t memory;
    /**
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
    linkState_t link;
    speed_t speed;
    duplexState_t duplex;
    macaddress_t source;
    macaddress_t destination;
} ethernetConnection_t;

/*\}*/

#endif	/* ETHERNETTYPES_H */

