#include <xc.h>
#include <stdlib.h>

#include "UART.h"
#include "GPS.h"

#pragma config OSC = HS         // Oscillator Selection bits (HS oscillator)
#pragma config OSCS = OFF       // Oscillator System Clock Switch Enable bit (Oscillator system clock switch option is disabled (main oscillator is source))

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOR = ON         // Brown-out Reset Enable bit (Brown-out Reset enabled)
#pragma config BORV = 20        // Brown-out Reset Voltage bits (VBOR set to 2.0V)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 128      // Watchdog Timer Postscale Select bits (1:128)

// CONFIG3H
#pragma config CCP2MUX = ON     // CCP2 Mux bit (CCP2 input/output is multiplexed with RC1)

// CONFIG4L
#pragma config STVR = ON        // Stack Full/Underflow Reset Enable bit (Stack Full/Underflow will cause RESET)
#pragma config LVP = OFF         // Low Voltage ICSP Enable bit (Low Voltage ICSP enabled)

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000200-001FFFh) not code protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) not code protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) not code protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) not code protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot Block (000000-0001FFh) not code protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000200-001FFFh) not write protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) not write protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) not write protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) not write protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block (000000-0001FFh) not write protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000200-001FFFh) not protected from Table Reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) not protected from Table Reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) not protected from Table Reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) not protected from Table Reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot Block (000000-0001FFh) not protected from Table Reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#define _XTAL_FREQ 20000000  
#define MAX_NMEA_LENGTH 200
#define SEL0 PORTCbits.RC0  
#define SEL1 PORTCbits.RC1  

void initSelectionPins() {
    TRISC = 0b10000000;
    TRISCbits.RC6 = 0;
    TRISCbits.RC7 = 1;

}

void selectUARTModule_0() {

    SEL0 = 0;
    SEL1 = 0;
}

void selectUARTModule_1() {

    SEL0 = 1;
    SEL1 = 0;
}

void updateATCommand(char* atCommand, double latitude, double longitude) {
    sprintf(atCommand, "AT%%HTTPCMD=\"GET\",1,\"http://ServerIP/GPS.php?latitude=%.4f&longitude=%.4f\"", latitude, longitude);
}

void main(void) {
    char GPS_Data[MAX_NMEA_LENGTH];
    double latitude, longitude;

    char* atCommand1 = "AT%HTTPCFG=\"NODES\",1,\"http://ServerIP/GPS.php\",\"\",\"\",\"0\"";
    char atCommand2[100];

    initSelectionPins();

    while (1) {


        while (1) {
            enableUARTInterrupt();
            UART_Init_9600();
            selectUARTModule_0();
            UART_Receive_Data(GPS_Data);
            disableUARTInterrupt();

            if (isValidGPSData(GPS_Data)) {

                updateCoordinates(GPS_Data, &latitude, &longitude);
                break;
            }

            __delay_ms(500);

        }

        memset(atCommand2, 0, sizeof (atCommand2));
        updateATCommand(atCommand2, latitude, longitude);
        __delay_ms(1000);

        selectUARTModule_1();
        UART_Init_115200();
        UART_Write_String(atCommand1);
        UART_Write('\r');
        UART_Write('\n');
        __delay_ms(1000);
        UART_Write_String(atCommand2);
        UART_Write('\r');
        UART_Write('\n');
        __delay_ms(1500);


    }
}