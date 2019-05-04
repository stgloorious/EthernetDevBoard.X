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
#include <stdlib.h>
#include <stdint.h>
#include "../stack/protocols/arpTypes.h"
#include "../eth/mac.h"
#include "../stack/protocols/ipv4.h"

typedef enum UART_colors {
    UART_COLOR_BG_RED,
    UART_COLOR_BG_GREEN,
    UART_COLOR_BG_BLUE,
    UART_COLOR_BG_YELLOW,
    UART_COLOR_BG_MAGENTA,
    UART_COLOR_BG_CYAN,

    UART_COLOR_FG_YELLOW,
    UART_COLOR_FG_BLUE
} UART_colors_t;

enum UART_specials {
    UART_LINE_SEPARATOR
};

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
 * \brief Returns a special string, given an input code
 * \details Used to save program memory: With this e.g. a line separator needs to be saved only once.
 * \param code UART_specials
 * \return string
 */
char *UART_special(uint8_t code);

/**
 * Sets a color using ANSI formatting
 * @param color from UART_colors
 */
void UART_setFormat(uint8_t color);

/**
 * \brief Resets the current ANSI formatting
 */
void UART_resetFormat();

/**
 * \brief Converts a numerical value into a string
 * \details This function calls itoa() (non-standard)
 * \param val
 * \param b Base (10=decimal, 16=hex etc.)
 * \return char string
 */
char *intToString(uint32_t val, uint8_t b);

/**
 * \brief Takes a MAC address and translates it to a human-readable char string
 * \param addr MAC address
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
 * \brief Takes the protocol field of an ipv4 header and returns the protocol name (e.g. "TCP")
 * \param protocol
 * \return Char string
 */
char *ipProtocolToString(ipv4_protocol_t protocol);

/**
 * \brief Takes an IPv4 Address and converts it into string (decimal notation)
 * \param ip address
 * \return char string
 */
char *ipAdressToString(ipv4_address_t ip);

/**
 * \brief Converts an ARP entry to a string
 * \param table
 * \return String
 */
char *arpEntryToString(arp_tableEntry_t table);


#endif	/* UART_H */