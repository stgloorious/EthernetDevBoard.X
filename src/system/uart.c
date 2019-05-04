/**
 * \file uart.c
 * \brief Basic UART functions
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

#include "../system/uart.h"

void UARTInit(void) {
    TXSTA1bits.TXEN = 1;
    TXSTA1bits.SYNC = 0;
    RCSTA1bits.SPEN = 1;
    TXSTA1bits.TX9 = 1;
    TXSTA1bits.TX9D = 1;
    SPBRG = 3; /* BAUDRATE IS 125000 */

}

void UARTTransmitText(const char *str) {
    while (*str) {
        TXREG1 = *str++;
        while (TRMT == 0);
    }
}

void UART_resetFormat() {
    UARTTransmitText("\033[0m");
}

void UART_setFormat(uint8_t color) {
    switch (color) {
        case UART_COLOR_BG_RED:
            UARTTransmitText("\033[41;10;10m");
            break;
        case UART_COLOR_BG_GREEN:
            UARTTransmitText("\033[42;30;10m");
            break;
        case UART_COLOR_BG_BLUE:
            UARTTransmitText("\033[44;10;10m");
            break;
        case UART_COLOR_BG_MAGENTA:
            UARTTransmitText("\033[45;10;10m");
            break;
        case UART_COLOR_BG_CYAN:
            UARTTransmitText("\033[46;30;10m");
            break;
        case UART_COLOR_BG_YELLOW:
            UARTTransmitText("\033[103;30;10m");
            break;
        case UART_COLOR_FG_YELLOW:
            break;
        case UART_COLOR_FG_BLUE:
            UARTTransmitText("\033[94;4;10m");
            break;
    }
}

char *UART_special(uint8_t code) {
    switch (code) {
        case UART_LINE_SEPARATOR:
            return (const char*)"-------------------------------------------------";
    }
}

char *intToString(uint32_t val, uint8_t b) {
    char static number [10];
    ltoa(&number[0], (long) val, b);
#ifndef __XC
#warning "Check if ltoa() is compatible with used compiler"
#endif
    return number;
}

char *macToString(macaddress_t addr) {
    char static address[20];
    char static number[3];

    uint8_t j = 0;
    for (uint8_t i = 0; i < 6; i++) {
        itoa(&number[0], (int) addr.address[i], 16);
        address[j++] = number[0];
        address[j++] = number[1];
        if (i < 5)
            address[j++] = ':';
    }
    address[j] = '\0';

    return address;
}

char *etherTypeToString(etherType_t ethertype) {
    switch (ethertype) {
        case ETHERTYPE_IPv4:
            return "IPv4";
        case ETHERTYPE_ARP:
            return "ARP";
        default:
            return "Unknown EtherType";
    }
}

char *ipProtocolToString(ipv4_protocol_t protocol) {
    switch (protocol) {
        case IPv4_PROTOCOL_ICMP:
            return "ICMP";
        case IPv4_PROTOCOL_TCP:
            return "TCP";
        case IPv4_PROTOCOL_UDP:
            return "UDP";
        default:
            return "Unknown Protocol";
    }
}

char *ipAdressToString(ipv4_address_t ip) {
    char static address [20];
    char static number[4];
    uint8_t pointer = 0;
    uint8_t j = 0;

    for (uint8_t i = 0; i < 4; i++) {
        itoa(&number[0], (int) ip.address[i], 10);
        if (ip.address[i] >= 100)
            address[pointer++] = number[j++];
        if (ip.address[i] >= 10)
            address[pointer++] = number[j++];
        address[pointer++] = number[j++];
        j = 0;
        if (i < 3)
            address[pointer++] = '.';
    }
    address[pointer] = '\0';
    return address;
}

char *arpEntryToString(arp_tableEntry_t table) {
    char static string [50];
    uint8_t pointer = 0;
    char *tempAddr;

    tempAddr = ipAdressToString(table.ip);
    while (*tempAddr != '\0')
        string[pointer++] = *(tempAddr++);

    string[pointer++] = ' ';

    tempAddr = macToString(table.mac);
    while (*tempAddr != '\0')
        string[pointer++] = *(tempAddr++);

    string[pointer++] = ' ';

    tempAddr = intToString(table.timeCreated, 10);
    while (*tempAddr != '\0')
        string[pointer++] = *(tempAddr++);

    string[pointer] = '\0';

    return string;
}