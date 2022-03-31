#include <msp430.h> 


/**
 * main.c
 */

char letra = 'A';
int main(void)
{
    PM5CTL0 &= ~LOCKLPM5; // Disable the GPIO power-on default high-impedance mode
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	P3SEL1 &= ~0x0030; //Funcion modulo primario de P3.4 y P3.5
	P3SEL0 |= 0x0030;

	CSCTL0_H = CSKEY >> 8; // Unlock clock registers
	CSCTL1 = DCOFSEL_3 | DCORSEL; // Set DCO to 8MHz
	CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
	CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1; // Set all dividers
	CSCTL0_H = 0; // Lock CS registers

	// Configure USCI_A1 for UART mode
	UCA1CTLW0 = UCSWRST; // Put eUSCI in reset
	UCA1CTLW0 |= UCSSEL__SMCLK; // CLK = SMCLK
	// Baud Rate calculation
	// 8000000/(16*9600) = 52.083
	// Fractional portion = 0.083
	// User's Guide Table 21-4: UCBRSx = 0x04
	// UCBRFx = int ( (52.083-52)*16) = 1
	UCA1BR0 = 52; // 8000000/16/9600
	UCA1BR1 = 0x00;
	UCA1MCTLW |= UCOS16 | UCBRF_1 | 0x4900;
	UCA1CTLW0 &= ~UCSWRST; // Initialize eUSCI
	// Configure interruptions to receive and transmit data in register UCA1IE

	UCA1IE |= UCTXIE; //Activamos interrupciones de transmision
	UCA1CTLW0 &= ~0x5800; //Activamos un bit de stop, 8 bits y sin paridad

	_BIS_SR(GIE); //Activamos interrupciones globales
	return 0;
}

#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void) {
    if(letra == 'Z'){
        letra = 'A'; //Llegamos al final y volvemos a la A
        UCA1TXBUF = letra; //Cargamos la letra en el buffer
    }
    else{
        letra = letra + 1; //Pasamos a la siguiente letra
        UCA1TXBUF = letra; //Cargamos la letra en el buffer
    }
}
