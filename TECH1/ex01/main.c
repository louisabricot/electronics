#include <avr/io.h>

# define RED 0b1000000
# define GREEN 0b0100000
# define BLUE 0b0001000
# define YELLOW 0b1100000
# define CYAN 0b0101000
# define MAGENTA 0b1001000
# define WHITE 0b1101000

uint8_t i = 0;
uint8_t order[7] = {
    RED,
    GREEN,
    BLUE,
    YELLOW,
    CYAN,
    MAGENTA,
    WHITE
};

void    main(void) {

    DDRD |= ( 1 << PD6) | ( 1  << PD5) | ( 1 << PD3);

	uint8_t last_state = (PIND >> PD2) & 1;
	for (;;)
	{
		uint8_t new_state = (PIND >> PD2) & 1;
		if (last_state != new_state)
		{
			if (new_state == 0) {
				PORTD = ~ (order[i]);
                if (i < 6)
                    i++;
                else 
                    i = 0;
            }
			for (uint32_t delay = 0; delay < 100000; delay++) ;
			last_state = new_state;
		}
	}	
}