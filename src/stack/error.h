/**
 * \file error.h
 * \brief Defines all error messages
 * \author Stefan Gloor
 * \version 1.0
 * \date 17. February 2019
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

#ifndef ERROR_H
#define	ERROR_H

#include <stdint.h>

/**
 * \addtogroup error Error Handling
 * \ingroup stack
 * \{
 */


#define ERROR_CODE_SUCCESSFUL          (uint16_t)0

typedef enum error_modules {
    ERROR_MODULE_MEMORY,
    ERROR_MODULE_ETHERNET_CONTROLLER,
    ERROR_MODULE_ETHERNET,
    ERROR_MODULE_ARP,
    ERROR_MODULE_IPv4
} error_module_t;

enum module_offsets {
    ERROR_MEMORY_OFFSET = 100,
    ERROR_ETHERNET_OFFSET = 200,
    ERROR_ETHERNET_CONTROLLER_OFFSET = 300,
    ERROR_ARP_OFFSET = 400,
    ERROR_IPv4_OFFSET = 500
};

enum errors_memory {
    ERROR_MEMORY_OUT_OF_MEMORY = ERROR_MEMORY_OFFSET + 1,
    ERROR_MEMORY_UNKNOWN = ERROR_MEMORY_OFFSET + 99
};

enum error_ethernet {
    ERROR_ETHERNET_SYMBOL_ERRORS = ERROR_ETHERNET_OFFSET + 1,
    ERROR_ETHERNET_CRC_ERROR = ERROR_ETHERNET_OFFSET + 2
};

enum errors_ethernetController {
    //Device
    ERROR_ETHERNET_CONTROLLER_UNKNOWN_DEVICE_ID = ERROR_ETHERNET_CONTROLLER_OFFSET + 1,
    ERROR_ETHERNET_CONTROLLER_UNRESPONSIVE = ERROR_ETHERNET_CONTROLLER_OFFSET + 2,
    //Packet Reception
    ERROR_ETHERNET_CONTROLLER_PACKET_COUNTER_OVERFLOW = ERROR_ETHERNET_CONTROLLER_OFFSET + 3,
    //Misc
    ERROR_ETHERNET_CONTROLLER_UNKNOWN = ERROR_ETHERNET_CONTROLLER_OFFSET + 99
};

enum errors_arp {
    ERROR_ARP_WAITING_FOR_REPLY = ERROR_ARP_OFFSET + 1,
    ERROR_ARP_MAXIMUM_NUMBER_OF_REQUESTS_REACHED = ERROR_ARP_OFFSET + 2,
    ERROR_ARP_INVALID_VALUES = ERROR_ARP_OFFSET + 3,
    ERROR_ARP_UNKNOWN = ERROR_ARP_OFFSET + 99
};

enum errors_ipv4 {
    ERROR_IPv4_ADDRESS_ALREADY_IN_USE = ERROR_IPv4_OFFSET + 1,
    ERROR_IPv4_ADDRESS_CONFLICT_DETECTED = ERROR_IPv4_OFFSET + 2,
    ERROR_IPv4_UNKNOWN = ERROR_IPv4_OFFSET + 99
};

typedef struct error {
    error_module_t module;
    uint16_t code;
} error_t;

/**\}*/


#endif	/* ERROR_H */

