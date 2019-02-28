/**
 * \file mac.c
 * \brief Function definitions for MAC Address operations
 * \author Stefan Gloor
 * \version 1.0
 * \date 29. Januar 2019
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

#include "mac.h"

void mac_setToBroadcast(macaddress_t *mac) {
    for (uint8_t i = 0; i < 6; i++)
        mac->address[i] = 0xff;
}

bool_t mac_isAllZero(macaddress_t* mac) {
    for (uint8_t i = 0; i < 6; i++) {
        if (mac->address[i] != 0x00)
            return 0;
    }
    return 1;
}

bool_t mac_cmp(macaddress_t* a, macaddress_t* b) {
    for (uint8_t i = 0; i < 6; i++) {
        if (a->address[i] != b->address[i])
            return 0;
    }
    return 1;
}

void mac_setAllZero(macaddress_t* mac) {
    for (uint8_t i = 0; i < 6; i++)
        mac->address[i] = 0x00;
}


