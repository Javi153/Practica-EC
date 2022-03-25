#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{
    PM5CTL0 &= ~LOCKLPM5; // Disable the GPIO power-on default high-impedance mode
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    P1DIR |= 0x01;                          // Set P1.0 to output direction
    P1OUT |= 0x02;
    P1REN |= 0x02;
    while(1){
        if(0x02 & P1IN) P1OUT |= 0x01;
        else P1OUT &= ~0x01;
    }
    return 0;
}
