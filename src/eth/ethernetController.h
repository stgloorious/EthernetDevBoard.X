/**
 * \file ethernetController.h
 * \brief Interfaces an ethernet controller device
 * \details Ethernet controller functions that are really device-specific (like a soft reset sequence)
 * are found in their own file. This file should only contain more general "Ethernet Controller" functions which
 * would be compatible with different physical Ethernet Controller chips.
 * \author Stefan Gloor
 * \version 1.0
 * \date 31. December 2018, 17:30
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



#ifndef ETHERNETCONTROLLER_H
#define	ETHERNETCONTROLLER_H

#include <stdint.h>
#include "../stack/error.h"
#include "../eth/ethernetTypes.h"
#include "../mem/memoryTypes.h"
#include "../enc424j600/interrupt.h"


/**
 * \brief Status LEDs in the RJ45 Receptacle. Usually they show Link/Unlinked Status and activity.
 * \addtogroup leds LEDs
 * \ingroup ethernetController
 */

/**
 * \brief Used to differentiate between the Status/Link LEDs on the RJ45 receptacle
 * \ingroup leds
 */
typedef enum LEDs {
    LEDA, LEDB
} LEDs_t;

/**
 * \brief States of the LEDs in the RJ45 receptacle
 * \ingroup leds
 */
typedef enum LEDStates {
    LED_ON,
    LED_OFF,
    LED_TRANSMIT_RECEIVE_EVENTS ///< LED flashes for every packet received and transmitted (Status LED)
} LEDStates_t;

/**
 * \defgroup ethernetController Ethernet Controller
 * 
 */

/* =======================  Initialisation  ======================= */
/**
 * \addtogroup init Initialisation
 * \ingroup ethernetController
 * \{
 */
/**
 * \brief Initialises the ethernet controller
 * \details Does a soft reset and checks the device ID. 
 * Then, various things are initialised and the pointers are set up.
 * \note To be called after power up
 * \return Error state of the initialisation process
 */
error_t ethernetController_init();

/** \}*/

/* =======================  Device Functions  ======================= */
/**
 * \addtogroup devicefunc Device Functions
 * \ingroup ethernetController
 * \{
 */
/**
 * \brief Enables ethernet functionality
 */
void ethernetController_enableEthernet();

/**
 * \brief Disables ethernet functionality
 */
void ethernetController_disableEthernet();

/**
 * \brief Enables transmission module
 */
void ethernetController_enableTransmission();

/**
 * \brief Disables transmission module
 */
void ethernetController_disableTransmission();

/**
 * \brief Enables reception module
 */
void ethernetController_enableReception();

/**
 * \brief Disables reception module
 */
void ethernetController_disableReception();

/**
 * \brief Performs a software reset of the Ethernet controller according to the suggested write-and-verify approach recommended in the datasheet.
 * \return Error state of the reset process
 */
error_t ethernetController_softReset();

/**
 * \brief Checks if the device ID equals the expected value
 * \return Error code if device ID is correct
 */
error_t ethernetController_checkDeviceId();

/**
 * \brief Checks the silicion revision of the ethernet controller
 * \return Numerical value of the silicion revision
 */
uint8_t ethernetController_getSiliconRevision();

/**
 * \brief Get device name as a string
 * \return String of device name, e.g. "ENC424J600"
 */
uint8_t *ethernetController_getDeviceName();

/**
 * \brief Intended to be called when an interrupt indicates that a link change as occured
 * \param [out] *state 
 */
void ethernetController_updateLinkStatus(ethernetConnection_t *state);

/**
 * \brief Returns the Maximum Transmission Unit of the Medium (normally 1500 bytes)
 * \return Maximum Transmission Unit: Max. number of bytes per packet
 */
uint16_t ethernetController_getMTU();

/**
 * \brief Checks the Interrupt flags and returns the status in a structure
 * \note In the current configuration, the interrupts are not real interrupts. 
 *  Instead, the flags are polled periodically with this function
 * \return Data structure of all interrupt flags
 */
interruptFlags_t ethernetController_pollInterruptFlags();

/**
 * \brief Clears a single interrupt flag
 * \param flag Bit number of the flag; use \ref interruptFlagsNames
 */
void ethernetController_clearInterruptFlag(uint8_t flag);

/** \}*/

/* =======================  Data Transmission  ======================= */
/**
 * \addtogroup data_transmission Data Transmission
 * \ingroup ethernetController
 * \{
 */
/**
 * \brief Sends the data in the SRAM buffer
 */

void ethernetController_sendPacket(memoryField_t field);


/**
 * \brief Streams data online to the internal memory of the Ethernet Controller. An additonal buffer on the MCU is not needed.
 * \param data  Byte to stream
 * \param field   Field that should be read
 * \warning Note that the field is only refering to what you want to stream just now. This is not necessarily the same as the whole
 * memory field that was reserved in memory. Don't do anything else if you haven't finished the number of bytes given in field.length because
 * otherwise the data transmission is not properly terminated which could lead to weird stuff.
 * \
 */
void ethernetController_streamToTransmitBuffer(uint8_t data, memoryField_t field);

/**
 * \brief Fills in the EtherType field in the SRAM buffer
 * \param ethtype EtherType
 * \param f Memory field where the concerning packet is stored
 */
void ethernetController_writeEtherTypeFieldToBuffer(etherType_t ethtype, memoryField_t f);

/**
 * \brief Fills in the destination MAC address in the SRAM buffer
 * \param addr Destination MAC address
 * \param f Memory field where the concerning packet is stored
 */
void ethernetController_writeDestinationMACAddress(macaddress_t addr, memoryField_t f);

/** \}*/


/* =======================  Data Reception  ======================= */
/**
 * \addtogroup data_reception Data Reception
 * \ingroup ethernetController
 * \{
 */


/**
 * \brief Discards a packet
 * \details This updates the Tail pointer in Ethernet Controller's memory and decrements the packet counter using \ref SETPKTDEC .
 * \note Not only is this function called if a packet should be ignored, it is called after *every* packet
 * that has been completely processed, so the memory occupied by this packet can be freed up.
 * \todo document this better
 */
void ethernetController_dropPacket();

/**
 * \brief Reads the nextPacketPointer from the current packet and stores it.
 * \note To be called right before the packet is going to be processed.
 */
void ethernetController_updateNextPacketPointer();

/**
 * \brief This functions allows online transmission to the internal buffer of the Ethernet Controller. An additional buffer on the MCU is not needed.
 * \return data  Current byte to stream
 * \param startEnd Control number; Pass 0 at the first byte, then 1 and 2 at the last byte
 * \param startAddress   Start address; \ref memoryField.start
 * \warning Once a stream is openend no other function should be called that could access the data communication
 * to the Ethernet controller. For maximum speed effiency, the read pointer is only set once (at the beginning of the stream).
 * After that, the read operation relies on the automatic pointer increment of the Ethernet Controller. So before doing anything else
 * this function has to be called with \p startEnd equal to 2 to properly end the stream.
 * \note If \p startEnd != 1 the returned value is always 0.
 * 
 */
uint8_t ethernetController_streamFromRXBuffer(uint8_t startEnd, uint16_t startAddress);

/**
 * \brief Parses the recently received frame and returns the Destination MAC address.
 * \param field \ref memoryField_t structure indicating where the packet that should be parsed lies in memory
 * \return Destination MAC address
 */
macaddress_t ethernetController_getDestinationMACAddress(memoryField_t field);

/**
 * \brief Parses the recently received frame and returns the Source MAC address.
 * \param field \ref memoryField_t structure indicating where the packet that should be parsed lies in memory
 * \return Source MAC address
 */
macaddress_t ethernetController_getSourceMACAddress(memoryField_t field);

/**
 * \brief Parses the recently received frame and returns the EtherType field.
 * \param field \ref memoryField_t structure indicating where the packet that should be parsed lies in memory
 * \return EtherType field
 */
etherType_t ethernetController_getEtherTypeField(memoryField_t field);

/**
 * \brief Reads the Receive Status Vector from the recently received Packet and returns the address of the first element
 * \return Pointer to RSV[0]
 */
RSV_t ethernetController_getRSV(uint16_t address);

/**
 * \brief Checks if there is a new frame ready for processing
 * \return boolean value
 */
uint8_t ethernetController_newPacketAvailable();

/**
 * \brief Get the total number of unprocessed packets
 * \return Number of packets
 */
uint8_t ethernetController_getCurrentPacketCount();

/**
 * \brief Get the address where the next packet starts
 * \return Address
 */
uint16_t ethernetController_getNextPacketPointer();

/**
 * \brief Gets unique MAC address that is currently used as source address. After POR it is set to a hardwired unique address.
 * \return MAC source address
 */
macaddress_t ethernetController_getMacAddress();

/**
 * \brief Sets a MAC source address. Has to be set after every POR, because it gets overwritten with a hardwired unique address after power-up.
 * \param mac MAC source address
 */
void ethernetController_setMacAddress(macaddress_t mac);

/**\}*/

/* =======================  LEDs  ======================= */
/**
 * \addtogroup leds LEDs
 * \ingroup ethernetController
 * \{
 */

/**
 * \brief Sets the LEDs in the RJ45 receptacle to a given configuration (ON, OFF or an 'automatic' mode).
 * \param LED Which LED; use \ref LEDs_t
 * \param conf Status of the LED; use \ref LEDStates_t
 */
void ethernetController_setLEDConfig(LEDs_t LED, LEDStates_t conf);

/**\}*/

#endif	/* ETHERNETCONTROLLER_H */