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
#include <math.h>
#include "../system/main.h"
#include "../enc424j600/ENC424J600.h"
#include "../stack/protocols/icmp.h"
#include "../stack/protocols/udp.h"

uint32_t volatile numberToDisplay = 0; //global because interrupt
uint8_t volatile buttonState = 0;
bool_t unlocked = false;
uint8_t foo = 0;


stack_t stack;

void main() {

    /* OSCILLATOR */
    /*  OSCCONbits.SCS = 0b00; //primary oscillator
      OSCCONbits.IRCF = 0b1110; //8 MHz oscillator
      OSCCON2bits.PLLEN = 0b1; //enable 4x PLL
     * */
    OSCFRQbits.FRQ = 0b0110; //32 MHz
    OSCENbits.HFOEN = 1; //Explicitly enable HFINTOSC

    /* Interrupt config */
    INTCON0bits.GIE = 0;
    PIE3bits.TMR0IE = 1;
    // INTCON3bits.INT2IE = 1;
    PIE3bits.U1RXIE = 1; // Enable UART Reveive IRQ
    IPR3bits.U1RXIP = 0; //Low priority
    //INTCONbits.PEIE = 1; //enable peripheral interrupts


    /* TIMER 0 */
    T0CON0bits.EN = 1;
    T0CON0bits.MD16 = 0; //8 bit
    T0CON1bits.CS = 0b011; //Source is internal clock HFINTOSC
    T0CON1bits.CKPS = 0b0000; // prescaler is assigned
    T0CON0bits.OUTPS = 0b101;


    /* TIMER 1 */
    T1CONbits.TMR1ON = 1;
    CCP1CONbits.CCP1MODE = 0b1011;
    CCPR1 = 8000; //32 MHz Fosc; (Fosc/4)/1 kHz, so an interrupt every 1 ms
    PIE4bits.CCP1IE = 1;
    //INTCONbits.PEIE = 1; //enable peripheral interrupts

    TRISBbits.TRISB6 = 0;


    UARTInit();
    sevenSegmentInit();
    __delay_ms(10);
    UARTTransmitText("\x12"); //form feed
    if (!PCON0bits.RWDT) {
        UART_setFormat(UART_COLOR_BG_RED); //Red color, Primary font
        UARTTransmitText("\a\n\r"); //Alert
        UARTTransmitText(UART_special(UART_LINE_SEPARATOR));
        UARTTransmitText("\n\r*** CRITICAL ERROR: WATCHDOG CAUSED RESET ***\n\r");
        UARTTransmitText(UART_special(UART_LINE_SEPARATOR));
        UART_resetFormat();
    }
    if (!PCON0bits.BOR) {
        UART_setFormat(UART_COLOR_BG_RED); //Red color, Primary font
        UARTTransmitText("\a\n\r"); //Alert
        UARTTransmitText(UART_special(UART_LINE_SEPARATOR));
        UARTTransmitText("\n\r*** CRITICAL ERROR: BROWN-OUT CAUSED RESET ***\n\r");
        UARTTransmitText(UART_special(UART_LINE_SEPARATOR));
        UART_resetFormat();
    }
    if (!PCON0bits.STKOVF) {
        UART_setFormat(UART_COLOR_BG_RED); //Red color, Primary font
        UARTTransmitText("\a\n\r"); //Alert
        UARTTransmitText(UART_special(UART_LINE_SEPARATOR));
        UARTTransmitText("\n\r*** CRITICAL ERROR: STACK OVRFL CAUSED RESET ***\n\r");
        UARTTransmitText(UART_special(UART_LINE_SEPARATOR));
        UART_resetFormat();
    }
    if (!PCON0bits.STKUNF) {
        UART_setFormat(UART_COLOR_BG_RED); //Red color, Primary font
        UARTTransmitText("\a\n\r"); //Alert
        UARTTransmitText(UART_special(UART_LINE_SEPARATOR));
        UARTTransmitText("\n\r*** CRITICAL ERROR: STACK UNDRFL CAUSED RESET ***\n\r");
        UARTTransmitText(UART_special(UART_LINE_SEPARATOR));
        UART_resetFormat();
    }
    UARTTransmitText("\n\rFIRMWARE BUILD DATE IS ");
    UARTTransmitText(__DATE__);
    UARTTransmitText(" ");
    UARTTransmitText(__TIME__);
    UARTTransmitText(".\n\r");
    error_t err;
    err = ethernetController_init();
    if (err.code != ERROR_CODE_SUCCESSFUL)
        printErrorMessage(err);
    err = ethernetController_checkDeviceId();
    if (err.code == ERROR_CODE_SUCCESSFUL) {
        UARTTransmitText(ethernetController_getDeviceName());
        UARTTransmitText(" detected. ");
    }
    /*  UARTTransmitText("Source MAC address is ");
      UARTTransmitText(macToString(ethernetController_getMacAddress()));
      UARTTransmitText(".\n\r");*/


    UARTTransmitText(UART_special(UART_LINE_SEPARATOR));
    UARTTransmitText("\n\r");

    stack_init();

    ethernetController_setLEDConfig(LEDA, LED_ON);
    ethernetController_setLEDConfig(LEDA, LED_ON);
    __delay_ms(500);

    ethernetController_setLEDConfig(LEDA, LED_OFF);
    ethernetController_setLEDConfig(LEDA, LED_OFF);


    uint32_t prescaler = 0;

    BUTTON_ANSEL;
    BUTTON_TRISB;

    uint8_t oldState;

    ipv4_address_t ipDst;
    ipDst.address[0] = 169;
    ipDst.address[1] = 254;
    ipDst.address[2] = 108;
    ipDst.address[3] = 135;

    //Now everything's set up, allow interrupts
    INTCON0bits.GIE = 1;
    //INTCONbits.PEIE = 1;

    srand(ethernetController_getMacAddress().address[5]);
    stack.source = ipv4_generateAutoIP();

    tcp_connection_t connection;
    connection.srcPort = 1;
    connection.seqNumber = (unsigned) rand();
    tcp_listen(&connection);

    udpHeader_t udp;
    udp.srcPort = 1;
    udp.dstPort = 49666;
    udp.length = 100 + 8;




    while (1) {
        CLRWDT(); //clear watch doggy

        handleStackBackgroundTasks(&stack);

        printEthernetState(stack.ethernet);
        numberToDisplay = (stack.ethernet.link == NO_LINK) ? 1000u : foo;
        //numberToDisplay = (stack.ethernet.link == NO_LINK) ? 1000u : ethernetController_getCurrentPacketCount();
        //numberToDisplay = (stack.ethernet.link == NO_LINK) ? 1000 : stack.source.address[3];

        if (unlocked) {
            if (stack.ethernet.link == LINK_ESTABLISHED && !stack.background.fPacketPending) {
                foo++;
                udp_sendPacket(udp, ipDst);
            }
        }


        if (buttonState) {
            buttonState = 0;
            foo = 0;
            unlocked = !unlocked;
            //icmp_sendEchoRequest(ipDst);
        }
    }
}

void printEthernetState(ethernetConnection_t state) {
    static ethernetConnection_t oldState;
    if (state.link != oldState.link) {
        UARTTransmitText("[ETH]: ");
        if (state.link == NO_LINK) {
            UARTTransmitText("Link lost.\n\r");
        } else if (state.link == LINK_ESTABLISHED) {
            UARTTransmitText("Link established. ");
        }

        /*  if (state.link == LINK_ESTABLISHED) {//Display other info only if there's a link
              if (state.speed == TEN_MBIT) {
                  UARTTransmitText("(10Mbps)");
              } else if (state.speed == HUNDRED_MBIT) {
                  UARTTransmitText("(100Mbps)");
              }
              if (state.duplex == HALF_DUPLEX) {
                  UARTTransmitText("connection is Half Duplex.\n\r");
              } else if (state.duplex == FULL_DUPLEX) {

                  UARTTransmitText("connection is Full Duplex.\n\r");
              }
          }*/
    }
    oldState = state;
}

void printErrorMessage(error_t err) {
    /*UARTTransmitText("[ERROR]");
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
        case ERROR_ARP_MAXIMUM_NUMBER_OF_PROBES_REACHED:
            UARTTransmitText("IP address could not be resolved.");
        default:
        case ERROR_ETHERNET_CONTROLLER_UNKNOWN:
            UARTTransmitText("An unknown error has occured.");
            break;
    }

    UARTTransmitText(" (Code: ");
    UARTTransmitText(intToString(err.code, 10));
    UARTTransmitText(").\n\r");*/
}

void buttonHandler(uint8_t volatile *state) {
    uint8_t static oldState = 0;
    uint8_t static newState = 0;
    uint32_t static debounceCounter = 0;
    const uint32_t debounceValue = 0x1f;
    const uint32_t resetValue = 0x3ff;
    if (BUTTON_STATE) {
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
    if (debounceCounter > resetValue)
        RESET();

    oldState = newState;
}

__interrupt(irq(CCP1), high_priority) void ccp1Int(void) {

    if (CCP1IE && CCP1IF) {
        CCP1IF = 0;
        //PORTBbits.RB6 = ~PORTBbits.RB6;
        updateTime();
    }
}

__interrupt(irq(TMR0), high_priority) void tmr0Int(void) {

    if (TMR0IE && TMR0IF) {//Timer 0 Overflow interrupt
        TMR0IF = 0;
        sevenSegmentUpdate(numberToDisplay);
        buttonHandler(&buttonState);
    }

    /*  if (INTCON3bits.INT2IF) {
         INTCON3bits.INT2IF = 0;*/
    /* IMPORTANT: 
     * This interrupt is not cleared (so it won't re-trigger) until
     * every bit in the EIR register of the ENC424J600 is cleared.
     * Note that the RX Packet Pending Flag cannot be cleared by software directly, 
     * it must be cleared by setting the packet count to zero! 
     */
    //}
    /*if (RCIF) { //UART data received
        // interrupt flag cleared when buffer is read
        //UARTReceptionHandler();
    }*/



}