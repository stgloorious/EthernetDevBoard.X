/** 
 * \file time.h
 * \brief Time base
 * \author Stefan Gloor
 * \version 1.0
 * \date 5. March 2019
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

#include <stdint.h>

#ifndef TIME_H
#define	TIME_H

typedef uint32_t time_t;

/**
 * \brief Increments a time counting value
 * \note Call this every 1 ms to keep time-sensitive functions up to date.
 */
void updateTime();

/**
 * \brief Returns the number of milliseconds passed since reset if \ref updateSeconds() was called regularly
 * \return Number of milliseconds
 */
time_t getMillis();

#endif	/* TIME_H */

