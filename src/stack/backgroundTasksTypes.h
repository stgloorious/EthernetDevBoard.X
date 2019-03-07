/**
 * \file backgroundTasksTypes.h
 * \brief Contains the data structure the background task handler uses
 * \author Stefan Gloor
 * \version 1.0
 * \date 19. February 2019
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

#ifndef BACKGROUNDTASKSTYPES_H
#define	BACKGROUNDTASKSTYPES_H

#include "bool.h"
#include "../stack/error.h"
#include "../enc424j600/interrupt.h"

/**
 * \addtogroup background Background Tasks
 * \ingroup stack
 * \{
 */

typedef struct backgroundTaskHandler {
    /**
     * \brief Error structure of all background tasks
     */
    error_t err;
    
    /**
     * \brief Interrupt flags are polled periodically and stored into this dedicated data structure.
     */
    interruptFlags_t interruptFlags;
    
    /**
     * \brief Indicates if there is a pending packet to send. This flag is reset when the transmission is complete.
     * \todo Can somebody pls tell me why bool_t doesn't work here; A bitfield gives really strange behaviour (?)
     */
    uint8_t fPacketPending;
} backgroundTaskHandler_t;

/**\}*/

#endif	/* BACKGROUNDTASKSTYPES_H */
