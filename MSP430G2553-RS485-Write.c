	//=========================================================================//
	//    A small program to transmit a byte through the RS485 network         // 
	//=========================================================================//
	
	//=========================================================================//
    // www.xanthium.in				                                           //
    // Copyright (C) 2015 Rahul.S                                              //
    //=========================================================================//

	//-------------------------------------------------------------------------//
	// Hardware                                                                //                                                                                 
    //-------------------------------------------------------------------------//
    // MSP430G2553 on LaunchPad Development board Connected to MAX485.         //
    // Clocks   :- DC0 @ 1MHz ,SMCLK @1MHz                                     //
    // Baudrate :- 9600bps                                                     //
    //    [MSP430] P1.1(RXD) <- R0 [RS485](Recieve Out)                        //
    //    [MSP430] P1.2(TXD) -> DI [RS485](Driver Input)                       //
    //    [MSP430] P2.0      -> DE [RS485](Driver Out Enable)                  //
    //    [MSP430] P2.1      -> RE [RS485](Recieve Enable)                     //
    //-------------------------------------------------------------------------//
    // Compiled on IAR Embedded Workbench for MSP430 version 5.30.1            //
    // 19-March-2015                                                           //
    // Rahul.S                                                                 //
    //-------------------------------------------------------------------------//
	

	#include "msp430g2553.h"
    void main(void)
    {
      WDTCTL = WDTPW + WDTHOLD; // Stop the Watch dog
     
      //------------------- Configure the Clocks -------------------//
        
      if (CALBC1_1MHZ==0xFF)   // If calibration constant erased
         {	
            while(1);          // do not load, trap CPU!!
         } 

      DCOCTL  = 0;             // Select lowest DCOx and MODx settings
      BCSCTL1 = CALBC1_1MHZ;   // Set range
      DCOCTL  = CALDCO_1MHZ;   // Set DCO step + modulation 
	  
	  //---------------- Configuring the LED's ----------------------//
	  
	  P1DIR  |=  BIT0;  // P1.0 output
	  P1OUT  &= ~BIT0;  // P1.0 = 0
	  
	  //------------ Configuring MAX485 Control Lines ---------------//
	  
	  P2DIR  |= BIT0 + BIT1; // P2.0 -> DE,P2.1 -> ~RE output
	  P2OUT   = 0x00;        // P2.0 and P2.1 = 0
	  
	  //--------- Setting the UART function for P1.1 & P1.2 ---------//
      
	  P1SEL  |=  BIT1 + BIT2;  		    // P1.1 UCA0RXD input
      P1SEL2 |=  BIT1 + BIT2;  		    // P1.2 UCA0TXD output
	  
	  //------------ Configuring the UART(USCI_A0) ----------------//
	  
	  UCA0CTL1 |=  UCSSEL_2 + UCSWRST;  // USCI Clock = SMCLK,USCI_A0 disabled
      UCA0BR0   =  104;                 // 104 From datasheet table-  
      UCA0BR1   =  0;                   // -selects baudrate =9600,clk = SMCLK
      UCA0MCTL  =  UCBRS_1;             // Modulation value = 1 from datasheet
      UCA0CTL1 &= ~UCSWRST;             // Clear UCSWRST to enable USCI_A0
	  
	 //----------------- Put MAX485 in Transmit Mode ---------------//
	  
	  P2OUT |= BIT0 + BIT1;  // P2.0 ->  DE high,P2.1 -> ~RE high
	 
	  UCA0TXBUF = 'A';       // Transmit a byte 
	  
	  _BIS_SR(LPM0_bits + GIE); // Going to LPM0
	}

	
	