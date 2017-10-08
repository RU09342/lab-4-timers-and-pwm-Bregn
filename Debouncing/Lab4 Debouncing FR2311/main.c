#include <msp430.h> 
//By Bryan Regn
//Last Updated: 10/7
/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    //needed to reset device
    P1SEL0 = 0x00; //sets P1 to GPIO
    P1SEL1 = 0x00; //sets P1 to GPIO
    P1DIR = BIT0; //sets LED at P1.0 to output

    PM5CTL0 &= ~LOCKLPM5;

    __bis_SR_register(GIE); //global interupt enable (enables all maskable interupts)

    TB0CCTL0 = CCIE; // CCR0 interrupt enabled
    TB0CCR0 = 60000; //register that timer will compare itself too

    P1REN = BIT1; //Resistor enabled for button P1.1
    P1OUT = BIT1; //Sets resistor to pull up for button

    P1IE |= BIT1; //interupt enable for button P1.1
    P1IFG &= ~BIT1; //Clears interupt flag register

    for(;;){}
    return 0;
}

//Interupt when button is pressed
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void){
    P1OUT ^= BIT0; //toggles LED when interupt triggered
    P1IE &=~BIT1; //interupt disable
    TB0CTL = TBSSEL_2 + MC_1 + TBCLR; // SMCLK, upmode, clear TAR
    P1IFG &= ~BIT1; //Clears interupt flag register needed otherwise will be stuck in infinite loop
}

//interupt when CCR0 is reached by TA0
#pragma vector=TIMER0_B0_VECTOR
__interrupt void TIMER0_B0_ISR(void){
    TB0CTL &= ~BIT4; //stops timer
    P1IE |= BIT1; //interupt enable
    P1IFG &= ~BIT1; //Clears interupt flag register needed otherwise will be stuck in infinite loop
}
