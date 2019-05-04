/**
 * \file icmpSettings.h
 * \brief Settings  for Internet control message protocol
 * \author Stefan Gloor
 * \version 1.0
 * \date 21. April 2019
 * \ingroup protocols
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

#ifndef ICMPSETTINGS_H
#define	ICMPSETTINGS_H

#ifndef ICMP_DEBUG_MESSAGES
#define ICMP_DEBUG_MESSAGES     false
#endif

#define ICMP_MAX_LENGTH         128     ///< Maximum accepted length of the icmp message (header+payload)

#endif /* ICMPSETTINGS_H */