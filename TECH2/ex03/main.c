#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>

void uart_init(uint8_t baud) {
    UBRR0H = (baud >> 8); // set baud rate .see Code 20.5 & Table 20-7
    UBRR0L = baud;
    //set frame format 115200 8N1
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

void adc_init(void)
{
        //ADC Multiplexer Selection Register.
    //  REFS0 & REFS1: Internal 1.1V (24.9.1)
    //  MUX2: Select temperature input channel see Table 24-4 

    ADMUX |= ( 1 << REFS0 ) | ( 1 << REFS1) | ( 1 << MUX3 );

    //ADCSRA -> ADC Control and Status Register A
    ADCSRA |= (1 << ADEN ) ;

}

uint16_t adc_read(void)
{
    //ADC Start Conversion
    ADCSRA |= ( 1 << ADSC );

    while ( ADCSRA & (1 << ADSC ));

    return ADCW; 
}


uint16_t adcVal = 0;
signed int toCelsius = 0;
void main(void)
{
    char strVal[8] = {0};

    uart_init(8);
    adc_init();

    while (1)
    {
        adcVal = adc_read();
        toCelsius = adcVal * 0.08; //
        itoa(toCelsius, strVal, 10);
        uart_printstr(strVal);
        uart_printstr("\n\r");
        _delay_ms(100);
    }
}