/**
 * \file bool.h
 * \brief Defines boolean type
 * \details Does the same as <stdbool.h> but with the possibility 
 * to use a bit variable (which is supported by xc8 Compiler and PIC MCUs) for 
 * increased memory efficiency.
 * \author Stefan Gloor
 * \version 1.0
 * \date 25. February 2019
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

#ifndef BOOL_H
#define	BOOL_H

#ifdef __XC
#advisory "Existence of bit variable is assumed because Microchip XC compiler is in use."
typedef bit bool_t;
#else 
#advisory "Existence of bit variable is not assumed."
typedef unsigned char bool_t;
#endif

#define true    1
#define false   0

#endif	/* BOOL_H */

