/** 
 * \file ipv4Settings.h
 * \brief Settings for IPv4
 * \author Stefan Gloor
 * \version 1.0
 * \date  31. March 2019
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

#ifndef IPV4SETTINGS_H
#define	IPV4SETTINGS_H

#include "../bool.h"

/**
 * \addtogroup ipv4Settings Settings
 * \ingroup ipv4
 * \{
 */

#ifndef IPv4_DEBUG_MESSAGES
#define IPv4_DEBUG_MESSAGES             false  ///< This enables ALL debug messages related to IPv4
#endif 

#ifndef IPv4_DEBUG_HIGH_PRIORITY
#define IPv4_DEBUG_HIGH_PRIORITY        true  ///< This enables debug messages related to IPv4 that are marked as "high priority"; This setting is useless if \ref IPv4_DEBUG_MESSAGES is true. 
#endif

#define IPv4_ENABLE_FRAGMENTING         true ///< TX Packets larger than the MTU will be fragmented

/**\}*/

#endif	/* IPV4SETTINGS_H */