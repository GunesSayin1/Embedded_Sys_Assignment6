#include <xc.h>

void setup_can(void){
    extern int identifier_byte0;
    extern int identifier_byte1;
    extern int identifier_byte2;
    extern int identifier_byte3;
///////CAN setup:///////////////////////////////////////////
//Initial LAT and TRIS bits for RX and TX CAN.
    TRISBbits.TRISB2 = 0; // RC6/CANTX set as output
    TRISBbits.TRISB3 = 1; // RC7/CANRX set as input

//Ensure that the ECAN module is in Configuration mode.
    CANCONbits.REQOP = 0b111; //Request configuration mode
    while(CANSTATbits.OPMODE != 0b100); // wait for configuration mode

//Select ECAN Operational mode.
    ECANCONbits.MDSEL = 0b00; //00= Legacy mode (Mode 0, default), 01= Enhanced Legacy mode (Mode 1), 10= Enhanced FIFO mode (Mode 2), 11= Reserved

//Set up the Baud Rate registers.
    BRGCON1     = 0xCF;
    BRGCON2	= 0xFF;
    BRGCON3	= 0x07;
//Set up the Filter and Mask registers.

//Set the ECAN module to normal mode or any other mode required by the application logic.
    CANCONbits.REQOP = 0b000; //Request normal mode
    while(CANSTATbits.OPMODE == 0b100); // wait for configuration mode

    // Disable the transmit buffers
    TXB0CON = 0;
    TXB1CON = 0;
    TXB2CON = 0;
    // emty tranmit buffers
    TXB0D0 = 0;
    TXB0D1 = 0;
    TXB0D2 = 0;
    TXB0D3 = 0;
    TXB0D4 = 0;
    TXB0D5 = 0;
    TXB0D6 = 0;
    TXB0D7 = 0;

    //Extended mask
    RXM0SIDLbits.EXIDEN = 1;
    RXM0SIDH = 0xFF;
    RXM0SIDLbits.SID = 0x07;
    RXM0SIDLbits.EID = 0x03;
    RXM0EIDH = 0xFF;
    RXM0EIDL = 0xFF;

    RXF0SIDLbits.EXIDEN = 1;
    RXF0SIDH = identifier_byte0;
    RXF0SIDLbits.SID = (identifier_byte1&0xE0);
    RXF0SIDLbits.EID = (identifier_byte1&0x03);
    RXF0EIDH = identifier_byte2;
    RXF0EIDL = identifier_byte3;

    TXB0SIDLbits.EXIDE = 1;
    TXB0SIDH = identifier_byte0;
    TXB0SIDLbits.SID = (identifier_byte1&0xE0);
    TXB0SIDLbits.EID = (identifier_byte1&0x03);
    TXB0EIDH = identifier_byte2;
    TXB0EIDL = (identifier_byte3+1);



/////// end CAN setup:##########################################
}