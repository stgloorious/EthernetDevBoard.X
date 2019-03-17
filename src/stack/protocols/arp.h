/**
 * \file arp.h
 * \brief Function definitions used for Address Resolution Protocol (ARP)
 * \author Stefan Gloor
 * \version 1.0
 * \date 1. February 2019
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
#include "arpSettings.h"

/**
 * \addtogroup arp Address Resolution Protocol
 * \ingroup protocols
 * \{
 */

/**
 * \brief Initialises the ARP table. 
 * \note To be called after reset.
 * \details IP and MAC addresses are set to 0:0:0:0 / 00:00:00:00:00:00, respectively. 
 * The timestamp is set to a maximum value so it will always be consired expired.
 */
void arp_init();

/**
 * \brief Takes a received ARP packet and does the necessary steps. 
 * \note To be called after a new packet was identified as an ARP message.
 * \param [in] *frame Pointer to the received Ethernet Packet
 */
void arp_handleNewPacket(ethernetFrame_t *frame);

/**
 * \brief Takes the received packet and puts it into a more readable format.
 * \param [in] *frame Pointer to the received Ethernet Packet
 * \return The message that was received
 */
arp_message_t static arp_parseFromRXBuffer(ethernetFrame_t *frame);

/**
 * \brief If the ARPmessage contains a request for my address, this function replies accordingly
 * \param request New message (parseFromRXBuffer() first).
 */
void static arp_sendReply(arp_message_t request);

/**
 * \brief Sends the given message
 * \param arp message to send
 */
void static arp_send(arp_message_t arp);

/**
 * \brief Sends an ARP request to a given IPv4 address
 * \param ipSender
 * \param ipTarget
 */
void static arp_sendRequest(ipv4_address_t ipSender, ipv4_address_t ipTarget);

/**
 * \brief Sends probes to check for address conflicts
 * \details Sends out #ARP_PROBE_NUM probes, with an initial random delay (#ARP_PROBE_WAIT) and a
 * random delay between #ARP_PROBE_MIN and #ARP_PROBE_MAX after each probe. It will check each time if
 * there is a valid ARP entry (to detect if there was a reply).
 * \note This function has to be called repeatedly until it returns an error code different than #ERROR_ARP_WAITING .
 * \param ipTarget The IPv4 address being probed for
 * \return #ERROR_ARP_MAXIMUM_NUMBER_OF_PROBES_REACHED if there was no answer\n
 * #ERROR_ARP_WAITING if probing process is not done yet\n
 * #ERROR_ARP_IPv4_ADDRESS_CONFLICT if there was an ARP message with the requested IP address\n
 */
error_t static arp_probe(ipv4_address_t ipTarget);

/**
 * \brief Announces the usage of an ip address (Gratuitous ARP)
 * \note This function has to be called repeatetly until it returns #ERROR_CODE_SUCCESSFUL
 * According to RFC 5227, a gratuitous ARP announce MUST be performed after the probing of an address
 * \see RFC 5227, Section 2.3, Page 12
 * \param ip IP address
 * \return #ERROR_ARP_WAITING if there are more announcements to make \n
 * #ERROR_CODE_SUCCESSFUL if everything's done
 */
error_t static arp_gratuitous(ipv4_address_t ip);

/**
 * \brief To be called periodically in the background task handler
 * \details Does various stuff that takes some time to wait, e.g. waiting for an ARP reply
 * \return Errors that occurred during operation
 */
error_t arp_background();

/**
 * \brief Checks for an entry in the ARP table
 * \param ip IP Address to check
 * \param [out] *index Index of the entry if one was found
 * \return Whether or not there is a valid (non-expired) entry in the table
 */
uint8_t static arp_checkForEntry(ipv4_address_t ip, uint8_t *index);

/**
 * \brief Gets an entry from the ARP table if there is one (check prior to calling this function)
 * \param index Index of the entry whose MAC address should be returned
 * \note Find the index using \ref arp_checkForEntry()
 * \return MAC address of the desired entry
 */
macaddress_t static arp_getEntryFromTable(uint8_t index);

/**
 * \brief Creates a new entry in the ARP table
 * \param mac MAC address of the new entry
 * \param ip IP address of the new entry
 * \param timestamp Current time (use getSeconds())
 */
void static arp_setNewEntry(macaddress_t mac, ipv4_address_t ip, uint32_t timestamp);

/**
 * \brief Used for debugging. Prints out ARP table via UART.
 * \todo Remove this later
 */
void printArpTable();

/**\}*/


#endif	/* ARP_H */