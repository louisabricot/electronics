#include <avr/io.h>

void uart_init(uint8_t baud) {
    UBRR0H = (baud >> 8); // set baud rate .see Code 20.5 & Table 20-7
    UBRR0L = baud;
    //set frame format 15200 8N1
    UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);

    //enable the Transmitter
    UCSR0B |= ( 1 << TXEN0);
    SREG = ( 1 << SREG_I); //enable global interrupt flag 20.5 
}

void uart_tx(char c) {
    while ( ! (UCSR0A & (1 << UDRE0))) ;
    UDR0 = c;
}

void main(void) {

    uart_init(8);

    uart_tx('Z');
}