/* Name: main.c
 */

#include "teensy_general.h"  // includes the resources included in the teensy_general.h file
#include "t_usb.h"

#define DIRpin1	1	
#define DIRpin2	3
#define DIRpin3	7
#define STEPpin1	0
#define STEPpin2	2
#define STEPpin3	6
#define motorSpeed	50	//microsecond delay between pulses AKA steps (determines rotation speed)

//declare global variables
long val1, val2, val3, val4, val5;
long mappedval1, mappedval2, mappedval3, mappedval4, mappedval5;
long steps1, steps2, steps3;
long prev1, prev2, prev3;
long i,j,k;

void analogSet(int ADCpinNumber){
	
	set(ADMUX, REFS0);	//setting reference voltage for ADC to Vcc
	clear(ADMUX, REFS1);
		
	set(ADCSRA, ADPS2);	//set ADC prescaler to 125 kHz based on system clock
	set(ADCSRA, ADPS1);
	set(ADCSRA, ADPS0);
	
	if (ADCpinNumber >= 0 && ADCpinNumber<=7){
		teensy_disableJTAG();		//when using F4-7, m ust diable JTAG
		clear(DDRF, ADCpinNumber);	//set ADC pin as input on PORTF
		set(DIDR0, ADCpinNumber);	//disable digital input for ADC1-7 (PF0-7)
	}
	if (ADCpinNumber > 7 && ADCpinNumber <= 10){
		int array1[] = {0, 0, 0, 0, 0, 0, 0, 0, 4, 6, 7};	//array to convert ADC8-10 to PD4,6,7
		int array2[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2};	//array to convert ADCpinNumber to bit# on DIDR2 register
		clear(DDRD, array1[ADCpinNumber]);	//set ADC pin as input on PORTD
		set(DIDR2, array2[ADCpinNumber]);	//disable digital input for ADC pin 
	}
	if (ADCpinNumber > 10 && ADCpinNumber <= 13){
		int array3[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 5, 6};	//array to convert ADC11-13 to PB4-6
		int array4[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 5};	//array to convert ADCpinNumber to bit# on DIDR2 register
		clear(DDRB, array3[ADCpinNumber]);	//set ADC pin as input on PORTB
		set(DIDR2, array4[ADCpinNumber]);	//disable digital input for ADC pin
	}
	
	set(ADCSRA, ADATE);	//have ADC begin new conversion immediately after finishing previous conversion
}

int analogRead(int ADCpinNumber1){
	if (ADCpinNumber1 == 0){
		clear(ADCSRB, MUX5);	//ADC0 (Pin F0)
		clear(ADMUX, MUX2);
		clear(ADMUX, MUX1);
		clear(ADMUX, MUX0);
	}
	if (ADCpinNumber1 == 1){
		clear(ADCSRB, MUX5);	//ADC1 (Pin F1)
		clear(ADMUX, MUX2);
		clear(ADMUX, MUX1);
		set(ADMUX, MUX0);
	}
	if (ADCpinNumber1 == 4){
		clear(ADCSRB, MUX5);	//ADC4 (Pin F4)
		set(ADMUX, MUX2);
		clear(ADMUX, MUX1);
		clear(ADMUX, MUX0);
	}
	if (ADCpinNumber1 == 5){
		clear(ADCSRB, MUX5);	//ADC5 (pin F5)
		set(ADMUX, MUX2);
		clear(ADMUX, MUX1);
		set(ADMUX, MUX0);
	}
	if (ADCpinNumber1 == 6){
		clear(ADCSRB, MUX5);	//ADC6 (pin F6)
		set(ADMUX, MUX2);
		set(ADMUX, MUX1);
		clear(ADMUX, MUX0);
	}
	if (ADCpinNumber1 == 7){
		clear(ADCSRB, MUX5);	//ADC7 (pin F7)
		set(ADMUX, MUX2);
		set(ADMUX, MUX1);
		set(ADMUX, MUX0);
	}
	if (ADCpinNumber1 == 8){
		set(ADCSRB, MUX5);		//ADC8 (pin D4)
		clear(ADMUX, MUX2);
		clear(ADMUX, MUX1);
		clear(ADMUX, MUX0);
	}
	if (ADCpinNumber1 == 9){
		set(ADCSRB, MUX5);		//ADC9 (pin D6)
		clear(ADMUX, MUX2);
		clear(ADMUX, MUX1);
		set(ADMUX, MUX0);
	}
	if (ADCpinNumber1 == 10){
		set(ADCSRB, MUX5);		//ADC10 (pin D7)
		clear(ADMUX, MUX2);
		set(ADMUX, MUX1);
		clear(ADMUX, MUX0);
	}
	if (ADCpinNumber1 == 11){
		set(ADCSRB, MUX5);		//ADC11 (pin B4)
		clear(ADMUX, MUX2);
		set(ADMUX, MUX1);
		set(ADMUX, MUX0);
	}
	if (ADCpinNumber1 == 12){
		set(ADCSRB, MUX5);		//ADC12 (pin B5)
		set(ADMUX, MUX2);
		clear(ADMUX, MUX1);
		clear(ADMUX, MUX0);
	}
	if (ADCpinNumber1 == 13){
		set(ADCSRB, MUX5);		//ADC13 (pin B6)
		set(ADMUX, MUX2);
		clear(ADMUX, MUX1);
		set(ADMUX, MUX0);
	}
	
	set(ADCSRA, ADEN);	//enable ADC subsystem
	set(ADCSRA, ADSC);	//begin conversion
	
	while(bit_is_clear(ADCSRA, ADIF)){
		//wait for conversion to finish
	}
	if (bit_is_set(ADCSRA, ADIF)){
// 		m_usb_tx_uint(ADC);
// 		m_usb_tx_string("\n");
		set(ADCSRA, ADIF);	//reset flag
		clear(ADCSRA, ADEN);	//disable ADC system so other analogReads can be performed
	}
	
	return ADC;			//return ADC value
	
}


int main(void)
{
	teensy_clockdivide(0);	//set system clock to 16 MHz
	
	//________Timer/counter________
		
	clear(TCCR1B, CS12);
	clear(TCCR1B, CS11);
	set(TCCR1B, CS10);	//turn on clock source for timer 1 16MHz
		
	set(TCCR1A, WGM10);	//timer 1 mode 7 PWM, up to OCR1A, reset at rollover (10-bit = 1023)
	set(TCCR1A, WGM11);
	set(TCCR1B, WGM12);
	clear(TCCR1B, WGM13);
		
	clear(TCCR1A, COM1B0);	//clear B6 at OCR1B, set at rollover (1023)
	set(TCCR1A, COM1B1);
	
// 	m_usb_init();	//initialize the USB subsystem
// 	while(!m_usb_isconnected());	//wait for connection
	
	analogSet(0);	//initialize pin F0 as analog input
	analogSet(1);	//initialize pin F1 as analog input
	analogSet(4);	//initialize pin F4 as analog input
	analogSet(5);	//initialize pin F5 as analog input
	analogSet(6);	//initialize pin F6 as analog input

	//sets output pins for stepper motors
	set(DDRD, STEPpin1);
	set(DDRD, DIRpin1);	
	set(DDRD, STEPpin2);	
	set(DDRD, DIRpin2);
	set(DDRC, STEPpin3);
	set(DDRC, DIRpin3);
	
	//sets output pins for DC motor
	set(DDRB, 4);
	set(DDRB, 5);
	set(DDRB, 6);
	
	clear(PORTB, 5);	//start with motors off
	clear(PORTB, 6);
	OCR1B = 800;		//~80% duty cycle
	
for(;;){
	
	val1 = analogRead(0);	//read analog value from kpot on pin F0	(base pot)
	mappedval1 = (val1 - 0) * (600 - 0) / (1023 - 0) + 0;	//map 0-1023 to 0-600 for easier control
	steps1 = mappedval1 - prev1;	//set number of steps to equal change in sensor val
	
	val2 = analogRead(1);	//read analog val on F1	(joint1 pot)
	mappedval2 = (val2 - 0) * (100 - 0) / (1023 - 0) + 0;
	steps2 = mappedval2 - prev2;
	
	val3 = analogRead(4);	//read analog val on F4	(joint2 pot)
	mappedval3 = (val3 - 0) * (50 - 0) / (1023 - 0) + 0;
	steps3 = mappedval3 - prev3;
	
	val4 = analogRead(5);	//read analog val on F5	(gripper control pot)
	mappedval4 = (val4 - 0) * (50 - 0) / (1023 - 0) + 0;
	
	val5 = analogRead(6);	//read analog val on F6 (gripper pot)
	mappedval5 = (val5 - 0) * (50 - 0) / (1023 - 0) + 0;
	
	//feedback for DC motor
	if (mappedval4 == mappedval5){
		clear(PORTB, 5);	//fast motor stop
		clear(PORTB, 4);
	}
	else if (mappedval5 < mappedval4){
		set(PORTB, 5);		//rotate CW
		clear(PORTB, 4);
	}
	else if (mappedval5 > mappedval4){
		clear(PORTB, 5);	//rotate CCW
		set(PORTB, 4);		
	}
	
	//set the direction of the stepper motors
	if (steps1 < 0){		
		set(PORTD, DIRpin1);
	}
	else if (steps1 > 0){
		clear(PORTD, DIRpin1);
	}
	if (steps2 < 0){
		set(PORTD, DIRpin2);
	}
	else if (steps2 > 0){
		clear(PORTD, DIRpin2);
	}
	if (steps3 < 0){
		set(PORTC, DIRpin3);
	}
	else if (steps3 > 0){
		clear(PORTC, DIRpin3);
	}
	
 	//set steps to always be positive
	steps1 = abs(steps1);		
 	steps2 = abs(steps2);
	steps3 = abs(steps3);
	
	//move the stepper motors "x" steps, I initially used events and services
	//to do this but my system was hyperesponsive and seemed alot more noisy.
	//the reason I chose a for loop is because my program is running fast enough
	//that the sensor value is only ever 1 or 2, so the number of loops (steps to take)
	//in the for loop will be 1 or 2
	for (i = 0; i < steps1; i++){			
		set(PORTD, STEPpin1);
		_delay_us(motorSpeed);		//delay so the motor has time to take the step
		clear(PORTD, STEPpin1);		//reset step pin, so that the next high pulse will be a step
		_delay_us(motorSpeed);
	}
	for (j = 0; j < steps2; j++){
		set(PORTD, STEPpin2);
		_delay_us(motorSpeed);
		clear(PORTD, STEPpin2);
		_delay_us(motorSpeed);
	}
	for (k = 0; k < steps3; k++){
		set(PORTC, STEPpin3);
		_delay_us(motorSpeed);
		clear(PORTC, STEPpin3);
		_delay_us(motorSpeed);
	}
	
	//set previous val so next loop will register CHANGE in sensor val
	prev1 = mappedval1;			
 	prev2 = mappedval2;
 	prev3 = mappedval3;
	
// 	m_usb_tx_long(steps1);		//debug print statements
// 	m_usb_tx_string("\t"); 
// 	m_usb_tx_long(steps2);
// 	m_usb_tx_string("\t");
// 	m_usb_tx_long(mappedval4);
// 	m_usb_tx_string("\t");
// 	m_usb_tx_long(mappedval5);
// 	m_usb_tx_string("\n");
// 	teensy_wait(10);
	
}

    return 0;   /* never reached */
}