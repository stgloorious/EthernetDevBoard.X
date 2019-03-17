/**
 * \file ENC424J600.h
 * \brief Drives Microchip's ENC424J600 10/100Base-TX Ethernet Controller
 * \author Stefan Gloor
 * \version 1.0
 * \date 31. December 2018
 * \todo Crypto engine support with DMA
 * \ingroup ethernetController
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

#ifndef ENC424J600_H
#define	ENC424J600_H

#include <xc.h>
#include <stdint.h>
#include "../eth/ethernetController.h"
#include "../stack/error.h"
#include "../system/system.h"
#include "../mem/memoryController.h"
#include "../enc424j600/rsv.h"

/**
 * \addtogroup enc424j600_module ENC424J600
 * \ingroup ethernetController
 * \details These functions directly interface the Ethernet controller and are device-specific.
 * Currently the only interface supported is SPI. The parallel port is not supported. 
 * 
 * All functions are static because they should only be accessible for ethernetController_* functions.
 * 
 * \note Direct multi-byte read/write operations as suggested by the datasheet are not (yet) implemented as separate functions.
 *  Instead, this functionality is achieved by calling \ref enc424j600_writeSPI() multiple times, 
 *  as seen in \ref enc424j600_setGPDATAWritePointer, for example.
 * 
 * \note Banked operations are not (yet) supported. Everything is accessed through unbanked addresses and commands.
 * 
 *
 */

/**
 * \ingroup enc424j600_module
 * \details This string is used so upper laying functions can display 
 * the name of the used Ethernet Controller chip
 */
#define DEVICE_NAME                 "ENC424J600"

/* =======================  Initialisation  ======================= */
/**
 * \addtogroup init Initialisation
 * \ingroup enc424j600_module
 * \{
 */

/**
 * \brief Initialises the hardware SPI module. To be called after power-up.
 */
void static enc424j600_initSPI();

/** \} */

/* =======================  READ/WRITE REGISTERS  ======================= */
/**
 * \addtogroup readwrite Read/Write Registers
 * \ingroup registers
 * \{
 */
/**
 * \brief Writes a single byte to the SPI Interface; CS Pin is not driven by this function
 * \param [in] data Pointer to the byte you want to send
 */
void static enc424j600_writeSPI(uint8_t *data);

/**
 * \brief Reads a single byte from the SPI interface; CS Pin is not driven by this function
 * \param [out] data Pointer to the byte where the read data is stored
 */
void static enc424j600_readSPI(uint8_t *data);

/**
 * \brief Sends a single byte opcode via the SPI interface
 * \param opcode Command which is going to be sent
 */
void static enc424j600_writeSingleByte(uint8_t opcode);

/**
 * \brief Reads a one-byte command via the SPI interface
 * \param opcode Command
 */
void static enc424j600_readSingleByte(uint8_t opcode);

/**
 * \brief Writes an unbanked Special Function Register from the ENC424J600. Bank offsets have to be added to the register addresses.
 * \param addr Register address
 * \param [in] data  Register data
 */
void static enc424j600_writeControlRegisterUnbanked(uint8_t addr, uint8_t *data);

/**
 * \brief Reads an unbanked Special Function Register from the ENC424J600. Bank offsets have to be added to the register addresses.
 * \param addr  Register address
 * \param [out] data  Register data
 */
void static enc424j600_readControlRegisterUnbanked(uint8_t addr, uint8_t *data);

/**\}*/

/* =======================  PHY REGISTERS  ======================= */
/**
 * \addtogroup phy PHY Registers
 * \ingroup registers
 * \{
 */

/**
 * \brief Reads a PHY Register
 * \param addr  Address of the PHY register
 * \param [out] data  Data that is read
 */
void static enc424j600_readPHYRegister(uint8_t addr, uint16_t *data);

/** \} */

/* =======================  TRANSMISSION  ======================= */
/**
 * \addtogroup transmission Transmission
 * \ingroup enc424j600_module
 * \{
 */
/**
 * \brief Sets the TX Frame Length; This has to be called prior to transmitting a frame.
 * \param len   Byte length of the TX Frame
 */
void static enc424j600_setTXLength(uint16_t len);

/**\}*/

/* =======================  RECEPTION  ======================= */
/**
 * \addtogroup reception Reception
 * \ingroup enc424j600_module
 * \{
 */

/**
 * \brief NextPacketPointer holds the address value of the received packet.
 * \param ptr Value which NextPacketPointer should be set to
 */
void static enc424j600_setNextPacketPointer(uint16_t ptr);

/**
 * \brief NextPacketPointer holds the address value of the received packet.
 * \return Value of NextPacketPointer
 */
uint16_t static enc424j600_getNextPacketPointer();


/**
 * \brief Reads out internal register PKCNT;
 *  \details Needs to be polled to check for new packets.
 * If it is greater zero, there are unprocessed packets. To decrement the packet count
 * (after processing) \ref SETPKTDEC needs to be set (via single byte command).
 * \return Value of PKCNT
 */
uint8_t enc424j600_getPacketCount();

/**\}*/

/* =======================  POINTER OPERATIONS  ======================= */
/**
 * \addtogroup pointers Pointer operations
 * \ingroup enc424j600_module
 * \note Since the SPI mode is the only supported interface data can not be written/read to/from the General Purpose Buffer directly.
 * Instead memory access is achieved through a data window. A pointer register indicates where the data gets written to or read from, respectively.
 * Data can then be exchanged through a dedicated register (data window).
 * \{
 */

/**
 * \brief Sets the TX Start Address Pointer to a desired value.
 * \details Data starting from this pointer will be sent.
 * \param addr Pointer value
 */
void static enc424j600_setTXStartAddress(uint16_t addr);

/**
 * \brief Sets the General Purpose Data Write Pointer to a desired position.
 * \details This pointer points to the location in memory which should be written to.
 * It is used to write data to the transmit buffer
 * \param addr  Pointer value
 */
void static enc424j600_setGPDATAWritePointer(uint16_t addr);

/**
 * \brief Sets the General Purpose Data Read Pointer to a desired position.
 * \details This pointer points to the location in memory which should be read from.
 * \param addr  Pointer value
 */
void static enc424j600_setGPDATAReadPointer(uint16_t addr);

/**
 * \brief Sets the Ethernet RX Data Window Register Read Pointer to a desired position.
 * \details This pointer points to the location in the reception buffer memory which should be read from.
 * It is used to read the received data
 * \param addr
 */
void static enc424j600_setERXDATAReadPointer(uint16_t addr);

/**
 * \brief Sets the receive buffer start address
 * \param addr Pointer value
 */
void static enc424j600_setRXBufferStartAddress(uint16_t addr);

/**
 * \brief Sets the RX tail pointer ERXTAIL
 * \details This pointer indicates where unprocessed data ends in the buffer. This way
 * data which lies between head and tail pointer is "protected" from being overwritten.
 * \see Figure 9-3, Page 88 of the datasheet.
 * \param addr Pointer value
 */
void static enc424j600_setRXTailPointer(uint16_t addr);

/**\}*/

/* =======================  SETTINGS  ======================= */
/**
 * \addtogroup settings Settings
 * \ingroup enc424j600_module
 * \{
 */

/**
 * \brief Enables the Ethernet module
 * \details Works by setting the ETHEN<15> bit in the ECON2 register (\ref ECON2H)
 */
void static enc424j600_enable();
/**
 * \brief Disables the Ethernet module
 * \details Works by clearing the ETHEN<15> bit in the ECON2 register (\ref ECON2H)
 * If the controller is disabled, it uses less power.
 */
void static enc424j600_enable();
/**
 * \brief Enables the ethernet connection to receive packets
 * \details It uses the dedicated single byte command \ref ENABLERX
 */
void static enc424j600_enableReception();

/**
 * \brief Disables the ethernet connection to receive packets
 * \details It uses the dedicated single byte command \ref DISABLERX
 */
void static enc424j600_disableReception();

/**
 * \brief Enables the auto-hardware-insertion of the MAC source address. The MAC address is unique and hardwired into the ENC424J600
 */
void static enc424j600_enableAutoMACInsertion();

/**
 * \brief Disables the auto-hardware-insertion of the MAC source address. The MAC address is unique and hardwired into the ENC424J600
 */
void static enc424j600_disableAutoMACInsertion();

/**\}*/

/* =======================  INTERRUPTS  ======================= */
/**
 * \addtogroup interrupts Interrupts
 * \ingroup enc424j600_module
 * \{
 */
/**
 * \brief Reads out the Interrupt Flag Register EIR.
 * \details Note that these flags are always valid, no matter if the interrupt is enabled or not.
 * This way the interrupt flags can be polled periodically for low priority stuff.
 * Note that the MSB is not an interrupt flag (it is returned, too).
 * \return The value of the Interrupt FLag Register EIR
 */
uint16_t enc424j600_getInterruptFlags();

/**
 * \brief Clears a single interrupt flag in the Interrupt Flag Register EIR.
 * \details Note that the MSB is not an interrupt flag.
 * \note Use \ref interruptFlagsNames with this.
 * \param flag Bit number of the flag to clear (0..15)
 */
void static enc424j600_clearInterruptFlag(uint8_t flag);

/**\}*/

/* =======================  RECEIVE STATUS VECTOR  ======================= */
/**
 * \addtogroup rsv Receive Status Vector
 * \ingroup reception
 * \details Receive Status Vector (RSV) 
 * The RSV includes the length of the frame, and flags indicating the type of packet and which filters were matched. It is put in 
 * \{
 */
/**
 * \brief This function takes in the Receive Status Vector and puts it in a nice structure
 * @param rsv [in] Pointer to the first element of a 6 Byte array which contains the RSV.
 * @return A RSV structure
 */
RSV_t static enc424j600_updateReceiveStatusVector(uint8_t *rsv);

/**\}*/

/**
 * \addtogroup HARDWARE_ACTIONS Hardware actions
 * \ingroup enc424j600_module
 * \details Used by low-level functions which access the SPI module directly.
 * The CS Pin has to be driven separately, the SPI moudle doesn't do that.
 * \{
 */
#define CS_PIN_LOW		PORTBbits.RB4=0
#define CS_PIN_HIGH		PORTBbits.RB4=1
/**\}*/


/**
 * \see EIDLED:DEVID<2:0> (Register 8-2, Page 81 of ENC424J600's datasheet)
 * Used by \ref ethernetController_checkDeviceId()
 */
#define EXPECTED_DEVICE_ID      0b001


/**
 * \addtogroup registers Registers
 * \ingroup enc424j600_module
 *
 */

/**
 * \addtogroup SFRs Special Function Registers Definitions
 * \ingroup registers
 */

/**
 * \addtogroup BANK_0 Bank 0 Instructions
 * \ingroup SFRs
 * \todo It may be nice to add the full register names
 * \{
 */
#define BANK_0_OFFSET	0x00
#define ETXSTL			0x00
#define ETXSTH			0x01
#define ETXLENL			0x02
#define ETXLENH			0x03
#define ERXSTL			0x04
#define ERXSTH			0x05
#define ERXTAILL		0x06
#define ERXTAILH		0x07
#define ERXHEADL		0x08
#define ERXHEADH		0x09
#define EDMASTL			0x0A
#define EDMASTH			0x0B
#define EDMALENL		0x0C
#define EDMALENH		0x0D
#define EDMADSTL		0x0E
#define EDMADSTH		0x0F
#define EDMACSL			0x10
#define EDMACSH			0x11
#define ETXSTATL		0x12
#define ETXSTATH		0x13
#define ETXWIREL		0x14
#define ETXWIREH		0x15
#define EUDASTL			0x16
#define EUDASTH			0x17
#define EUDANDL			0x18
#define EUDANDH			0x19
#define ESTATL			0x1A
#define ESTATH			0x1B
#define EIRL			0x1C
#define EIRH			0x1D
#define ECON1L			0x1E
#define ECON1H			0x1F
/**\}*/

/**
 * \addtogroup BANK_1 Bank 1 Instructions
 * \ingroup SFRs
 * \{
 */
#define BANK_1_OFFSET	0x20
#define EHT1L			0x00
#define EHT1H			0x01
#define EHT2L			0x02
#define EHT2H			0x03
#define EHT3L			0x04
#define EHT3H			0x05
#define EHT4L			0x06
#define EHT4H			0x07
#define EPMM1L			0x08
#define EPMM1H			0x09
#define EPMM2L			0x0A
#define EPMM2H			0x0B
#define EPMM3L			0x0C
#define EPMM3H			0x0D
#define EPMM4L			0x0E
#define EPMM4H			0x0F
#define EPMCSL			0x10
#define EPMCSH			0x11
#define EPMOL			0x12
#define EPMOH			0x13
#define ERXFCONL		0x14
#define ERXFCONH		0x15
#define EUDASTL			0x16
#define EUDASTH			0x17
#define EUDANDL			0x18
#define EUDANDH			0x19
#define ESTATL			0x1A
#define ESTATH			0x1B
#define EIRL			0x1C
#define EIRH			0x1D
#define ECON1L			0x1E
#define ECON1H			0x1F
/** \} */

/**
 * \addtogroup BANK_2 Bank 2 Instructions
 * \ingroup SFRs
 * \{
 */
#define BANK_2_OFFSET	0x40
#define MACON1L			0x00
#define MACON1H			0x01
#define MACON2L			0x02
#define MACON2H			0x03
#define MABBIPGL		0x04
#define MABBIPGH		0x05
#define MAIPGL			0x06
#define MAIPGH			0x07
#define MACLCONL		0x08
#define MACLCONH		0x09
#define MAMXFLL			0x0A
#define MAMXFLH			0x0B
//#define RESERVED		0x0C
//#define RESERVED		0x0D
//#define RESERVED		0x0E
//#define RESERVED		0x0F
//#define RESERVED		0x10
//#define RESERVED		0x11
#define MICMDL			0x12
#define MICMDH			0x13
#define MIREGADRL		0x14
#define MIREGADRH		0x15
#define EUDASTL			0x16
#define EUDASTH			0x17
#define EUDANDL			0x18
#define EUDANDH			0x19
#define ESTATL			0x1A
#define ESTATH			0x1B
#define EIRL			0x1C
#define EIRH			0x1D
#define ECON1L			0x1E
#define ECON1H			0x1F
/** \}*/

/**
 * \addtogroup BANK_3 Bank 3 Instructions
 * \ingroup SFRs
 * \{
 */
#define BANK_3_OFFSET	0x60
#define MAADR3L			0x00
#define MAADR3H			0x01
#define MAADR2L			0x02
#define MAADR2H			0x03
#define MAADR1L			0x04
#define MAADR1H			0x05
#define MIWRL			0x06
#define MIWRH			0x07
#define MIRDL			0x08
#define MIRDH			0x09
#define MISTATL			0x0A
#define MISTATH			0x0B
#define EPAUSL			0x0C
#define EPAUSH			0x0D
#define ECON2L			0x0E
#define ECON2H			0x0F
#define ERXWML			0x10
#define ERXWMH			0x11
#define EIEL			0x12
#define EIEH			0x13
#define EIDLEDL			0x14
#define EIDLEDH			0x15
#define EUDASTL			0x16
#define EUDASTH			0x17
#define EUDANDL			0x18
#define EUDANDH			0x19
#define ESTATL			0x1A
#define ESTATH			0x1B
#define EIRL			0x1C
#define EIRH			0x1D
#define ECON1L			0x1E
#define ECON1H			0x1F
/** \}*/

/**
 * \addtogroup BANK_UN Unbanked Instructions
 * \ingroup SFRs
 * \{
 */
#define UNBANKED_OFFSET	0x80
#define EGPDATA			0x00
//#define RESERVED		0x01
#define ERXDATA			0x02
//#define RESERVED		0x03
#define EUDADATA		0x04
//#define RESERVED		0x05
#define EGPRDPTL		0x06
#define EGPRDPTH		0x07
#define EGPWRPTL		0x08
#define EGPWRPTH		0x09
#define ERXRDPTL		0x0A
#define ERXRDPTH		0x0B
#define ERXWRPTL		0x0C
#define ERXWRPTH		0x0D
#define EUDARDPTL		0x0E
#define EUDARDPTH		0x0F
#define EUDAWRPTL		0x10
#define EUDAWRPTH		0x11
//#define RESERVED		0x12
//#define RESERVED		0x13
//#define RESERVED		0x14
//#define RESERVED		0x15
//#define RESERVED		0x16
//#define RESERVED		0x17
//#define RESERVED		0x18
//#define RESERVED		0x19
//#define RESERVED		0x1A
//#define RESERVED		0x1B
//#define RESERVED		0x1C
//#define RESERVED		0x1D
//#define -				0x1E
//#define -				0x1F
/** \}*/


/**
 * \addtogroup single_byte Single Byte Instructions
 * \ingroup enc424j600_module
 * \brief Use with \ref enc424j600_writeSingleByte ()
 * \see Table 4-2, Page 44 in ENC424J600's datasheet.
 * \{
 */
#define B0SEL			0b11000000 ///< Selects SFR Bank 0
#define B1SEL			0b11000010 ///< Selects SFR Bank 1
#define B2SEL			0b11000100 ///< Selects SFR Bank 2
#define B3SEL			0b11000110 ///< Selects SFR Bank 3 
#define SETETHRST		0b11001010 ///< Issues System Reset by setting ETHRST (ECON2<4>) 
#define FCDISABLE		0b11100000 ///< Disables flow control (sets ECON1<7:6> = 00)
#define FCSINGLE		0b11100100 ///< Transmits a single pause frame (sets ECON1<7:6> = 01) 
#define FCMULTIPLE		0b11100100 ///< Enables flow control with periodic pause frames (sets ECON1<7:6> = 10) 
#define FCCLEAR			0b11100110 ///< Terminates flow control with a final pause frame (sets ECON1<7:6> = 11) 
#define SETPKTDEC		0b11001100 ///< Decrements PKTCNT by setting PKTDEC (ECON1<8>) 
#define DMASTOP			0b11010010 ///< Stops current DMA operation by clearing DMAST (ECON1<5>) 
#define DMACKSUM		0b11011000 ///< Starts DMA and checksum operation (sets ECON1<5:2> = 1000) 
#define DMACKSUMS		0b11011010 ///< Starts DMA checksum operation with seed (sets ECON1<5:2> = 1010) 
#define DMACOPY			0b11011100 ///< Starts DMA copy and checksum operation (sets ECON1<5:2> = 1100) 
#define DMACOPYS		0b11011110 ///< Starts DMA copy and checksum operation with seed (sets ECON1<5:2> = 1110) 
#define SETTXRTS		0b11010100 ///< Sets TXRTS (ECON1<1>), sends an Ethernet packet 
#define ENABLERX		0b11101000 ///< Enables packet reception by setting RXEN (ECON1<0>) 
#define DISABLERX		0b11101010 ///< Disables packet reception by clearing RXEN (ECON1<0>) 
#define SETEIE			0b11101100 ///< Enable Ethernet Interrupts by setting INT (ESTAT<15>) 
#define CLREIE			0b11101110 ///< Disable Ethernet Interrupts by clearing INT (ESTAT<15>) 
/** \} */


#endif /* ENC424J600_H */