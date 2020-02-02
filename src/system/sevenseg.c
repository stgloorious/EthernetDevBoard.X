/**
 * \file sevenseg.c
 * \brief Drives the on-board multiplexed 3-digit 7-Segment display
 * \author Stefan Gloor
 * \version 1.0
 * \date  28. December 2018
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

#include <xc.h>
#include "../system/main.h"
#include "../system/sevenseg.h"

void sevenSegmentInit() {
    //Turn the digits off prior to setting TRIS registers
    DIGIT_SELECT_PORT &= ~(0x07);
    //Set TRIS registers
    SET_SEVEN_SEG_DIGITS_TRIS;
    SET_SEVEN_SEG_TRIS;

    SET_SEVEN_SEG_ANSEL;
    SET_SEVEN_SEG_DIGITS_ANSEL;

    SEVEN_SEG_PORT = 0xff; //everything off (inverted logic)
}

void sevenSegmentUpdate(uint16_t numberToDisplay) {
    //function has to be called periodically, time-critical
    uint8_t static digitPointer = 0; //points to the currently active digit
    uint8_t static digitsToDisplay[3] = {0, 0, 0}; //number we want to display, split up into digits
    DIGIT_SELECT_PORT &= ~(unsigned)(1 << digitPointer); //turn previous digit off
    SEVEN_SEG_PORT = 0x00; //everything off 
    digitPointer = (unsigned)((digitPointer >= 2) ? 0 : digitPointer + 1); //increment pointer
    //it's inefficient to do that for every digit, but this way it's consistent (consistent brightness)
    if (numberToDisplay > 999) {//number too big to display
        return;
    }
    for (uint8_t i = 0; i < 3; i++) {//go through the digits
        digitsToDisplay[i] = parseNumber(numberToDisplay, i);
    }
    DIGIT_SELECT_PORT |= (unsigned)(1 << digitPointer); //turn current digit on
    SEVEN_SEG_PORT = (sevenSegCharacters[digitsToDisplay[digitPointer]]); //set segments (inverted logic)
}

uint8_t static parseNumber(uint16_t numberToDisplay, uint8_t digit) {
    //returns desired single digit of a given number
    uint8_t hundreds = numberToDisplay / 100u;
    uint8_t tens = (numberToDisplay - hundreds * 100u) / 10u;
    uint8_t ones = (numberToDisplay - hundreds * 100u - tens * 10u);
    switch (digit) {
        case 2:
            return ones;
        case 1:
            return tens;
        case 0:
            return hundreds;
        default: return 0;
    }
}
