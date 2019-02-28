/** 
 * \file rsv.h
 * \brief Data type for the Receive Status Vector (RSV) of the ENC424J600
 * \author Stefan Gloor
 * \version 1.0
 * \date  19. Februar 2019
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

#ifndef RSV_H
#define	RSV_H

#include <stdint.h>

/**
 * \brief This structure represents the Receive Status Vector in a nice way.
 * \ingroup rsv
 */
typedef struct RSV {
    /**
     * \details  Length of the received frame in bytes. This includes the
     * destination address, source address, type/length, data, padding
     * and CRC fields. This field is stored in little-endian format.
     */
    uint16_t length;
    uint8_t packetPreviouslyIgnored : 1; ///< A frame larger than 50,000 bit times occurred or a packet has been dropped since the last receive.
    /**
     *  A carrier event was detected at some time since the last receive.
     * The carrier event is not associated with this packet. A carrier event
     * is activity on the receive channel that does not result in a packet
     * receive attempt being made.
     */
    uint8_t carrierEventPreviouslySeen : 1;
    uint8_t CRCError : 1; ///< Frame CRC field value does not match the CRC calculated by the MAC.
    uint8_t lengthCheckError : 1; ///< Frame length field value in the packet does not match the actual byte length and specifies a valid length.
    uint8_t lengthOutOfRange : 1; ///<  Frame type/length field was larger than 1500 bytes (type field).
    uint8_t receivedOk : 1; ///<  Received packet had a valid CRC and no symbol errors.
    uint8_t multicast : 1; ///<  Current frame has a valid Multicast address.
    uint8_t broadcast : 1; ///<  Current frame has a valid Broadcast address.
    uint8_t dribbleNibble : 1; ///<  Indicates that after the end of this packet, an additional 1 to 7 bits were received. The extra bits were thrown away.
    uint8_t controlFrame : 1; ///<  Current frame was recognized as a control frame for having a valid type/length designating it as a control frame.
    uint8_t pauseFrame : 1; ///<  Current frame was recognized as a control frame containing a valid pause frame opcode and a valid destination address.
    uint8_t unknownOpcode : 1; ///<  Current frame was recognized as a control frame but it contained an unknown opcode.
    uint8_t vlan : 1; ///< Current frame was recognized as a VLAN tagged frame.
    uint8_t runtFilter : 1; ///< Current frame met criteria for the Runt Packet Receive filter.
    uint8_t notMeFilter : 1; ///< Current frame met criteria for the Not-Me Receive filter.
    uint8_t hashFilter : 1; ///<  Current frame met criteria for the Hash Receive filter as configured when the packet was received.
    uint8_t magicFilter : 1; ///< Current frame met criteria for the Magic Packet Receive filter as configured when the packet was received.
    uint8_t patternMatch : 1; ///<  Current frame met criteria for the Pattern Match Receive filter as configured when the packet was received.
    uint8_t unicast : 1; ///<  Current frame met criteria for the Unicast Receive filter.
} RSV_t;

/**
 * \addtogroup rsvBits Receive Status Vector Bit Masks
 * \ingroup rsv
 * \details Used by \ref ENC424J600_updateReceiveStatusVector () which takes in the "raw" vector 
 *  and returns it in a more readable \ref RSV_ structure.
 * \see Table 9-1, Page 91 in ENC424J600's datasheet.
 * \todo I don't really like how this looks
 * \{
 */
#define RSV_BYTE2_PACKET_PREVIOUSLY_IGNORED         0b00000001
//#define RESERVED                                  0b00000010
#define RSV_BYTE2_CARRIER_EVENT_PREVIOUSLY_SEEN     0b00000100
//#define RESERVED                                  0b00001000
#define RSV_BYTE2_CRC_ERROR                         0b00010000
#define RSV_BYTE2_LENGTH_CHECK_ERROR                0b00100000
#define RSV_BYTE2_LENGTH_OUT_OF_RANGE               0b01000000
#define RSV_BYTE2_RECEIVED_OK                       0b10000000

#define RSV_BYTE3_RECEIVED_MULTICAST_PACKET         0b00000001
#define RSV_BYTE3_RECEIVED_BROADCAST_PACKET         0b00000010
#define RSV_BYTE3_DRIBBLE_NIBBLE                    0b00000100
#define RSV_BYTE3_RECEIVED_CONTROL_FRAME            0b00001000
#define RSV_BYTE3_RECEIVED_PAUSE_CONTROL_FRAME      0b00010000
#define RSV_BYTE3_RECEIVED_UNKNOWN_OPCODE           0b00100000
#define RSV_BYTE3_RECEIVED_VLAN                     0b01000000
#define RSV_BYTE3_RUNT_FILTER_MATCH                 0b10000000

#define RSV_BYTE4_NOT_ME_FILTER_MATCH               0b00000001
#define RSV_BYTE4_HASH_FILTER_MATCH                 0b00000010
#define RSV_BYTE4_MAGIC_PACKET_FILTER_MATCH         0b00000100
#define RSV_BYTE4_PATTERN_MATCH_FILTER_MATCH        0b00001000
#define RSV_BYTE4_UNICAST_FILTER_MATCH              0b00010000
//#define RESERVED                                  0b00100000
//#define RESERVED                                  0b01000000
//#define ZERO                                      0b10000000

//#define ZERO                                      0b00000001
//#define ZERO                                      0b00000010
//#define ZERO                                      0b00000100
//#define ZERO                                      0b00001000
//#define ZERO                                      0b00010000
//#define ZERO                                      0b00100000
//#define ZERO                                      0b01000000
//#define ZERO                                      0b10000000
/** \} */

#endif	/* RSV_H */

