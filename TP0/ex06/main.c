#include <avr/io.h>

void main(void)
{
	DDRB = 0xff; //set all pins to output mode

	DDRD = 0; //set all pins to input mode

	uint8_t n = 1;

	for (;;)
	{
		if (((PIND >> PD3) & 1) == 0)
		{
			PORTB = n++;

			while (((PIND >> PD3) & 1) == 0) ;

			for (uint32_t delay = 0; delay < 16000; delay++) ;
		}
	}	
}
