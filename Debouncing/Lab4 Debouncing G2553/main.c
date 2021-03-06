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
    P1SEL = 0x00; //sets P1 to GPIO
    P1SEL2 = 0x00; //sets P1 to GPIO
    P1DIR = BIT0; //sets LED at P1.0 to output

    __bis_SR_register(GIE); //global interupt enable (enables all maskable interupts)

    TA0CCTL0 = CCIE; // CCR0 interrupt enabled
    TA0CCR0 = 60000; //register that timer will compare itself too

    P1REN = BIT3; //Resistor enabled for button P1.3
    P1OUT = BIT3; //Sets resistor to pull up for button

    P1IE |= BIT3; //interupt enable for button P1.3
    P1IFG &= ~BIT3; //Clears interupt flag register

    for(;;){}
    return 0;
}

//Interupt when button is pressed
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void){
    P1OUT ^= BIT0; //toggles LED when interupt triggered
    P1IE &=~BIT3; //interupt disable
    TA0CTL = TASSEL_2 + MC_1 + TACLR; // SMCLK, upmode, clear TAR
    P1IFG &= ~BIT3; //Clears interupt flag register needed otherwise will be stuck in infinite loop
}

//interupt when CCR0 is reached by TA0
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void){
    TA0CTL &= ~BIT4; //stops timer
    P1IE |= BIT3; //interupt enable
    P1IFG &= ~BIT3; //Clears interupt flag register needed otherwise will be stuck in infinite loop
}
