#include <avr/io.h>

void main(void)
{
	DDRB |= ( 1 << PB3 ); //set PB3 to output mode
	
	for (;;)
	{
		for (uint32_t delay = 0; delay < 160000; delay++) ; //approximation
		
		PORTB ^= ( 1 << PB3 ); //toggle PB3
	} 
}
