/** 
 * \file stack.c
 * \brief Function definitions for memory operations
 * \author Stefan Gloor
 * \version 1.0
 * \date  2. February 2019
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

#include "../stack/stack.h"

uint32_t static seconds = 0;

void updateSeconds() {
    uint16_t static prescaler_ms = 0;
    /**
     * \brief Increment the second counter every 244th cycle because 244*4096 us = 0.999424 seconds, close enough :)
     */
    if (prescaler_ms == 243) {
        prescaler_ms = 0;
        seconds++;
    } else {
        prescaler_ms++;
    }
}

uint32_t getSeconds() {
    return seconds;
}



