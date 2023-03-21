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

void uart_printstr(const char *str) {
    uint8_t i = 0;
    while (str[i]){
        uart_tx(str[i]);
        i++;
    }
}

__attribute__((signal)) void TIMER1_COMPA_vect()
{
    uart_printstr("Hello World!\n\r");

}

void main(void) {

    uart_init(8);
    TCCR1A |= (1 << COM1A1);
    TCCR1B = (1 << WGM12); //CTC mode
	TCCR1B |= (1 << CS12) | (1 << CS10); //1024. set 0-2 bits of TCCR1B to prescalar. see: 16.11.2 & Table 16-5.
    OCR1A = 31249; //f_cpu / 1024 * 2 -1
    TIMSK1 |= ( 1 << OCIE1A);

    for (;;) ;
}