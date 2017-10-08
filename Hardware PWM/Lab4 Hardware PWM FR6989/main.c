#include <msp430.h> 
//By Bryan Regn
//Last updated 10/8

/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    P1SEL0 = 0x01; //sets P1.0 to TA0.1
    P1SEL1 = 0x00; //sets P1.0 to TA0.1
    P1DIR = BIT0; //sets P1.0 to TA0.1

    P9DIR = BIT7; //sets P9.7 to output (2nd LED)

    PM5CTL0 &= ~LOCKLPM5;

    P1REN = BIT1; //Resistor enabled for button P1.1
    P1OUT = BIT1; //pull up resistor for button P1.1

    __bis_SR_register(GIE); //global interupt enable (enables all maskable interupts)

    TA0CCTL1 = OUTMOD_7; //PWM reset/set
    TA0CCR0 = 1250; //period
    TA0CCR1 = 625; //duty cycle initially set to 50%

    P1IFG &= ~BIT1; //Clears interupt flag register
    P1IE |= BIT1; //interupt enable for button P1.1
    P1IES |= BIT1; //sets interupt to trigger on falling edge (button press)


    TA0CTL = TASSEL_2 + ID_3 + MC_1 + TACLR; // SMCLK, divide clock by 8, up mode, clear TAR

    for(;;){}
    return 0;
}

//Interupt when button is pressed
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void){
    //blinks 2nd led when button is pressed
    if ((P1IN&BIT1)==0x0000) //if button pressed
    {
    P1IES &= ~BIT1; //sets interupt to trigger on rising edge (button release)
    P9OUT^= BIT7; // toggle 2nd led
    }
    else //if button released
    {
    P1IES |= BIT1; //sets interupt to trigger on falling edge (button press)
    P9OUT^= BIT7; // toggle led
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

    P1IFG &= ~BIT1; //Clears interupt flag register needed otherwise will be stuck in infinite loop
}
