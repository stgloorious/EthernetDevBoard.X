/** 
 * \file memoryController.h
 * \brief Function definitions for memory operations
 * \author Stefan Gloor
 * \version 1.0
 * \date  3. Februar 2019
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

#ifndef ENC424J600_MEMORY_H
#define	ENC424J600_MEMORY_H

#include <stdint.h>
#include "../mem/memoryTypes.h"

/**
 * \addtogroup memory Memory
 * \ingroup stack
 * \{
 */

#define NUMBER_OF_MEMORY_FIELDS     8 ///< Maximum number of packet memory fields that can be simultaneously assigned


#define TX_DATA_START_ADDRESS       0x0000     ///< The transmission buffer starts at this address
#define RX_DATA_START_ADDRESS       0x3000     ///< The reception buffer ranges from this address to the end of the memory
#define END_OF_MEMORY_ADDRESS       0x5FFF    ///< Size of the memory buffer


//Prevent stupid things
#if (TX_DATA_START_ADDRESS >= RX_DATA_START_ADDRESS)
#error ("TX_DATA_START_ADDRESS can't be greater than RX_DATA_START_ADDRESS");
#endif
#if (RX_DATA_START_ADDRESS % 2 == 1)
#error "RX_DATA_START_ADDRESS must be even."
#endif
#if (END_OF_MEMORY_ADDRESS % 2 == 0)
#error "END_OF_MEMORY_ADDRESS should be odd, shouldn't it?"
#endif


/**
 * \brief Returns an address where data of the given length can be stored
 * \param length Length of the packet to be sent
 * \return MemoryField where the packet can be written to
 */
memoryField_t memory_txFrameRequest(uint16_t length);

/**
 * \brief Free up a memoryField
 * \param index Index of the memoryField that should be cleared up
 */
void memory_txFrameClear(uint8_t index);

/**
 * \brief Checks if two (unassigned) fields overlap
 * \param a
 * \param b
 * \return 1 if they overlap, otherwise 0
 */
uint8_t static checkForOverlap(memoryField_t a, memoryField_t b);

/**
 * \brief The same as \ref checkForOverlap (), but a and b are switched
 * \note This is necessary because the xc8 Compiler does not support recursion.
 * \param a
 * \param b
 * \return 1 if they overlap, otherwise 0
 */
uint8_t static checkForOverlapSwitched(memoryField_t a, memoryField_t b);

/**\}*/


#endif	/* ENC424J600_MEMORY_H */

