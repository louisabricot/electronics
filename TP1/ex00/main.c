#include <avr/io.h>

//using fast-PWM
void main(void)
{
	DDRB |= ( 1 << PB1) ; //set PB1 to output mode see 16.8
	
	TCCR1A |= (1 << COM1A1); //clear OC1A 16.11.1
	
	TCCR1A |= (1 << WGM11 ); //set Fast PWM mode 14 TOP=ICR1 16.11.1
	TCCR1B |= ( 1 << WGM12) | ( 1 << WGM13); //set Fast PWM mode 14 TOP=ICR1 16.11.1


	ICR1 = F_CPU / 256 - 1; //set TOP. comparison happens at the next cycle 16.9.3


	TCCR1B |= (1 << CS12); //256. set 0-2 bits of TCCR1B to prescalar. see: 16.11.2 & Table 16-5.
	

	OCR1A = ICR1 / 2; //rapport cyclique de 50%

	for (;;) ;
}
