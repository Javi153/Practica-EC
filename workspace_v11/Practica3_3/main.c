#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{
    PM5CTL0 &= ~LOCKLPM5; // Disable the GPIO power-on default high-impedance mode
    WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

    P1DIR |= 0x01;

    // Configuración TIMER_A:
    // TimerA0, ACLK/1, modo up, reinicia TACLR

    TA0CTL = TASSEL_1 | TACLR | MC_1;
    // ACLK tiene una frecuencia de 32768 Hz
    TA0CCR0 = 40000;
    TA0CCTL0 = CCIE; // Habilita interrupción (bit CCIE)

    _BIS_SR(LPM4_bits + GIE);
}



// Rutina de interrupción de TIMER0_A0
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void){
    P1OUT ^= 0x01; // conmuta LED en P1.0
}
