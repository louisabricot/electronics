#include <avr/io.h>

void main(void)
{
	DDRB |= ( 1 << PB3 ); //set PB3 to output mode

	DDRD |= (0 << PD3); //set PD3 to input mode

	for (;;)
	{
		if (PIND & (1 << PD3 ))
			PORTB &= ~( 1 << PD3 );
		else
			PORTB |= ( 1 << PB3 );
	} 
}
