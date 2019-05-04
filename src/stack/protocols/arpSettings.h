/** 
 * \file arpSettings.h
 * \brief Timing constants and general settings
 * \details Timing constants according to RFC 5227 "IPv4 Address Conflict Detection", Page 5.
 * \warning These constants require a time measurement in milliseconds.
 * \see RFC 5227
 * \author Stefan Gloor
 * \version 1.0
 * \date 1. Februar 2019
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

#ifndef ARPSETTINGS_H
#define	ARPSETTINGS_H

/** \addtogroup arpSettings Settings
 *  \ingroup arp
 *  \details These settings allow detailed configuration of
 *  how the ARP operates.
 * \{
 */

/** \addtogroup arpTiming Timing settings
 *  \see Mostly taken from RFC 5227
 *  \ingroup arpSettings
 * \{
 */
#define ARP_PROBE_WAIT          1000u    ///< Initial random delay
#define ARP_PROBE_NUM           3u       ///< Number of probe packets
#define ARP_PROBE_MIN           1000u    ///< Minimum delay until repeated probe
#define ARP_PROBE_MAX           2000u    ///< Maximum delay until repeated probe
#define ARP_ANNOUNCE_WAIT       2000u    ///< Delay before announcing
#define ARP_ANNOUNCE_NUM        2u       ///< Number of announcement packets
#define ARP_ANNOUNCE_INTERVAL   2000u    ///< Time between announcement packets
#define ARP_MAX_CONFLICTS       10u      ///< Max conflicts before rate-limit
#define ARP_RATE_LIMIT_INTERVAL 60000u   ///< Delay between successive attempts
#define ARP_DEFEND_INTERVAL     10000u   ///< Minimum interval between defensive ARPs
#define ARP_REQUEST_NUM         3u       ///< Maximum number of requests that are sent trying to resolve an address
#define ARP_REQUEST_WAIT        2000u    ///< Delay after a request has been sent
#define ARP_TABLE_LENGTH        8u       ///< Number of entries in the ARP table
#define ARP_TABLE_ENTRY_TTL     30000u   ///< Time to live for an ARP Entry in milliseconds

#if ARP_TABLE_LENGTH > 255
#error "ARP_TABLE_LENGTH too large"
#endif

/**
 * \}
 */

#ifndef ARP_DEBUG_MESSAGES
#define ARP_DEBUG_MESSAGES      false    ///< This enables ALL debug messages related to the ARP
#endif   

/**
 * \brief Replies are sent using a link-level broadcast and not unicast
 * \details Enabling this option leads to a quicker conflict detection, but does also increase traffic.
 * \warning NOT RECOMMENDED for general use, but other specification may choose to use broadcast for ARP replies if appropriate.
 * \see RFC 5227, Section 2.6, Page 14f.
 */
#define ARP_LINK_BROADCAST      true     

#define ARP_IPv4_PLEN           4u       ///< Number of bytes in an IPv4 address; Not made to be changed.
#define ARP_ETHERNET_HLEN       6u       ///< Number of bytes in a MAC address; Not made to be changed.

/**\}*/

#endif	/* ARPSETTINGS_H */