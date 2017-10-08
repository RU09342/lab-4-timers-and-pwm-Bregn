#include <msp430.h> 
//By Bryan Regn
//Last Updated: 10/8
/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    //needed to reset device

    P1SEL = 0x00; //sets P1 to GPIO
    P1SEL2 = 0x00; //sets P1 to GPIO
    P1DIR = BIT0 | BIT6; //sets LED at P1.0 and P1.6 to output

    P1REN = BIT3; //Resistor enabled for button P1.3
    P1OUT = BIT3; //Sets resistor to pull up for button

    __bis_SR_register(GIE); //global interupt enable (enables all maskable interupts)

    TA0CCTL0 = CCIE; // CCR0 interrupt enabled
    TA0CCTL1 = CCIE; // CCR0 interrupt enabled
    TA0CCR0 = 1250; //register that timer will compare itself too
    TA0CCR1 = 625; //duty cycle, initially set to 50% duty cycle

    P1IFG &= ~BIT3; //Clears interupt flag register

    P1IE |= BIT3; //interupt enable for button P1.3
    P1IES |= BIT3; //sets interupt to trigger on falling edge (button press)

    P1OUT |= BIT0; //initializes led on

    TA0CTL |= BIT1;
    TA0CTL = TASSEL_2 + ID_3 + MC_1 + TACLR; // SMCLK, divide clock by 8, upmode, clear TAR

    for(;;){}
    return 0;
}

//Interupt when button is pressed
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void){

    //blinks 2nd led when button is pressed
    if ((P1IN&BIT3)==0x0000) //if button pressed
    {
    P1IES &= ~BIT3; //sets interupt to trigger on rising edge (button release)
    P1OUT^= BIT6; // toggle led
    }
    else //if button released
    {
    P1IES |= BIT3; //sets interupt to trigger on falling edge (button press)
    P1OUT^= BIT6; // toggle led
    }

    //adds 10% to duty cycle or resets back to 0%
    if(TA0CCR1<1250) //checks if duty cycle is less than 100%
    {
    TA0CCR1 += 125; //adds 10% to duty cycle
    }
    else
    {
     TA0CCR1 = 20; // if duty cycle is at 100% it is reset to 0% not actually 0 b/c it does not trigger on 0
    }

    P1IFG &= ~BIT3; //Clears interupt flag register needed otherwise will be stuck in infinite loop
}

//interupt when CCR0 is reached by TA0
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void){
    P1OUT ^= BIT0; //toggles LED when interupt triggered
    TA0CCTL0 &= ~BIT0; //clears interupt flag
}

//interupt when CCR1 is reached by TA0
#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR(void){
    P1OUT ^= BIT0; //toggles LED when interupt triggered
    TA0CCTL1 &= ~BIT0; //clears interupt flag
}
