/** 
 * \file memoryTypes.h
 * \brief Data definitions for memory management
 * \author Stefan Gloor
 * \version 1.0
 * \date  19. Februar 2019
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
 * 
 */

#ifndef MEMORYTYPES_H
#define	MEMORYTYPES_H

#include <stdint.h>
#include "../stack/bool.h"

/**
 * \brief Represents a part of memory where a packet is stored
 * \ingroup memory
 */
typedef struct memoryField {
    /**
     * \brief Memory start address
     * \note Please take note that this field includes the NextPacketPointer and the RSV.
     * If you want to access the Dst Addr, you have to add an offset of 8 Bytes to the start address pointer.
     * For the Src Addr it's 12 Bytes and so on.
     */
    uint16_t start;
    uint16_t end; ///< Memory end address
    uint16_t length; ///< Length in bytes. The length does also include NextPacketPointer and RSV.
    uint8_t fIsAssigned; ///< 1: In use, 0: Free
    uint8_t fOutOfMemory;///< Error flag if there is no more room in the buffer
    /**
     * \brief Index number of the field so it can be identified when freeing memory up
     * \note Range is 0..\ref ARP_TABLE_LENGTH - 1
     */
    uint8_t index;///< 
} memoryField_t;

#endif	/* MEMORYTYPES_H */