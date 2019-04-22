/**
 * \file icmpTypes.h
 * \brief Type definitions used for Internet control message protocol
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

#ifndef ICMPTYPES_H
#define	ICMPTYPES_H

#include <stdint.h>

typedef struct icmpHeader {
    uint8_t typeOfMessage;
    uint8_t code;
    uint16_t checksum;
    uint32_t headerData;
} icmpHeader_t;

typedef enum icmpMessage {
    ICMP_ECHO_REPLY = 0,
    ICMP_DESTINATION_UNREACHABLE = 3,
    ICMP_SOURCE_QUENCH = 4,
    ICMP_REDIRECT_MESSAGE = 5,
    ICMP_ECHO_REQUEST = 8,
    ICMP_ROUTER_ADVERTISEMENT = 9,
    ICMP_ROUTER_SOLICITATION = 10,
    ICMP_TIME_EXCEEDED = 11,
    ICMP_PARAM_PROBLEM_BAD_HEADER = 12,
    ICMP_TIMESTAMP = 13,
    ICMP_TIMESTAMP_REPLY = 14
} icmpMessage_t;

const char *icmpTxtMessages[] = {
    "ECHO REPLY", //0
    "", //Spaceholder so the indices match the enum icmpMessage
    "", //2
    "DESTINATION UNREACHABLE", //3
    "SOURCE QUENCH", //4
    "REDIRECT MESSAGE", //5
    "", //6
    "", //7
    "ECHO REQUEST", //8
    "ROUTER ADVERTISEMENT", //9
    "ROUTER SOLICITATION", //10
    "TIME EXCEEDED", //11
    "PARAM PROBLEM: BAD HEADER", //12
    "TIMESTAMP", //13
    "TIMESTAMP REPLY"//14
};


#endif /* ICMPTYPES_H */