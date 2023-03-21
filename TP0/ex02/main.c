#include <avr/io.h>

void main(void)
{
	DDRB |= ( 1 << PB3 ); //set PB3 to output mode
	PORTB |= ( 1 << PB3 ); //set PB3 to high mode
	for (;;) ;
}
