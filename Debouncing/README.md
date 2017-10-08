# Software Debouncing
By Bryan Regn
Created 9/27
Lasted updated 10/8

## Dependancies 
Similar to all labs MSP430.h was required. 

## Functionality and differences 
Differences between boards includes change of pins with regaurd to secondary LEDs and buttons, and the differences between SEL registers from board to board. Also the FR2311 used TimerB instead of TimerA. Lastly, for the MSP430FR family of processors the following line of code was required to turn off the default high impedance mode on GPIO: PM5CTL0 &= ~LOCKLPM5;

PxSEL bits are defaulted to GPIO and not periferals; so LEDs can be used as GPIO. PxDIR bits for LEDs had to be set to '1' or output. Global interupt enable had to be used to enable all maskable interupts to use timer interupt. Timer interup enable was set using TA0CCTL0 = CCIE; The capture control register was set to 60,000 (what the timer will compare itself to when triggering interupt). Resistor enable for the button was set using PxREN. The resistor was set to pull up so that when button is not pressed the value is '1', and '0' when pressed. Interupt enable for the button was set to trigger on the falling edge or button press. Lastly the timer was set to use SMCLK, stop mode, and it was cleared. 

### Interupts 
For the button interupt, the led is toggled, the button interupt was disabled. Timer was cleared and set to up mode, also the interupt flag register was cleared. 

For the timer interupt the timer was stopped, button interupt was reinabled and the interupt flag register was cleared. 