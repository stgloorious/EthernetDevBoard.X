/**
 * \file sevenseg.h
 * \brief Function definitions for the 7-Segment driver
 * \author Stefan Gloor
 * \version 1.0
 * \date  28. Dezember 2018
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

#ifndef SEVENSEG_H
#define	SEVENSEG_H

#include <stdint.h>

// Seven segment display
#define SEVEN_SEG_TRIS				TRISA
#define SEVEN_SEG_ANSEL             ANSELA
#define SEVEN_SEG_PORT				PORTA
#define SET_SEVEN_SEG_TRIS			SEVEN_SEG_TRIS &=~0xff; //everything configured as an output
#define SET_SEVEN_SEG_ANSEL         SEVEN_SEG_ANSEL&=~0xff;

#define DIGIT_SELECT_PORT			PORTC
#define DIGIT_SELECT_TRIS			TRISC
#define DIGIT_SELECT_ANSEL          ANSELC
#define SET_SEVEN_SEG_DIGITS_TRIS	DIGIT_SELECT_TRIS&= ~0x07;
#define SET_SEVEN_SEG_DIGITS_ANSEL  DIGIT_SELECT_ANSEL&= ~0x07;


uint8_t sevenSegCharacters[12] = {
//   dpgfedcba
    0b00111111, //0
    0b00000110, //1
    0b01011011, //2
    0b01001111, //3
    0b01100110, //4
    0b01101101, //5
    0b01111101, //6
    0b00000111, //7
    0b01111111, //8
    0b01101111, //9
    0b00000000, //blank
    0b01000000, //-
};
#define SEVEN_SEG_DP		0b10000000

/**
 * \brief Initialises the seven segment display
 */
void sevenSegmentInit();

/**
 * \brief Does the multiplexing and updates the display; has to be called periodically.
 * \param numberToDisplay 
 */
void sevenSegmentUpdate(uint16_t numberToDisplay);

/**
 * \brief Parses number into individual digits
 * \param numberToDisplay   Number to parse
 * \param digit             Digit position
 * \return                  Digit value
 */
uint8_t static parseNumber(uint16_t numberToDisplay, uint8_t digit);

#endif	/* SEVENSEG_H */