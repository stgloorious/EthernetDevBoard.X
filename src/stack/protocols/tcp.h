/** 
 * \file tcp.h
 * \brief Transmission Control protocol
 * \author Stefan Gloor
 * \version 1.0
 * \date  19. February 2019
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

#ifndef TCP_H
#define	TCP_H

#include "tcpTypes.h"
#include "tcpSettings.h"
#include "../../mem/memoryTypes.h"
#include "../error.h"
#include "ipv4Types.h"
#include "../../system/uart.h"

error_t tcp_handleNewPacket(ipv4_header_t header, memoryField_t ipPayload);

error_t tcp_parseHeader(memoryField_t ipPayload, tcp_header_t* header);

error_t tcp_stateMachine(tcp_connection_t *connection, tcp_header_t *newHeader);

error_t tcp_listen(tcp_connection_t *connection);

error_t tcp_sendSYNACK(tcp_connection_t *connection, uint32_t seq);

error_t static tcp_calculateHeaderChecksum(tcp_header_t *tcpHeader, ipv4_header_t ipv4Header);

void tcp_init();


#endif	/* TCP_H */

