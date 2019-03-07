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

/*void UARTTerminalHandler(char *buf, uint8_t bufferPointer) {
    while (bufferPointer){
        buf[bufferPointer--]
    }
}

void UARTReceptionHandler() {
    uint8_t receivedCharacter = RCREG1;
    uint8_t static bufferPointer = 0;
    uint8_t static buffer[64]; //max string length
    buffer[bufferPointer++] = receivedCharacter;
    if (receivedCharacter = '\n') {
        UARTTerminalHandler(&buffer[0], bufferPointer);
        bufferPointer = 0;
    }
}*/

void UARTTransmitText(const char *str) {
    while (*str) {
        TXREG1 = *str++;
        while (TRMT == 0);
    }
}

void UARTTransmitInt(uint32_t val) {
    UARTTransmitText(intToString(val));
    /*   uint8_t tenthousands = val / 10000;
       uint8_t thousands = (val - tenthousands * 10000) / 1000;
       uint8_t hundreds = (val - tenthousands * 10000 - thousands * 1000) / 100;
       uint8_t tens = (val - tenthousands * 10000 - thousands * 1000 - hundreds * 100) / 10;
       uint8_t ones = val % 10;

       if (val > 9999) {
           while (TRMT == 0);
           TXREG1 = tenthousands + 0x30;
       }

       if (val > 999) {
           while (TRMT == 0);
           TXREG1 = thousands + 0x30;
       }

       if (val > 99) {
           while (TRMT == 0);
           TXREG1 = hundreds + 0x30;
       }

       if (val > 9) {
           while (TRMT == 0);
           TXREG1 = tens + 0x30;
       }

       while (TRMT == 0);
       TXREG1 = ones + 0x30;*/
}

char *intToString(uint32_t val) {
    char static number [10];
    uint8_t pointer = 0;

    uint8_t tenthousands = val / 10000u;
    uint8_t thousands = (val - tenthousands * 10000u) / 1000u;
    uint8_t hundreds = (val - tenthousands * 10000u - thousands * 1000u) / 100u;
    uint8_t tens = (val - tenthousands * 10000u - thousands * 1000u - hundreds * 100u) / 10u;
    uint8_t ones = val % 10u;

    if (val > 9999) {
        number[pointer++] = tenthousands + 0x30u;
    }

    if (val > 999) {
        number[pointer++] = thousands + 0x30u;
    }

    if (val > 99) {
        number[pointer++] = hundreds + 0x30u;
    }

    if (val > 9) {
        number[pointer++] = tens + 0x30u;
    }

    number[pointer++] = ones + 0x30u;
    number[pointer++] = '\0';

    return number;
}

char hexToChar(uint8_t val) {
    if (val < 10) {
        return val + 0x30u;
    } else if (val < 16) {
        return val + 0x41u - 10;
    }
    return 0;
}

char *hexToString(uint32_t val) {
    char static number [10];
    uint8_t pointer = 0;
    if (val > 0xff) {
        number[pointer++] = hexToChar((val >> 12) & 0xf);
        number[pointer++] = hexToChar((val >> 8) & 0xf);
    }
    number[pointer++] = hexToChar((val >> 4) & 0xf);
    number[pointer++] = hexToChar(val & 0xf);

    number[pointer++] = '\0';
    return number;
}

char *macToString(macaddress_t addr) {
    char address[] = "??:??:??:??:??:??";
    uint8_t j = 0;
    uint8_t firstDigit;
    uint8_t secondDigit;

    for (int i = 0; i < 6; i++) {
        firstDigit = ((addr.address[i] & 0xF0) >> 4)&0x0F; //parse digits
        secondDigit = addr.address[i] & 0x0F;

        firstDigit += 0x30; //ASCII conversion
        secondDigit += 0x30;

        if (firstDigit > '9') {//is it a hexadecimal letter?
            address[j++] = firstDigit + 7; //ASCII conversion (letters)
        } else {
            address[j++] = firstDigit; //it's a number
        }
        if (secondDigit > '9') {//is it a hexadecimal letter?
            address[j++] = secondDigit + 7; //ASCII conversion (letters)
        } else {
            address[j++] = secondDigit; //it's a number
        }

        if (i < 5) {
            address[j++] = ':';
        }
    }
    return &address;
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
    char static address [] = "???.???.???.???";
    uint8_t hundreds[4];
    uint8_t tens[4];
    uint8_t ones[4];
    uint8_t pointer = 0;

    for (uint8_t i = 0; i < 4; i++) {

        hundreds[i] = ip.address[i] / 100u;
        tens[i] = (ip.address[i] - hundreds[i]*100u) / 10u;
        ones[i] = ip.address[i] % 10u;

        if (hundreds[i] > 0)
            address[pointer++] = hundreds[i] + 0x30;
        if ((tens[i] > 0) || hundreds[i] > 0)
            address[pointer++] = tens[i] + 0x30;
        address[pointer++] = ones[i] + 0x30;
        if (i != 3)
            address[pointer++] = '.';
    }
    address[pointer] = '\0';
    return address;
}

char *arpEntryToString(ARP_tableEntry_t table) {
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

    tempAddr = intToString(table.timeCreated % 0xffff);
    while (*tempAddr != '\0')
        string[pointer++] = *(tempAddr++);

    string[pointer] = '\0';

    return string;
}

