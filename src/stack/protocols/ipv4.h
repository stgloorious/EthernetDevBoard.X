/* 
 * \file ipv4.h
 * \brief Function definitions for IPv4
 * \author Stefan Gloor
 * \version 1.0
 * \date 29. January 2019
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

#ifndef IPV4_H
#define	IPV4_H

#include "../bool.h"
#include "../../eth/ethernetTypes.h"
#include "../protocols/ipv4Types.h"
#include "../protocols/arp.h"
#include "../error.h"
#include "../../eth/ethernetTypes.h"
#include "../stack.h"

/** \addtogroup ipv4 IPv4
 *  \ingroup protocols
 * \{
 */

/**
 * \brief Requests a memory field and prepares the transmission of an IPv4 Packet
 * \note To be called every time when an IPv4 Packet should be sent
 * \param [inout] *packet 
 */
void ipv4_txFrameRequest(ipv4_packet_t *packet);

/**
 * \brief Call this function after everything in the header has been filled in
 */
void ipv4_calculateHeaderChecksum(ipv4_header_t *header);

/**
 * \brief Checks if the received checksum is correct using \ref ipv4_calculateHeaderChecksum()
 * \param [in] header ipv4 header
 * \return True if correct, false if incorrect
 */
bool_t static ipv4_checkHeaderChecksum(ipv4_header_t *header);

/**
 * \brief Assembles the IPv4 Header and writes it into a buffer
 * \param header Header that gets written to the buffer
 * \param [out] *ptr Pointer to the first element of the buffer array
 * \warning Always make sure the array which ptr points to is big enough!
 */
void ipv4_writeHeaderIntoBuffer(ipv4_header_t header, uint8_t* ptr);

/**
 * \brief Streams data to the memory
 * \param data 
 * \param packet
 */
void ipv4_streamToTransmissionBuffer(uint8_t data, ipv4_packet_t packet);

/**
 * \brief Sends the prepared IPv4 Packet
 * \param ipPacket Packet
 * \return Error code
 */
error_t ipv4_sendFrame(ipv4_packet_t ipPacket);

/**
 * \brief Does all the necessary things to process a new packet
 * \note To be called if a received packet is identified as an IPv4 packet
 * \param [in] *frame The received packet
 */
void ipv4_handleNewPacket(ethernetFrame_t *frame);

/**
 * \brief Reads a field in memory where an ip header lies and puts the information in a nice structure
 * \param [in] field Memory field where the header is
 * \return ip header
 */
ipv4_header_t static ipv4_parseHeader(memoryField_t *field);

/**
 * 
 * @return 
 */
ipv4_address_t ipv4_getIPSourceAddress();

/**
 * 
 */
ipv4_address_t ipv4_getPreliminaryIPSourceAddress();

/**
 * 
 * @param ip
 */
void ipv4_setIPSourceAddress(ipv4_address_t ip);

/**
 * \brief To be called periodically in the background task handler
 * \details Does various stuff that takes some time to wait, e.g. handling ARP when setting a new ip address
 * \return Errors that occurred during operation
 */
error_t ipv4_background(linkState_t link);

/**
 * \brief Returns the \ref fSrcAddrChanged flag located in \ref ipv4.c
 * \note Used by ARP to check if it's necessary to send probes
 * \param [in] err At this point the pointer to the arp error structure is given so the ip module knows about its status
 * \return true if the ipv4 address has changed, false if not
 */
bool_t ipv4_SrcAddrChanged(error_t *err);

/**
 * 
 */
void ipv4_init();

/**\}*/

/** \addtogroup ip_operations IPv4 Address operations
 *  \ingroup ipv4
 * \{
 */

/**
 * \brief Compares two IPv4 addresses and checks if they are equal
 * \param a
 * \param b
 * \return true if they are equal, otherwise false
 */
bool_t ipv4_cmp(ipv4_address_t* a, ipv4_address_t *b);

/**
 * \brief Checks if the IP Address is 0.0.0.0
 * \param [in] *ip 
 * \return true if it is 0.0.0.0, otherwise false
 */
bool_t ipv4_isAllZero(ipv4_address_t *ip);

/**
 * \brief Sets the IP address to 0.0.0.0
 * \param [out] *ip Address which is set to 0.0.0.0
 */
void ipv4_setToAllZero(ipv4_address_t *ip);

/**\}*/

#endif	/* IPV4_H */