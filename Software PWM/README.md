# Software PWM
By Bryan Regn
Created 9/27
Last updated 10/8

## Dependancies
Similar to all labs MSP430.h was required 

## Functionality and differences 
Differences between boards includes change of pins with regaurd to secondary LEDs and buttons, and the differences between SEL registers from board to board. Also the FR2311 used TimerB instead of TimerA. Lastly, for the MSP430FR family of processors the following line of code was required to turn off the default high impedance mode on GPIO: PM5CTL0 &= ~LOCKLPM5;
The setup of the software pwm was very similar to hardware. For the software pwm CCTL0 and CCTTL1 were used to enable CCR interupts for the period and duty cycle.  

### Interupts 
The button interupt was the exact same as it was for hardware PWM (see hardware PWM readme for more details). 

For CCR timer interupts the led was toggled and interupt flag was cleared.