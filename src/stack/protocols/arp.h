/**
 * \file arp.h
 * \brief Function definitions used for Address Resolution Protocol (ARP)
 * \author Stefan Gloor
 * \version 1.0
 * \date 1. February 2019
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

#ifndef ARP_H
#define	ARP_H

#include <stdint.h>
#include "../../eth/ethernetController.h"
#include "../../eth/mac.h"
#include "../protocols/ipv4.h"
#include "../../eth/ethernet.h"
#include "../../system/uart.h"
#include "../protocols/arpTypes.h"

/**
 * \addtogroup arp Address Resolution Protocol
 * \ingroup protocols
 */

/**
 * \brief Initialises the ARP table. 
 * \ingroup arp
 * \note To be called after reset.
 * \details IP and MAC addresses are set to 0:0:0:0 / 00:00:00:00:00:00, respectively. 
 * The timestamp is set to a maximum value so it will always be consired expired.
 */
void ARP_initTable();

/**
 * \brief Takes a received ARP packet and does the necessary steps. 
 * \ingroup arp
 * \note To be called after a new packet was identified as an ARP message.
 * \param [in] *frame Pointer to the received Ethernet Packet
 */
void ARP_handleNewPacket(ethernetFrame_t *frame);

/**
 * \brief Takes the received packet and puts it into a more readable format.
 * \ingroup arp
 * \param [in] *frame Pointer to the received Ethernet Packet
 * \return ARPmessage data structure 
 */
ARP_message_t static ARP_parseFromRXBuffer(ethernetFrame_t *frame);

/**
 * \brief If the ARPmessage contains a request for my address, this function replies accordingly
 * \ingroup arp
 * \param request New message (parseFromRXBuffer() first).
 */
void ARP_sendReply(ARP_message_t request);

/**
 * \brief Sends the given message
 * \ingroup arp
 * \param arp message to send
 */
void static ARP_send(ARP_message_t arp);

/**
 * \brief Sends an ARP request to a given IPv4 address
 * \ingroup arp
 * \param ipSender
 * \param ipTarget
 */
void ARP_sendRequest(ipv4_address_t ipSender, ipv4_address_t ipTarget);

/**
 * \brief
 * \param ipTarget
 * \return 
 */
error_t ARP_probe (ipv4_address_t ipTarget);

/**
 * \brief Checks for an entry in the ARP table
 * \ingroup arp
 * \param ip IP Address to check
 * \param [out] *index Index of the entry if one was found
 * \return Wheter or not there is a valid (non-expired) entry in the table
 */
uint8_t ARP_checkForEntry(ipv4_address_t ip, uint8_t *index);

/**
 * \brief Gets an entry from the ARP table if there is one (check prior to calling this function)
 * \ingroup arp
 * \param index Index of the entry whose MAC address should be returned
 * \return MAC address of the desired entry
 */
macaddress_t ARP_getEntryFromTable(uint8_t index);

/**
 * \brief Creates a new entry in the ARP table
 * \ingroup arp
 * \param mac MAC address of the new entry
 * \param ip IP address of the new entry
 * \param timestamp Current time (use getSeconds())
 */
void static ARP_setNewEntry(macaddress_t mac, ipv4_address_t ip, uint32_t timestamp);

/**
 * \brief Used for debugging. Prints out ARP table via UART.
 * \ingroup arp
 * \todo Remove this later
 */
void printArpTable();


#endif	/* ARP_H */

