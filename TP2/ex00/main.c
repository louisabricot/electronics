#include <avr/io.h>

void    main(void) {

    DDRB |= ( 1 << PB1) ; //set PB1 to output mode see 16.8

    EICRA |= (1 << ISC11 ); //the falling edge of INT1 generates an interrupt request 13.2

    EIMSK |= ( 1 << INT1 ); //enable external pin interrupt 13.2.2

    SREG |= (1 << SREG_I); //enable external pin interrupt 13.2.2 + https://web.ics.purdue.edu/~jricha14/Interrupts/SREG.htm to find out which bit was l-bit

    for (;;) ;
}

__attribute__((signal)) void INT1_vect()
{
    PORTB ^= ( 1 << PB1 );
}