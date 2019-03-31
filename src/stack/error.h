/**
 * \file error.h
 * \brief Defines all error messages
 * \author Stefan Gloor
 * \version 1.0
 * \date 17. February 2019
 * \ingroup error
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
 * \note Many "error" states are also used as a status indicator, so an error code different than \ref ERROR_CODE_SUCCESSFUL
 * is not necessarily a bad thing. Example: \ref ERROR_ARP_WAITING .
 * \ingroup stack
 * \{
 */


#define ERROR_CODE_SUCCESSFUL          (uint16_t)0          ///< Returned when everything went fine (= No errors).
#define ERROR_CODE_UNDEFINED           (uint16_t)0xffff     ///< Used to initialize \ref error_t types where initialisation with \ref ERROR_CODE_SUCCESSFUL would be inappropriate.

/**
 * \brief Error modules are used to find out the source of an error
 */
typedef enum error_modules {
    ERROR_MODULE_MEMORY, ///< Memory field management
    ERROR_MODULE_ETHERNET_CONTROLLER, ///< Physical Ethernet controller chip
    ERROR_MODULE_ETHERNET, ///< Higher level ethernet stuff than the controller
    ERROR_MODULE_ARP, ///< IP Address resolving
    ERROR_MODULE_IPv4///< IP stuff
} error_module_t;

/**
 * \brief The use of offsets allow changes in the error code value order
 */
enum module_offsets {
    ERROR_MEMORY_OFFSET = 100,
    ERROR_ETHERNET_OFFSET = 200,
    ERROR_ETHERNET_CONTROLLER_OFFSET = 300,
    ERROR_ARP_OFFSET = 400,
    ERROR_IPv4_OFFSET = 500
};

/**
 * \brief Error definitions of memory related errors
 */
enum errors_memory {
    ERROR_MEMORY_OUT_OF_MEMORY = ERROR_MEMORY_OFFSET + 1, ///< The memory controller couldn't fit the requested field
    ERROR_MEMORY_UNKNOWN = ERROR_MEMORY_OFFSET + 99 ///< Something else is wrong; don't use.
};

/**
 * \brief Error definitions of ethernet related errors
 */
enum error_ethernet {
    ERROR_ETHERNET_SYMBOL_ERRORS = ERROR_ETHERNET_OFFSET + 1, ///< A received ethernet frame had symbol errors
    ERROR_ETHERNET_CRC_ERROR = ERROR_ETHERNET_OFFSET + 2 ///< The CRC checksum of a received frame was incorrect
};

/**
 * \brief Error definitions of ethernet controller related errors
 */
enum errors_ethernetController {
    //Device
    ERROR_ETHERNET_CONTROLLER_UNKNOWN_DEVICE_ID = ERROR_ETHERNET_CONTROLLER_OFFSET + 1, ///< The ethernet controller returned a different device ID than expected
    ERROR_ETHERNET_CONTROLLER_UNRESPONSIVE = ERROR_ETHERNET_CONTROLLER_OFFSET + 2, ///< The ethernet controller is not reachable
    //Packet Reception
    ERROR_ETHERNET_CONTROLLER_PACKET_COUNTER_OVERFLOW = ERROR_ETHERNET_CONTROLLER_OFFSET + 3, ///< There were too many unprocessed packets so the counter overflowed
    //Misc
    ERROR_ETHERNET_CONTROLLER_UNKNOWN = ERROR_ETHERNET_CONTROLLER_OFFSET + 99
};

/**
 * \brief Error definitions of the Address Resolution Protocol
 */
enum errors_arp {
    ERROR_ARP_WAITING = ERROR_ARP_OFFSET + 1, ///< Is currently sending probes or waits for a reply
    ERROR_ARP_MAXIMUM_NUMBER_OF_PROBES_REACHED = ERROR_ARP_OFFSET + 3, ///< The maximum number of probes were sent
    ERROR_ARP_MAXIMUM_NUMBER_OF_REQUESTS_REACHED = ERROR_ARP_OFFSET + 4, ///< All the requests were sent, no one answered
    ERROR_ARP_INVALID_VALUES = ERROR_ARP_OFFSET + 5, ///< A received message contained illegal values
    ERROR_ARP_IPv4_ADDRESS_CONFLICT = ERROR_ARP_OFFSET + 6, ///< There is a duplicate address on the network
    ERROR_ARP_CONNECTION_FAILED = ERROR_ARP_OFFSET + 7, ///< Ethernet Link was lost during operation
    ERROR_ARP_UNKNOWN = ERROR_ARP_OFFSET + 99 ///< Something else is wrong with ARP; don't use
};

/**
 * \brief Error definitions of the Internet Protocol Version 4
 */
enum errors_ipv4 {
    ERROR_IPv4_ADDRESS_ALREADY_IN_USE = ERROR_IPv4_OFFSET + 1,
    ERROR_IPv4_UNKNOWN = ERROR_IPv4_OFFSET + 99
};

/**
 * \brief Error code type
 * \see \ref error
 * 
 */
typedef struct error {
    /**
     * \brief Tells you where the error is coming from
     */
    error_module_t module;

    /**
     * \brief Tells you what the error is
     * \see Use the codes defined for individual modules in \ref error
     */
    uint16_t code;
} error_t;

/**\}*/


#endif	/* ERROR_H */