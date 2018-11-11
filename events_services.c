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

long val1, val2, val3, val4;
long mappedval1, mappedval2, mappedval3, mappedval4;
long steps1, steps2, steps3, steps4;
long prev1, prev2, prev3, prev4;
long i,j,k;

void analogSet(int ADCpinNumber){
	
	set(ADMUX, REFS0);	//setting reference voltage for ADC to Vcc
	clear(ADMUX, REFS1);
		
	set(ADCSRA, ADPS2);	//set ADC prescaler to 125 kHz based on system clock
	set(ADCSRA, ADPS1);
	set(ADCSRA, ADPS0);
	
	if (ADCpinNumber >= 0 && ADCpinNumber<=7){
		teensy_disableJTAG();
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
	
	m_usb_init();	//initialize the USB subsystem
	while(!m_usb_isconnected());	//wait for connection
	
	analogSet(0);	//initialize pin F0 as analog input
	analogSet(1);	//initialize pin F1 as analog input
	analogSet(4);	//initialize pin F4 as analog input
	analogSet(5);	//initialize pin F5 as analog input

	set(DDRD, 0);
	set(DDRD, 1);	//sets D1 as output
	set(DDRD, 2);	//sets D2 as output
	set(DDRD, 3);
	set(DDRC, 6);
	set(DDRC, 7);
	
for(;;){
	
	val1 = analogRead(0);	//read analog value from kpot on pin F0
	mappedval1 = (val1 - 0) * (400 - 0) / (1023 - 0) + 0;	//map 0-1023 to 0-200 (steps of motor)
	steps1 = mappedval1 - prev1;	//set number of steps to equal change in sensor val
	
	val2 = analogRead(1);	//read analog val on F1
	mappedval2 = (val2 - 0) * (100 - 0) / (1023 - 0) + 0;
	steps2 = mappedval2 - prev2;
	
	val3 = analogRead(4);	//read analog val on F4
	mappedval3 = (val3 - 0) * (50 - 0) / (1023 - 0) + 0;
	steps3 = mappedval3 - prev3;
	
	val4 = analogRead(5);
	mappedval4 = (val4 - 0) * (50 - 0) / (1023 - 0) + 0;
	steps4 = mappedval4 - prev4;
	
	if (steps1 < 0){			//if statements to determine direction of the motor
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
	
 	steps1 = abs(steps1);		//set calculated steps number to always be positive
 	steps2 = abs(steps2);
	steps3 = abs(steps3);
	
	if (i++ < steps1){			//have the motor take one step each loop
		set(PORTD, STEPpin1);
		_delay_us(motorSpeed);
		clear(PORTD, STEPpin1);		//reset step pin, so that the next high pulse will be a step
	}
	else i=0;
	if (j++ < steps2){
		set(PORTD, STEPpin2);
		_delay_us(motorSpeed);
		clear(PORTD, STEPpin2);
	}
	else j=0;
	if (k++ < steps3){
		set(PORTC, STEPpin3);
		_delay_us(motorSpeed);
		clear(PORTC, STEPpin3);
		
	}
	else k=0;
	
	_delay_us(motorSpeed);		//delay will dictate motor speed
	
	prev1 = mappedval1;			//set previous val so next loop will register CHANGE in sensor val
 	prev2 = mappedval2;
 	prev3 = mappedval3;
	prev4 = mappedval4;
	
// 	m_usb_tx_long(steps1);		//print value
// 	m_usb_tx_string("\t"); 
// 	m_usb_tx_long(steps2);
// 	m_usb_tx_string("\t");
// 	m_usb_tx_long(steps3);
// 	m_usb_tx_string("\t");
// 	m_usb_tx_long(steps4);
// 	m_usb_tx_string("\n");
// 	teensy_wait(500);
	
}

    return 0;   /* never reached */
}