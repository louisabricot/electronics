#include <avr/io.h>

uint8_t variation = 1;
uint8_t order = 1;

void main(void)
{
    //Timer 1 & LED PB1

    DDRB |= ( 1 << PB1) ; //set PB1 to output mode

    TCCR1A |= (1 << COM1A1); //clear OC1A 16.11.1
	
	TCCR1A |= (1 << WGM11 ); //set Fast PWM mode 14 TOP=ICR1 16.11.1
	TCCR1B |= ( 1 << WGM12) | ( 1 << WGM13); //set Fast PWM mode 14 TOP=ICR1 16.11.1


	ICR1 = 255 - 1; //set TOP. comparison happens at the next cycle 16.9.3


	TCCR1B |= (1 << CS12); //256. set 0-2 bits of TCCR1B to prescalar. see: 16.11.2 & Table 16-5.
	

	OCR1A = variation * (ICR1 / 100); //variation of cyclical report


    //Timer 0 to periodically vary the cyclical report

    TCCR0A |= ( 1 << WGM01 ); //set CTC mode with TOP=OCRA table 15-8

    TCCR0B |= ( 1 << CS02) | ( 1 << CS00 ); //set prescalar to 1024 table 15-9

    OCR0A = 85;

    TIMSK0 |= ( 1 << OCIE0A ); //Output Compare Match A Interrupt Enable 15.9.6

    SREG |= ( 1 << SREG_I); //


	for (;;) ;
}

__attribute__((signal)) void TIMER0_COMPA_vect()
{
    if (variation == 0 || variation == 100)
        order *= -1;
    variation += order;
    OCR1A = variation * (ICR1 / 100);
}