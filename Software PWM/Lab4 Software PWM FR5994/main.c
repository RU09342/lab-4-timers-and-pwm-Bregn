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

    P1SEL0 = 0x00; //sets P1 to GPIO
    P1SEL1 = 0x00; //sets P1 to GPIO
    P1DIR = BIT0 | BIT1; //sets LED at P1.0 output PWM and P1.1 output 2nd led

    PM5CTL0 &= ~LOCKLPM5;

    P5REN = BIT6; //Resistor enabled for button P5.6
    P5OUT = BIT6; //Sets resistor to pull up for button

    __bis_SR_register(GIE); //global interupt enable (enables all maskable interupts)

    TA0CCTL0 = CCIE; // CCR0 interrupt enabled
    TA0CCTL1 = CCIE; // CCR0 interrupt enabled
    TA0CCR0 = 1250; //register that timer will compare itself too
    TA0CCR1 = 625; //duty cycle, initially set to 50% duty cycle

    P5IFG &= ~BIT6; //Clears interupt flag register

    P5IE |= BIT6; //interupt enable for button P5.6
    P5IES |= BIT6; //sets interupt to trigger on falling edge (button press)

    P1OUT |= BIT0; //initializes led on

    TA0CTL |= BIT1;
    TA0CTL = TASSEL_2 + ID_3 + MC_1 + TACLR; // SMCLK, divide clock by 8, upmode, clear TAR

    for(;;){}
    return 0;
}

//Interupt when button is pressed
#pragma vector=PORT5_VECTOR
__interrupt void Port_5(void){

    //blinks 2nd led when button is pressed
    if ((P5IN&BIT6)==0x0000) //if button pressed
    {
    P5IES &= ~BIT6; //sets interupt to trigger on rising edge (button release)
    P1OUT^= BIT1; // toggle led
    }
    else //if button released
    {
    P5IES |= BIT6; //sets interupt to trigger on falling edge (button press)
    P1OUT^= BIT1; // toggle led
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

    P5IFG &= ~BIT6; //Clears interupt flag register needed otherwise will be stuck in infinite loop
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
