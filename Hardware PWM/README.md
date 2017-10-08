# Hardware PWM
By Bryan Regn
Created 9/27
Last updated 10/8

## Dependancies 
Similar to all labs MSP430.h was required. 

## Functionality and differences 
Differences between boards includes change of pins with regaurd to secondary LEDs and buttons, and the differences between SEL registers from board to board. Also the FR2311 used TimerB instead of TimerA. Lastly, for the MSP430FR family of processors the following line of code was required to turn off the default high impedance mode on GPIO: PM5CTL0 &= ~LOCKLPM5;

PxSEL bits are defaulted to GPIO and not periferals; so LEDs can be used as GPIO. PxDIR bits for LEDs had to be set to '1' or output. Global interupt enable had to be used to enable all maskable interupts to use timer interupt. Hardware was used to implement PWM with TAxCCTL0 = OUTMOD_X. The timer could be A or B depending on the processor. The period is set with CCR0 and the duty cycle with CCR1. OUTMOD_7 was used which is reset/set, this way with a larger CCR1 the duty cycle will also be larger. Interupt enable for the button was set to trigger on the falling edge or button press. Lastly the timer was set to use SMCLK divided by 8, up counting mode, and the timer was cleared. 

### Interupts
For the button interupt it checks if the button is pressed or not. The edge select is flipped and the LED is toggled indicating that the button is pressed or not. Then while still in the button interupt it checks the value of the duty cycle to see if it has reached 100% or not. If less than 100% it adds 10% making the duty cycle longer. If 100% was reached then it is reset to a very low duty cycle. 



