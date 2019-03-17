/**
 * \file backgroundTasks.h
 * \brief Functions should be run in free time in the background to do various stuff
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

#include <stdint.h>
#include "../eth/ethernetController.h"
#include "../stack/protocols/ipv4.h"
#include "../eth/ethernet.h"
#include "../stack/error.h"
#include "../stack/stack.h"
#include "../stack/backgroundTasksTypes.h"


#ifndef BACKGROUNDTASKS_H
#define	BACKGROUNDTASKS_H

/**
 * \brief To be called periodically in main() function.
 * \param stack Pointer to the Stack
 * \ingroup background
 */
void handleStackBackgroundTasks (stack_t *stack);

#endif	/* BACKGROUNDTASKS_H */