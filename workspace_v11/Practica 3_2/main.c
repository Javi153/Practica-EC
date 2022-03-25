#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{
    PM5CTL0 &= ~LOCKLPM5; // Disable the GPIO power-on default high-impedance mode
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	P1DIR |= 0x01;
    P1OUT |= 0x02;
	P1REN |= 0x02;
	P1IE |= 0x02;
	P1IES |= 0x02;
	P1IFG &= ~0x02;

	_BIS_SR(LPM4_bits + GIE);
	return 0;
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void){
    P1OUT ^= 0x01;
    P1IFG &= ~0x02;
}
