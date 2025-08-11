/*
 * File:   UART.c
 * Author: sooju
 *
 * Created on 2025? 8? 11? (?), ?? 10:31
 */

#include "UART.h"

#define MAX_NMEA_LENGTH 200

void enableUARTInterrupt() {
    PIE1bits.RCIE = 1; 
}

void disableUARTInterrupt() {
    PIE1bits.RCIE = 0; 
}

void enableUARTReceive() {
    RCSTAbits.CREN = 1;  
}

void disableUARTReceive() {
    RCSTAbits.CREN = 0;  
}

void UART_Init_115200() {
    TXSTA=0x24;
    RCSTA=0x90;
    SPBRG = 10;  
   
}

void UART_Init_9600() {
    TXSTA=0x24;
    RCSTA=0x90;
    SPBRG = 129;  
   
}

void UART_Write(char data) {
    while (!TXSTAbits.TRMT);  
    TXREG = data;  
}

void UART_Write_String(const char* str) {
    while (*str) {
        UART_Write(*str++);  
    }
}

char UART_Read() {
    while (!PIR1bits.RCIF); 
    return RCREG; 
}

void UART_Receive_Data(char* buffer) {
    enableUARTReceive(); 
    char received_char;
    int i = 0;
    memset(buffer, 0, MAX_NMEA_LENGTH);

    while (1) {
        received_char = UART_Read();

        
        if (i >= MAX_NMEA_LENGTH - 1 || (received_char == '$' && i > 0)) {
            i = 0; 
            memset(buffer, 0, MAX_NMEA_LENGTH); 
            buffer[i++] = '$'; 
            continue;
        }

        buffer[i++] = received_char;  

        
        if (received_char == '\n') {
            buffer[i] = '\0'; 
            break;
        }
    }
    disableUARTReceive();
}