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
    P5SEL0 = 0x00; //sets P5 to GPIO
    P5SEL1 = 0x00; //sets P5 to GPIO
    P1DIR = BIT0; //sets LED at P1.0 to output

    PM5CTL0 &= ~LOCKLPM5;

    __bis_SR_register(GIE); //global interupt enable (enables all maskable interupts)

    TA0CCTL0 = CCIE; // CCR0 interrupt enabled
    TA0CCR0 = 60000; //register that timer will compare itself too

    P5REN = BIT6; //Resistor enabled for button P5.6
    P5OUT = BIT6; //Sets resistor to pull up for button

    P5IE |= BIT6; //interupt enable for button P5.6
    P5IFG &= ~BIT6; //Clears interupt flag register

    for(;;){}
    return 0;
}

//Interupt when button is pressed
#pragma vector=PORT5_VECTOR
__interrupt void Port_5(void){
    P1OUT ^= BIT0; //toggles LED when interupt triggered
    P5IE &=~BIT6; //interupt disable
    TA0CTL = TASSEL_2 + MC_1 + TACLR; // SMCLK, upmode, clear TAR
    P5IFG &= ~BIT6; //Clears interupt flag register needed otherwise will be stuck in infinite loop
}

//interupt when CCR0 is reached by TA0
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void){
    TA0CTL &= ~BIT4; //stops timer
    P5IE |= BIT6; //interupt enable
    P5IFG &= ~BIT6; //Clears interupt flag register for button needed otherwise will be stuck in infinite loop
}
