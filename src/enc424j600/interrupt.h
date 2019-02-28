/**
 * \file interrupt.h
 * \brief Contains bit name definitions and the data structure for all the different interrupt flags of the ENC424J600
 * \author Stefan Gloor
 * \version 1.0
 * \date 23. February 2019
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

#ifndef INTERRUPT_H
#define	INTERRUPT_H

#include <stdint.h>

/**
 * \ingroup interrupts
 * \brief This structure represents the EIR Interrupt flag register
 * \details Note that the MSB (Bit 15) is not implemented here because it is not an interrupt flag
 */
typedef struct interruptFlags {
    uint8_t MODEXIF : 1; ///< [Bit 14] Modular Exponentiation Interrupt Flag bit
    uint8_t HASHIF : 1; ///< [Bit 13] MD5/SHA-1 Hash Interrupt Flag bit
    uint8_t AESIF : 1; ///< [Bit 12] AES Encrypt/Decrypt Interrupt Flag bit
    uint8_t LINKIF : 1; ///< [Bit 11] PHY Link Status Change Interrupt Flag bit
    //Bits 7..10 are reserved
    uint8_t PKTIF : 1; ///< [Bit 6] RX Packet Pending Interrupt Flag bit
    uint8_t DMAIF : 1; ///< [Bit 5] DMA Interrupt Flag bit
    //Bit 4 is reserved
    uint8_t TXIF : 1; ///< [Bit 3] Transmit Done Interrupt Flag bit
    uint8_t TXABTIF : 1; ///< [Bit 2] Transmit Abort Interrupt Flag bit
    uint8_t RXABTIF : 1; ///< [Bit 1] Receive Abort Interrupt Flag bit
    uint8_t PCFULIF : 1; ///< [Bit 0] Packet Counter Full Interrupt Flag bit
} interruptFlags_t;

/**
 * \ingroup interrupts
 * \brief Contains the interrupt flag names of the EIR Interrupt Flag register
 * \details The numbers represent the bit numbers. To be used with \ref ENC424J600_clearInterruptFlag ()
 * \note The flag names represent bit numbers, not masks.
 * \see Register 13-1, Page 120 of ENC424J600's data sheet.
 */
typedef enum interruptFlagsNames {
    PCFULIF = 0, ///< Packet Counter Full Interrupt Flag bit
    RXABTIF = 1, ///< Receive Abort Interrupt Flag bit
    TXABTIF = 2, ///< Transmit Abort Interrupt Flag bit
    TXDIF = 3, ///< Transmit Done Interrupt Flag bit
    DMAIF = 5, ///< DMA Interrupt Flag bit
    PKTIF = 6, ///< RX Packet Pending Interrupt Flag bit
    LINKIF = 11, ///< PHY Link Status Change Interrupt Flag bit
    AESIF = 12, ///< AES Encrypt/Decrypt Interrupt Flag bit
    HASHIF = 13, ///< MD5/SHA-1 Hash Interrupt Flag bit
    MODEXIF = 14 ///< Modular Exponentiation Interrupt Flag bit
} interruptFlagName_t;

#endif	/* INTERRUPT_H */
