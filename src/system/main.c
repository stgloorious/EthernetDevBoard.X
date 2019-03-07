/**
 * \file main.c
 * \brief main
 * \author Stefan Gloor
 * \version 1.0
 * \date 28. December 2018
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

#include <stdlib.h>
#include "../system/main.h"
#include "../enc424j600/ENC424J600.h"

uint32_t volatile numberToDisplay = 0; //global because interrupt
uint8_t volatile buttonState = 0;

stack_t static stack;

void main() {

    /* OSCILLATOR */
    OSCCONbits.SCS = 0b00; //primary oscillator
    OSCCONbits.IRCF = 0b1110; //8 MHz oscillator
    OSCCON2bits.PLLEN = 0b1; //enable 4x PLL

    /* Interrupt config */
    INTCONbits.GIE = 0;
    INTCONbits.TMR0IE = 1;
    INTCON3bits.INT2IE = 1;
    RCIP = 0; // Set UART Receive IRQ Priority Low
    RCIE = 1; // Enable UART Reveive IRQ
    INTCONbits.PEIE = 1; //enable peripheral interrupts


    /* TIMER 0 */
    T0CONbits.TMR0ON = 1;
    T0CONbits.T08BIT = 1; //8 bit
    T0CONbits.T0CS = 0; //Source is internal clock
    T0CONbits.PSA = 0; // prescaler is assigned
    T0CONbits.T0PS = 0b101;


    /* TIMER 1 */
    T1CONbits.TMR1ON = 1;
    CCP1CONbits.CCP1M = 0b1011;
    CCPR1 = 8000; //32 MHz Fosc; (Fosc/4)/1 kHz, so an interrupt every 1 ms
    PIE1bits.CCP1IE = 1;
    INTCONbits.PEIE = 1; //enable peripheral interrupts

    TRISBbits.RB6 = 0;


    UARTInit();
    sevenSegmentInit();
    __delay_ms(10);
    UARTTransmitText("\x12"); //form feed
    if (!RCONbits.TO) {
        UARTTransmitText("------------------------------------------------\n\r");
        UARTTransmitText("\a*** CRITICAL ERROR: WATCHDOG CAUSED RESET ***\n\r");
        UARTTransmitText("------------------------------------------------\n\r");
    }
    UARTTransmitText("FIRMWARE BUILD DATE IS ");
    UARTTransmitText(__DATE__);
    UARTTransmitText(" ");
    UARTTransmitText(__TIME__);
    UARTTransmitText("\n\r");
    UARTTransmitText("Initialising Ethernet Controller...\n\r");
    error_t err;
    err = ethernetController_init();
    if (err.code != ERROR_CODE_SUCCESSFUL)
        printErrorMessage(err);
    err = ethernetController_checkDeviceId();
    if (err.code == ERROR_CODE_SUCCESSFUL) {
        UARTTransmitText(ethernetController_getDeviceName());
        UARTTransmitText(" detected. ");
    }
    UARTTransmitText("Silicon revision is ");
    UARTTransmitInt(ethernetController_getSiliconRevision());
    UARTTransmitText(".\n\r");
    UARTTransmitText("Source MAC address is ");
    UARTTransmitText(macToString(ethernetController_getMacAddress()));
    UARTTransmitText(".\n\r");
    UARTTransmitText("Initialisation completed.\n\r");
    UARTTransmitText("------------------------------------------------\n\r");

    ARP_initTable();

    ethernetController_setLEDConfig(LEDA, LED_ON);
    ethernetController_setLEDConfig(LEDA, LED_ON);
    __delay_ms(500);

    ethernetController_setLEDConfig(LEDA, LED_OFF);
    ethernetController_setLEDConfig(LEDA, LED_OFF);


    uint32_t prescaler = 0;

    BUTTON_ANSEL;
    BUTTON_TRISB;

    uint8_t oldState;

    ipv4_address_t IPsource;
    IPsource.address[0] = 192;
    IPsource.address[1] = 168;
    IPsource.address[2] = 0;
    IPsource.address[3] = 4;

    ipv4_address_t IPdestination;
    IPdestination.address[0] = 192;
    IPdestination.address[1] = 168;
    IPdestination.address[2] = 0;
    IPdestination.address[3] = 5;

    // ipv4_setIPDestinationAddress(IPdestination);
    ipv4_setIPSourceAddress(IPsource);


    //Now everything's set up, allow interrupts
    INTCONbits.GIE = 1; //global interrupt enable
    INTCONbits.PEIE = 1;

    srand(ethernetController_getMacAddress().address[5]);


    while (1) {
        CLRWDT(); //clear watch doggy

        handleStackBackgroundTasks(&stack);

        printEthernetState(stack.ethernet);
        numberToDisplay = (stack.ethernet.link == NO_LINK) ? 1000 : ethernetController_getCurrentPacketCount();

        if (buttonState) {
            buttonState = 0;

            if (stack.ethernet.link == LINK_ESTABLISHED) {

                //////////////////////////////////////////////
                uint8_t headerBuf[32];
                stack.pendingPacketToSend.ipv4Header.destination = IPdestination;
                stack.pendingPacketToSend.ipv4Header.source = IPsource;
                stack.pendingPacketToSend.ipv4Header.totalLength = 120;
                stack.pendingPacketToSend.ipv4Header.timeToLive = 255;
                stack.pendingPacketToSend.ipv4Header.version = 4;
                ipv4_calculateHeaderChecksum(&stack.pendingPacketToSend.ipv4Header);
                ipv4_writeHeaderIntoBuffer(stack.pendingPacketToSend.ipv4Header, headerBuf);

                ipv4_txFrameRequest(&stack.pendingPacketToSend);

                for (uint8_t i = 0; i < stack.pendingPacketToSend.ipv4Header.totalLength; i++) {
                    if (i < stack.pendingPacketToSend.ipv4Header.headerLength * 4) {
                        ipv4_streamToTransmissionBuffer(headerBuf[i], stack.pendingPacketToSend);
                    } else
                        ipv4_streamToTransmissionBuffer(0, stack.pendingPacketToSend);
                }
                //////////////////////////////////////////////      
                stack.background.fPacketPending = 1;

                UARTTransmitText(hexToString(stack.pendingPacketToSend.memory.start));
                UARTTransmitText(", ");
                UARTTransmitText(hexToString(stack.pendingPacketToSend.memory.end));
                UARTTransmitText("\n\r");
            }
        }
    }
}

void printEthernetState(ethernetConnection_t state) {
    static ethernetConnection_t oldState;
    if (state.link != oldState.link) {
        if (state.link == NO_LINK) {
            UARTTransmitText("Link lost.\n\r");
        } else if (state.link == LINK_ESTABLISHED) {
            UARTTransmitText("Link established. ");
        }

        if (state.link == LINK_ESTABLISHED) {//Display other info only if there's a link
            if (state.speed == TEN_MBIT) {
                UARTTransmitText("Speed is 10 Mbps, ");
            } else if (state.speed == HUNDRED_MBIT) {
                UARTTransmitText("Speed is 100 Mbps, ");
            }
            if (state.duplex == HALF_DUPLEX) {
                UARTTransmitText("connection is Half Duplex.\n\r");
            } else if (state.duplex == FULL_DUPLEX) {

                UARTTransmitText("connection is Full Duplex.\n\r");
            }
        }
    }
    oldState = state;
}

void printErrorMessage(error_t err) {
    UARTTransmitText("[ERROR]");
    switch (err.module) {
        case ERROR_MODULE_MEMORY:
            UARTTransmitText(" MEMORY MODULE: ");
            break;
        case ERROR_MODULE_ETHERNET_CONTROLLER:
            UARTTransmitText(" ETHERNET CONTROLLER MODULE: ");
            break;
        case ERROR_MODULE_ETHERNET:
            UARTTransmitText(" ETHERNET MODULE: ");
            break;
        case ERROR_MODULE_ARP:
            UARTTransmitText(" ARP MODULE: ");
            break;
        case ERROR_MODULE_IPv4:
            UARTTransmitText(" IPv4 MODULE: ");
        default:
            break;
    }
    switch (err.code) {
        case ERROR_ETHERNET_CONTROLLER_UNRESPONSIVE:
            UARTTransmitText("Ethernet controller is unresponsive.");
            break;
        case ERROR_ETHERNET_CONTROLLER_UNKNOWN_DEVICE_ID:
            UARTTransmitText("Ethernet controller has returned an unknown device ID.");
            break;
        case ERROR_ARP_MAXIMUM_NUMBER_OF_REQUESTS_REACHED:
            UARTTransmitText("IP address could not be resolved.");
        default:
        case ERROR_ETHERNET_CONTROLLER_UNKNOWN:
            UARTTransmitText("An unknown error has occured.");
            break;
    }

    UARTTransmitText(" (Code: ");
    UARTTransmitText(intToString(err.code));
    UARTTransmitText(").\n\r");
}

void buttonHandler(uint8_t volatile *state) {
    uint8_t static oldState = 0;
    uint8_t static newState = 0;
    uint32_t static debounceCounter = 0;
    const uint32_t debounceValue = 0xfff;
    if (BUTTON_STATE) {
        if (debounceCounter < debounceValue)
            debounceCounter++;
    } else {
        debounceCounter = 0;
    }
    newState = (unsigned) ((debounceCounter >= debounceValue) ? 1 : 0);
    if (newState && !oldState) {
        debounceCounter = debounceValue;
        *state = 1;
    } else {
        *state = 0;
    }
    oldState = newState;
}

void interrupt ISR(void) {

    if (PIR1bits.CCP1IF) {
        PIR1bits.CCP1IF = 0;
        PORTBbits.RB6 = ~PORTBbits.RB6;
        updateTime();
    }

    if (INTCONbits.TMR0IF) {//Timer 0 Overflow interrupt
        INTCONbits.TMR0IF = 0;
        sevenSegmentUpdate(numberToDisplay);
        buttonHandler(&buttonState);


    }

    if (INTCON3bits.INT2IF) {
        INTCON3bits.INT2IF = 0;
        /* IMPORTANT: 
         * This interrupt is not cleared (so it won't re-trigger) until
         * every bit in the EIR register of the ENC424J600 is cleared.
         * Note that the RX Packet Pending Flag cannot be cleared by software directly, 
         * it must be cleared by setting the packet count to zero! 
         */
    }
    if (RCIF) { //UART data received
        // interrupt flag cleared when buffer is read
        //UARTReceptionHandler();
    }
}

