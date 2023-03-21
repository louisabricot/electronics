#include <avr/io.h>

void main(void)
{
	DDRB |= ( 1 << PB3 ); //set PB3 to output mode
	
	DDRD |= (0 << PD3); //set PD3 to input mode

	for (;;)
	{
		if (((PIND >> PD3) & 1) == 0)
		{
			PORTB ^= (1 << PD3);

			while (((PIND >> PD3) & 1) == 0) ;

			for (uint32_t delay = 0; delay < 16000; delay++) ;
		}
	} 
}
