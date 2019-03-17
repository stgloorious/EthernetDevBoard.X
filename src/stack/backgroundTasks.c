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

#include <stdlib.h>

/**
 * \bug (Kind of) Unsolved problem: If \ref PKTIF is checked before \ref LINKIF without
 * calling \ref ethernetController_pollInterruptFlags again LINKIF seems to be set without no reason ??
 * It seems the problem does not occur if checked in this order or if \ref ethernetController_pollInterruptFlags is called
 * multiple times.
 */
void handleStackBackgroundTasks(stack_t* stack) {
    unsigned char foo[10];

    stack->background.interruptFlags = ethernetController_pollInterruptFlags();

    //stack->background.interruptFlags = ethernetController_pollInterruptFlags();
    if (stack->background.interruptFlags.LINKIF) {//link status change
        //if (enc424j600_getInterruptFlags()&(1 << LINKIF)) {
        ethernetController_updateLinkStatus(&(stack->ethernet)); //save current status
        //Set up the LEDs manually at every link change because of hardware mistake (LEDs are wired up inverted)
        if (stack->ethernet.link == LINK_ESTABLISHED) {//is another ethernet partner present?
            ethernetController_setLEDConfig(LEDA, LED_ON); //LED A indicates the link
            ethernetController_setLEDConfig(LEDB, LED_TRANSMIT_RECEIVE_EVENTS); //LED B is switched off when a packet is on the wire
        } else {
            ethernetController_setLEDConfig(LEDA, LED_OFF); //LED A indicates the link
            ethernetController_setLEDConfig(LEDB, LED_OFF); //Turn LED B off, indicating there are no packets
        }
        
        ipv4_setIPSourceAddress(ipv4_getIPSourceAddress());
        
        ethernetController_clearInterruptFlag(LINKIF);
    }

    if (stack->background.interruptFlags.PKTIF) {//indicating a new packet
        //Check anyway if there is something new
        if (ethernetController_newPacketAvailable()) {
            stack->background.err = ethernet_rxGetNewFrame(&stack->ethernet);
        }
    }
    /**
     * \todo check other relevant interrupt flags and return errors
     * 
     */
    if (stack->background.fPacketPending) {
        //There is a packet awaiting transmission

        /*  stack->background.err = ipv4_sendFrame(stack->pendingPacketToSend);
          if (stack->background.err.code == ERROR_ARP_WAITING) {

          }
          if (stack->background.err.module == ERROR_MODULE_ARP &&
                  stack->background.err.code == ERROR_ARP_MAXIMUM_NUMBER_OF_REQUESTS_REACHED) {
              UARTTransmitText("Could not resolve ");
              UARTTransmitText(ipAdressToString(stack->pendingPacketToSend.ipv4Header.destination));
              UARTTransmitText("\n\r");
              stack->background.fPacketPending = false;
          }
          if (stack->background.err.module == ERROR_MODULE_ARP &&
                  stack->background.err.code == ERROR_CODE_SUCCESSFUL) {
              UARTTransmitText("IP packet sent.\n\r");
              //address resolution is completed
              stack->background.fPacketPending = false;
          }*/
    }

    arp_background();
    ipv4_background();
}