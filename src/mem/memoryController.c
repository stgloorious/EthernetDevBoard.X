/** 
 * \file memoryController.c
 * \brief Function definitions for memory operations
 * \author Stefan Gloor
 * \version 1.0
 * \date  3. February 2019
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

#include "../mem/memoryController.h"

memoryField_t static occupiedMemory[NUMBER_OF_MEMORY_FIELDS];
uint8_t static numberOfOccupiedFields = 0;

void initBuffer() {
	for (uint8_t i = 0; i < NUMBER_OF_MEMORY_FIELDS; i++) {
		occupiedMemory[i].fIsAssigned = 0;
		occupiedMemory[i].fOutOfMemory = 0;
	}
}

uint8_t entireMemoryEmpty() {
	for (uint8_t i = 0; i < NUMBER_OF_MEMORY_FIELDS;i++) {
		if (occupiedMemory[i].fIsAssigned)
			return 0;
	}
	return 1;
}

uint8_t entireMemoryFull() {
	for (uint8_t i = 0; i < NUMBER_OF_MEMORY_FIELDS;i++) {
		if (!occupiedMemory[i].fIsAssigned)
			return 0;
	}
	return 1;
}

uint8_t isOnlyOneAssigned(uint8_t index) {
	for (uint8_t i = 0; i < NUMBER_OF_MEMORY_FIELDS;i++) {
		if (i == index)
			continue;
		if (occupiedMemory[i].fIsAssigned)
			return 0;
	}
	return 1;
}

uint8_t static checkForOverlap(memoryField_t a, memoryField_t b) {
	if ((a.start == b.start) ||
		(a.end == b.end) ||
		(a.start == b.end) ||
		(a.end == b.start) ||
		(a.end > RX_DATA_START_ADDRESS) ||
		(a.start > RX_DATA_START_ADDRESS) ||
		(b.end > RX_DATA_START_ADDRESS) ||
		(b.start > RX_DATA_START_ADDRESS))
		return 1;


	if (a.start < a.end) {
		if (b.start < b.end) {
			//No wrap arounds
			if (a.start < b.start) {//b comes after a
				if (a.end > b.start) {//a ends after b starts
					return 1;//overlap!
				}
				else {//a ends before b starts
					return 0;
				}
			}
			else {
				return checkForOverlapSwitched(b, a);
			}
		}
		else {
			if (b.end < a.start)
				return 0;
			else
				return 1;
		}
	}
	else {
		return checkForOverlapSwitched(b, a);//yay, recursion!
	}
}

uint8_t static checkForOverlapSwitched(memoryField_t a, memoryField_t b){
    /* xc8 does not allow recursion, so we do it the stupid way */
    
    	if ((a.start == b.start) ||
		(a.end == b.end) ||
		(a.start == b.end) ||
		(a.end == b.start) ||
		(a.end > RX_DATA_START_ADDRESS) ||
		(a.start > RX_DATA_START_ADDRESS) ||
		(b.end > RX_DATA_START_ADDRESS) ||
		(b.start > RX_DATA_START_ADDRESS))
		return 1;


	if (a.start < a.end) {
		if (b.start < b.end) {
			//No wrap arounds
			if (a.start < b.start) {//b comes after a
				if (a.end > b.start) {//a ends after b starts
					return 1;//overlap!
				}
				else {//a ends before b starts
					return 0;
				}
			}
		}
		else {
			if (b.end < a.start)
				return 0;
			else
				return 1;
		}
	}
}

memoryField_t memory_txFrameRequest(uint16_t length) {
	memoryField_t field;
	uint8_t freeIndex = 0;

	if (length == 0) {
		field.fOutOfMemory = 1;
		return field;
	}

	if (entireMemoryFull()) {//checked all fields, none is empty or large enough
		field.fOutOfMemory = 1;
		return field;
	}
	if (entireMemoryEmpty()) {
		//there is no field that is assigned
		if ((TX_DATA_START_ADDRESS + length) > RX_DATA_START_ADDRESS - 1) {
			field.fOutOfMemory = 1;
			return field;
		}
		else {
			occupiedMemory[0].start = TX_DATA_START_ADDRESS;
			occupiedMemory[0].end = TX_DATA_START_ADDRESS + length;
			occupiedMemory[0].length = length;
			occupiedMemory[0].fIsAssigned = 1;
            occupiedMemory[0].index=0;
			return occupiedMemory[0];
		}
	}

	while (freeIndex <= NUMBER_OF_MEMORY_FIELDS) {//criteria is always met
		//Cycle through all possible memory fields, return if one is found or all have been checked
		if (!occupiedMemory[freeIndex].fIsAssigned) {//check for free fields
			//We have found a memory field that is not assigned
			//Let's get the end address of the previous field (notice it may be the 0th field, so we have to wrap around)
			//Put the start address of the current field next to the end address of the previous field
			occupiedMemory[freeIndex].start = occupiedMemory[(freeIndex == 0) ? NUMBER_OF_MEMORY_FIELDS - 1 : freeIndex - 1].end + 1;
			if (occupiedMemory[freeIndex].start + length > RX_DATA_START_ADDRESS) {
				if (freeIndex == NUMBER_OF_MEMORY_FIELDS - 1 && !occupiedMemory[0].fIsAssigned) {
					occupiedMemory[freeIndex].length = length;
					occupiedMemory[freeIndex].end = occupiedMemory[freeIndex].length - ((RX_DATA_START_ADDRESS - 1) - occupiedMemory[freeIndex].start);
					for (uint8_t i = 0; i < NUMBER_OF_MEMORY_FIELDS;i++) {
						if (i == freeIndex)
							continue;
						if (checkForOverlap(occupiedMemory[freeIndex], occupiedMemory[i]) && occupiedMemory[i].fIsAssigned) {
							//No more room!
							field.fOutOfMemory = 1;
							return field;
						}
					}
					occupiedMemory[freeIndex].fIsAssigned = 1;
                    occupiedMemory[freeIndex].index=freeIndex;
					return occupiedMemory[freeIndex];
				}
			}
			//check for any possible overlaps to any existing fields
			uint8_t fNoOverlapMarker = 1;
			uint8_t i;
			occupiedMemory[freeIndex].end = occupiedMemory[freeIndex].start + length;
			for (i = 0; i < NUMBER_OF_MEMORY_FIELDS; i++) {
				if (i == freeIndex)//don't check against itself
					continue;
				if (!occupiedMemory[i].fIsAssigned) {
					continue;
					//we don't care about fields that are not assigned
				}

				if (checkForOverlap(occupiedMemory[freeIndex], occupiedMemory[i])) 
					fNoOverlapMarker = 0;

			}
			if (fNoOverlapMarker) {
				occupiedMemory[freeIndex].fIsAssigned = 1;
				occupiedMemory[freeIndex].end = occupiedMemory[freeIndex].start + length;
				occupiedMemory[freeIndex].fOutOfMemory = 0;
				occupiedMemory[freeIndex].length = length;
                occupiedMemory[freeIndex].index=freeIndex;
				return occupiedMemory[freeIndex];
			}
		}
		freeIndex++;
		if (freeIndex == NUMBER_OF_MEMORY_FIELDS) {
			//No more room!
			field.fOutOfMemory = 1;
			return field;
		}
	}//end while(), never reached
}

void memory_txFrameClear(uint8_t index) {
	occupiedMemory[index].fIsAssigned = 0;
}





