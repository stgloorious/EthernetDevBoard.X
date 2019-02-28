/**
 * \file uart.h
 * \brief Function definitions for UART
 * \author Stefan Gloor
 * \version 1.0
 * \date  28. Dezember 2018
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

#ifndef UART_H
#define	UART_H

#include <xc.h>
#include <stdint.h>
#include "../stack/protocols/arpTypes.h"
#include "../eth/mac.h"
#include "../stack/protocols/ipv4.h"



/**
 * \brief Initialises the UART module and sets up baud rate and other settings
 */
void UARTInit(void);

/**
 * \brief Transmits a char string over the UART interface
 * \param str Char string to be transmitted
 */
void UARTTransmitText(const char *str);

/**
 * \brief Intended to be used for debug messages only, calls UART text function
 * \param text Char String
 */
void DEBUG_MSG(char *text);

/**
 * \brief Transmits a four-digit numerical value
 * \param val Number to be transmitted
 */
void UARTTransmitInt(uint32_t val);

/**
 * \brief Converts a numerical value into a string
 * \param val
 * \return char string
 */
char *intToString(uint32_t val);

/**
 * \brief Takes a MAC address and translates it to a human-readable char string
 * \param mac MAC address
 * \return Char string
 */
char *macToString(macaddress_t addr);

/**
 * \brief Takes the etherType field value and converts it into human-readable string (e.g. "IPv4")
 * \param ethertype
 * \return Char string
 */
char *etherTypeToString(etherType_t ethertype);

/**
 * \brief Takes an IPv4 Address and converts it into string (decimal notation)
 * \param IP address
 * \return char string
 */
char *ipAdressToString(ipv4_address_t ip);

/**
 * \brief Converts an ARP entry to a string
 * \param table
 * \return String
 */
char *arpEntryToString(ARP_tableEntry_t table);


#endif	/* UART_H */
