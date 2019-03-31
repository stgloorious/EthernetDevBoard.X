/**
 * \file stack.h
 * \brief Top-level stuff of the Stack
 * \author Stefan Gloor
 * \version 1.0
 * \date 27. Januar 2019
 * \ingroup stack
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

#ifndef STACK_H
#define	STACK_H

#include <stdint.h>

#include "../eth/ethernetTypes.h"
#include "../stack/backgroundTasksTypes.h"
#include "../stack/protocols/ipv4.h"



/**
 * \defgroup stack Stack
 * \{
 */

/**
 * \brief Top-level structure of the entire stack.
 */
typedef struct stack {
    backgroundTaskHandler_t background; ///< Stuff that happens in the background, like sending ARP requests


    ethernetConnection_t ethernet; ///< The ethernet connection that uses the stack
    /**
     * \bug Pointer (?) error: If there is no foo element which uses at least 4 bytes, the next member of the stack structure (background) will be corrupted 
     */
    uint32_t foo[3];

    ethernetFrame_t newReceivedFrame; ///< A newly received frame that is next being processed
    ipv4_packet_t pendingPacketToSend; ///< The packet that is currently being prepared for transmission
    ipv4_address_t source; ///< The IPv4 address of the stack
} stack_t;

/**
 * \brief Initialises all protocols 
 * \note To be called after system reset
 * \return
 */
error_t stack_init();

/**\}*/

#endif	/* STACK_H */