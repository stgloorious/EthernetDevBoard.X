/**
 * \file udp.h
 * \brief User datagram protocol
 * \author Stefan Gloor
 * \version 1.0
 * \date 4. May 2019
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

#ifndef UDP_H
#define	UDP_H

#include "udpTypes.h"
#include "../../mem/memoryTypes.h"
#include "../error.h"
#include "ipv4.h"
#include "ipv4Types.h"


error_t udp_handleNewPacket(ipv4_header_t ipHeader, memoryField_t ipPayload);

error_t udp_sendPacket(udpHeader_t header, ipv4_address_t dst);

void static udp_calculateHeaderChecksum(udpHeader_t *header);


#endif	/* UDP_H */

