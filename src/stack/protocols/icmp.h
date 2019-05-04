/**
 * \file icmp.h
 * \brief Function definitions used for Internet control message protocol
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

#ifndef ICMP_H
#define	ICMP_H

#include "icmpTypes.h"
#include "icmpSettings.h"
#include "../../mem/memoryTypes.h"
#include "ipv4Types.h"
#include "../error.h"
#include "../../system/uart.h"

/**
 * 
 * @param header
 * @param payload
 * @return 
 */
error_t icmp_handleNewPacket(ipv4_header_t ipHeader, memoryField_t ipPayload);

/**
 * 
 * @param ipPayload
 * @param header
 * @return 
 */
error_t icmp_parseHeader(memoryField_t ipPayload, icmpHeader_t *ipHeader);

/**
 */
uint16_t static icmp_calculateChecksum(memoryField_t ipPayload);

void icmp_sendEchoReply(ipv4_header_t ipHeader, icmpHeader_t icmpHeader, memoryField_t ipPayload);

void icmp_writeHeaderIntoBuffer(uint8_t *buf, icmpHeader_t header);

uint16_t static icmp_calculateChecksumBuf(uint8_t *buf, uint16_t ipPayloadLength);

void icmp_sendEchoRequest(ipv4_address_t ipAddr);


#endif	/* ICMP_H */

