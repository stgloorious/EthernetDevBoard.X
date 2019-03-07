/**
 * \file stack.h
 * \brief Top-level stuff of the Stack
 * \author Stefan Gloor
 * \version 1.0
 * \date 27. Januar 2019
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
    ethernetConnection_t ethernet;
    backgroundTaskHandler_t volatile background;
    ethernetFrame_t newReceivedFrame;
    ipv4_packet_t pendingPacketToSend;
} stack_t;



/**\}*/

#endif	/* STACK_H */

