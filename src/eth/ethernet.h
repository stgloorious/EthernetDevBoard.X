/** 
 * \file ethernet.h
 * \brief 
 * \author Stefan Gloor
 * \version 1.0
 * \date 16. February 2019
 * \ingroup ethernet
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

#ifndef ETHERNET_H
#define	ETHERNET_H

#include <stdint.h>
#include "../eth/ethernetController.h"
#include "../stack/error.h"
#include "../eth/ethernetTypes.h"
#include "../mem/memoryController.h"

/**
 * \addtogroup protocols Protocols
 * \ingroup ethernet
 * 
 */

/**
 * \addtogroup ethernet Ethernet
 * \ingroup stack
 */

/**
 * \brief Takes the necessary steps to send a packet.
 * \ingroup ethernet
 * \details First, this function requests a memory field from the memory controller.
 * For this, the argument pointer must point to an ethernetFrame which must contain the correct length of the frame.
 * It then writes the Destination MAC address and the EtherType field in the right location in the buffer.
 * \param [inout] *frame Packet to send
 * \return Error code 
 */
error_t ethernet_txFrameRequest(ethernetFrame_t *frame);

/**
 * \brief Takes care of the necessary steps to read a received packet.
 * \ingroup ethernet
 * \details This function figures out the memory start and end address of the packet that was received.
 * It then parses the Receive Status Vector and the EtherType field. From this, the appropriate function of 
 * the responsible upper laying protocol is called. (E.g. \ref arp_handleNewPacket)
 * \param [inout] *frame Received Packet
 * \return Error code
 */
error_t ethernet_rxGetNewFrame(ethernetFrame_t *frame);



#endif	/* ETHERNET_H */