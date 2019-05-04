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
 * Edit: This was likely fixed; Check this.
 * 
 * \todo Fragmenting!!
 */
void handleStackBackgroundTasks(stack_t* stack) {
    unsigned char foo[10];
    error_t errArp;
    error_t errIPv4;
    interruptFlags_t intf;
    /* =======================  Poll Interrupt Flags  ======================= */
    intf = ethernetController_pollInterruptFlags();
    /* ==== Link Status Change ==== */
    /* Ethernet cable was newly disconnected/connected */
    if (intf.LINKIF) {
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
    /* ==== New Packet available ==== */
    /* there are unprocessed packets in the receive buffer */
    if (intf.PKTIF) {
        //Check anyway if there is something new
        if (ethernetController_newPacketAvailable()) {
            //stack->background.err = ethernet_rxGetNewFrame(&stack->ethernet);
            ethernet_rxGetNewFrame(&stack->newReceivedFrame);
        }
    }
    /* ==== Packet counter overflow ==== */
    /* The number of unprocessed packets in the receive buffer exceeded 255 */
    if (intf.PCFULIF) {
        UART_setFormat(UART_COLOR_BG_RED); //Red color, Primary font
        UARTTransmitText("\a\n\r"); //Alert
        UARTTransmitText(UART_special(UART_LINE_SEPARATOR));
        UARTTransmitText("\n\r*** CRITICAL ERROR: RX BUF OVFL CAUSED SYSTEM RESET ***\n\r");
       UARTTransmitText(UART_special(UART_LINE_SEPARATOR));
        UART_resetFormat();
        UARTTransmitText("\n\r");
        ethernetController_disableEthernet();
        ethernetController_init();
        RESET();
    }
    /**
     * \todo check other relevant interrupt flags and return errors
     * 
     */

    /* =======================  Packet Transmission ======================= */
    if (stack->background.fPacketPending == 1) {
        //There is a packet awaiting transmission

        enum states {
            INIT, PREPARE_FRAGMENTING, SEND_FRAME
        };
        uint8_t static state;
        ipv4_packet_t static currentPacket;
        uint8_t static remainingLength = 0;
        uint8_t static numberOfPacketsSent = 0;
        uint16_t static lastEndAddress = 0;

        switch (state) {
            case INIT:
                if (stack->pendingPacketToSend.ipv4Header.totalLength > ethernetController_getMTU()) {
#if IPv4_DEBUG_MESSAGES==true
                    UARTTransmitText("[IPv4]: Packet will be fragmented.\n\r");
                    UARTTransmitText(intToString(stack->pendingPacketToSend.ethernet.memory.start,10));
                    UARTTransmitText(" -> ");
                    UARTTransmitText(intToString(stack->pendingPacketToSend.ethernet.memory.end,10));
                    UARTTransmitText(" (");
                    UARTTransmitText(intToString(stack->pendingPacketToSend.ethernet.memory.length,10));
                    UARTTransmitText(")\n\r");
#endif
                    state = PREPARE_FRAGMENTING;
                } else {
                    state = SEND_FRAME;
                    currentPacket = stack->pendingPacketToSend;
                }
                break;
            case PREPARE_FRAGMENTING:
                currentPacket = stack->pendingPacketToSend;
                if (numberOfPacketsSent == 0) {
                    //First packet
                    currentPacket.ethernet.memory.length = ethernetController_getMTU();
                    currentPacket.ethernet.memory.end = currentPacket.ethernet.memory.start + currentPacket.ethernet.memory.length;
                    remainingLength = currentPacket.ipv4Header.totalLength - currentPacket.ethernet.memory.length;
                    lastEndAddress = currentPacket.ethernet.memory.end;
                } else {
                    if (remainingLength >= ethernetController_getMTU())
                        currentPacket.ethernet.memory.length = ethernetController_getMTU();
                    else
                        currentPacket.ethernet.memory.length = remainingLength;
                    currentPacket.ethernet.memory.end = currentPacket.ethernet.memory.start + currentPacket.ethernet.memory.length;
                    remainingLength = currentPacket.ipv4Header.totalLength - currentPacket.ethernet.memory.length;
                    lastEndAddress = currentPacket.ethernet.memory.end;
                }
                UARTTransmitText(intToString(currentPacket.ethernet.memory.start,10));
                UARTTransmitText(" -> ");
                UARTTransmitText(intToString(currentPacket.ethernet.memory.end,10));
                UARTTransmitText(" (");
                UARTTransmitText(intToString(currentPacket.ethernet.memory.length,10));
                UARTTransmitText(") ");
                UARTTransmitText("Remaining Length is ");
                UARTTransmitText(intToString(remainingLength,10));
                UARTTransmitText("\n\r");
                state = SEND_FRAME;
                break;
            case SEND_FRAME:
                errIPv4 = ipv4_sendFrame(stack->pendingPacketToSend);
                numberOfPacketsSent++;
                if (errIPv4.module == ERROR_MODULE_ARP) {
                    switch (errIPv4.code) {
                        case ERROR_ARP_WAITING:
                            break;
                        case ERROR_CODE_SUCCESSFUL:
                            stack->background.fPacketPending = 0;
                            state = INIT;
                            break;
                        case ERROR_ARP_MAXIMUM_NUMBER_OF_REQUESTS_REACHED:
#if IPv4_DEBUG_MESSAGES==true || IPv4_DEBUG_HIGH_PRIORITY==true
                            UART_setFormat(UART_COLOR_BG_RED); //Red color, Primary font
                            UARTTransmitText("[IPv4]: Failed to resolve ");
                            UARTTransmitText(ipAdressToString(stack->pendingPacketToSend.ipv4Header.destination));
                            UART_resetFormat();
                            UARTTransmitText("\n\r");

#endif
                            stack->background.fPacketPending = 0;
                            break;
                        default:
                            break;
                    }
                } else {

                }
                break;
        }


    }


    errArp = arp_background(stack->ethernet.link);
    errIPv4 = ipv4_background(stack->ethernet.link);

    if (errIPv4.module == ERROR_MODULE_IPv4 &&
            errIPv4.code == ERROR_IPv4_ADDRESS_ALREADY_IN_USE) {
#if IPv4_DEBUG_MESSAGES==true || IPv4_DEBUG_HIGH_PRIORITY==true
        UART_setFormat(UART_COLOR_BG_RED); //Red color, Primary font
        UARTTransmitText("[IPv4]: Address conflict detected.\n\r");
        UART_resetFormat();
#endif
        stack->source = ipv4_generateAutoIP();
#if IPv4_DEBUG_MESSAGES==true || IPv4_DEBUG_HIGH_PRIORITY==true
        UART_setFormat(UART_COLOR_BG_RED); //Red color, Primary font
        UARTTransmitText("[IPv4]: Setting IPv4 Src Address to ");
        UARTTransmitText(ipAdressToString(stack->source));
        UARTTransmitText("\n\r");
        UART_resetFormat();
#endif
        ipv4_setIPSourceAddress(stack->source);
    }


}