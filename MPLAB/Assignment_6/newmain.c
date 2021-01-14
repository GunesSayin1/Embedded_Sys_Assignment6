/*******************************************
2 Assignment 6 - Solar Irradiance Meter
3 Gunes Sayin, Hristiyan Georgiev, Mirte Winters
4 Embedded Systems Semester 5
5 Sensor Technology Hanzehogeschool
6 Date: 13/01/2021
7 ********************************************/

#define __18F25K80
#include "xc.h"
#include "fuses.h"
#include <stdio.h>
#include<stdbool.h>

#define _XTAL_FREQ 8000000  // X-tal = 8 MHz

int _Analog_Digital_convertor(void);

#define button1 PORTBbits.RB0
#define button2 PORTBbits.RB1
#define gee1 LATCbits.LATC4

void putch (char c){
    while(TXSTA1bits.TRMT==0);
    TXREG1 = c;
}

void interrupt MyIsr (void){
    if(INTCONbits.TMR0IE && INTCONbits.TMR0IF)
    {
        INTCONbits.TMR0IF = 0;
        TMR0H = 0x48; //72 in decimal
        TMR0L = 0xE5; //229 in decimal
        float Solar = ((_Analog_Digital_convertor())/4095.0) * 5.0;
        printf("%f\n\r",Solar);
    }

    if(INTCONbits.INT0E && INTCONbits.INT0F)
    {
        gee1 = 0;
        INTCONbits.TMR0IE = 0;
        INTCONbits.INT0F =0;
    }
    if(INTCON3bits.INT1E && INTCON3bits.INT1F)
    {
        gee1 = 1;
        INTCONbits.TMR0IE = 1;
        INTCON3bits.INT1F = 0;
    }

}


void main(void) {
    TRISCbits.TRISC6 = 0; // TX Output 1

    PIE3bits.RC2IE = 0; //Disable Rx interrupt USART2
    PIE3bits.TX2IE = 0; //Disable Tx interrupt USART2
    PIE1bits.RC1IE = 0; //Disable Rx interrupt USART1
    PIE1bits.TX1IE = 0; //Disable Tx interrupt USART1

    //bit 7 and bit 5 are set to 1 all others are set to 0.
    //Master mode, transmit enabled
    TXSTA1 = 0b10100000;
    //bit 7 and 4 are set to 1 all others are set to zero.
    //serial port is enabled.
    RCSTA1 = 0b10000000; 
     //bit 7 and bit 6 are set to 1 all others are set to 0.
    //receive operation is idle.
    BAUDCON1 = 0b11000000; //
     //transmission speed is set to 9600 with SPBRG1 = 12
    SPBRG1 = 0x0C; //for 9600 b/s

    TRISBbits.TRISB0 = 1;
    TRISBbits.TRISB1 = 1;
    ANCON1bits.ANSEL10 = 0;
    ANCON1bits.ANSEL8 = 0;
    T0CON = 0b10000110; //1:128
    INTCON2bits.TMR0IP = 1;
    INTCONbits.TMR0IF = 0;
    INTCONbits.TMR0IE = 1;
    INTCON2bits.INTEDG0 = 0;
    INTCONbits.INT0E = 1;
    INTCON3bits.INT1E = 1;
    INTCON2bits.INTEDG1 = 1;
    ei();
    while(1);
}

// Initialize ADC
int _Analog_Digital_convertor(void) {
  ADCON0 = 0b00100111;//channel ANI, Start ADConversion, ADC on
  ADCON1 = 0b00000000;//trigger ECCPI, AVdd, AVss, Neg Channel00
  ADCON2 = 0b10000000;//Right justified, Tad = 0, conversion clock Fosc/2
  while (ADCON0bits.GO ==  1); //Conversion ready
  return ADRESH<<8|ADRESL;
}

