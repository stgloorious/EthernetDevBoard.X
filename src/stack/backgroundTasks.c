/**
 * \file backgroundTasks.c
 * \brief 
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

#include "../stack/backgroundTasks.h"
#include "protocols/ipv4Types.h"
#include "../enc424j600/ENC424J600.h"
#include "protocols/ipv4Settings.h"

#include <stdlib.h>

/**
 * \bug (Kind of) Unsolved problem: If \ref PKTIF is checked before \ref LINKIF without
 * calling \ref ethernetController_pollInterruptFlags again LINKIF seems to be set without no reason ??
 * It seems the problem does not occur if checked in this order or if \ref ethernetController_pollInterruptFlags is called
 * multiple times.
 * 
 * Edit: This was likely fixed; Check this.
 */
void handleStackBackgroundTasks(stack_t* stack) {
    unsigned char foo[10];
    error_t errArp;
    error_t errIPv4;
    interruptFlags_t intf;

    intf = ethernetController_pollInterruptFlags();
    if (intf.LINKIF) {//link status change
        //if (enc424j600_getInterruptFlags()&(1 << LINKIF)) {
        ethernetController_updateLinkStatus(&(stack->ethernet)); //save current status
        //Set up the LEDs manually at every link change because of hardware mistake (LEDs are wired up inverted)
        if (stack->ethernet.link == LINK_ESTABLISHED) {//is another ethernet partner present?
            ethernetController_setLEDConfig(LEDA, LED_ON); //LED A indicates the link
            ethernetController_setLEDConfig(LEDB, LED_TRANSMIT_RECEIVE_EVENTS); //LED B is switched off when a packet is on the wire
            ipv4_setIPSourceAddress(stack->source);
        } else {
            ethernetController_setLEDConfig(LEDA, LED_OFF); //LED A indicates the link
            ethernetController_setLEDConfig(LEDB, LED_OFF); //Turn LED B off, indicating there are no packets
            arp_init(); //Delete the table if connection is disconnected
        }
        ethernetController_clearInterruptFlag(LINKIF);
    }
    if (intf.PKTIF) {//indicating a new packet
        //Check anyway if there is something new
        if (ethernetController_newPacketAvailable()) {
            //stack->background.err = ethernet_rxGetNewFrame(&stack->ethernet);
            ethernet_rxGetNewFrame(&stack->newReceivedFrame);
        }
    }
    /**
     * \todo check other relevant interrupt flags and return errors
     * 
     */


    if (stack->background.fPacketPending == 1) {
        //There is a packet awaiting transmission
        errIPv4 = ipv4_sendFrame(stack->pendingPacketToSend);
        if (errIPv4.module == ERROR_MODULE_ARP) {
            switch (errIPv4.code) {
                case ERROR_ARP_WAITING:
                    break;
                case ERROR_CODE_SUCCESSFUL:
#if IPv4_DEBUG_MESSAGES==true
                    UARTTransmitText("[IPv4]: Packet was sent successfully.\n\r");
#endif
                    stack->background.fPacketPending = 0;
                    break;
                case ERROR_ARP_MAXIMUM_NUMBER_OF_REQUESTS_REACHED:
#if IPv4_DEBUG_MESSAGES==true || IPv4_DEBUG_HIGH_PRIORITY==true
                    UARTTransmitText("[IPv4]: Failed to resolve ");
                    UARTTransmitText(ipAdressToString(stack->pendingPacketToSend.ipv4Header.destination));
                    UARTTransmitText("\n\r");
#endif
                    stack->background.fPacketPending = 0;
                    break;
                default:
                    break;
            }
        } else {

        }
    }


    errArp = arp_background(stack->ethernet.link);
    errIPv4 = ipv4_background(stack->ethernet.link);

    if (errIPv4.module == ERROR_MODULE_IPv4 &&
            errIPv4.code == ERROR_IPv4_ADDRESS_ALREADY_IN_USE) {
#if IPv4_DEBUG_MESSAGES==true || IPv4_DEBUG_HIGH_PRIORITY==true
        UARTTransmitText("[IPv4]: Address conflict detected.\n\r");
#endif
        stack->source.address[3]++;
#if IPv4_DEBUG_MESSAGES==true || IPv4_DEBUG_HIGH_PRIORITY==true
        UARTTransmitText("[IPv4]: Setting IPv4 Src Address to ");
        UARTTransmitText(ipAdressToString(stack->source));
        UARTTransmitText("\n\r");
#endif
        ipv4_setIPSourceAddress(stack->source);
    }


}