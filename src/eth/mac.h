/** 
 * \file mac.h
 * \brief Function definitions for mac operations
 * \author Stefan Gloor
 * \version 1.0
 * \date  13. January 2019
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

#ifndef MAC_H
#define	MAC_H

#include <stdint.h>
#include "../stack/bool.h"
#include "ethernetTypes.h"

/**
 * \addtogroup mac_operations MAC Address Operations
 * \ingroup ethernet
 * \{
 */

/**
 * \brief Compares two MAC addresses and checks if they are equal
 * \param [in] *a
 * \param [in] *b
 * \return true if addresses are equal, otherwise false
 */
bool_t mac_cmp(macaddress_t *a, macaddress_t *b);

/**
 * \brief Sets the address to FF:FF:FF:FF:FF:FF
 * \param [out] *mac
 */
void mac_setToBroadcast(macaddress_t *mac);

/**
 * \brief Checks if the address is equal to FF:FF:FF:FF:FF:FF
 * \param [in] *mac
 * \return true if address is broadcast, otherwise false
 */
bool_t mac_isBroadcast(macaddress_t *mac);

/**
 * \brief Sets the address to 00:00:00:00:00:00
 * \param [out] *mac
 */
void mac_setAllZero(macaddress_t *mac);

/**
 * \brief Checks if the address is equal to 00:00:00:00:00:00
 * \param [in] *mac
 * \return true if address is all zero, otherwise false
 */

bool_t mac_isAllZero(macaddress_t *mac);

/**\}*/


#endif	/* MAC_H */