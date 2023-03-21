#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>

# define RED 0
# define GREEN 1
# define BLUE 2
# define DEBOUNCE_DELAY 50000
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

void adc_init(void)
{
        //ADC Multiplexer Selection Register.
    //  REFS0: Voltage Ref AVCC w external capacitor at AREF pin (24.9.1)
    //  MUX2: Select ADC4 input channel see Table 24-4 
    //  ADLAR: left adjust ACDL bit in order to get a 8 bit resolution 24.9.3.2

    ADMUX |= ( 1 << REFS0 ) | ( 1 << MUX2 ) | ( 1 << ADLAR );

    //ADCSRA -> ADC Control and Status Register A
    ADCSRA |= (1 << ADEN ) ;

}

uint8_t adc_read(void)
{
    //ADC Start Conversion
    ADCSRA |= ( 1 << ADSC );

    while ( ADCSRA & (1 << ADSC ));

    return ADCH; 
}


uint16_t adcVal = 0;

void main(void)
{
    char strVal[8] = {0};

    uart_init(8);
    adc_init();
    // LEDS
            //  R           G             B
    DDRD = (1 << PD6) | (1 << PD5) | (1 << PD3);
    DDRB = (1 << PB3);

    // TIMER 0
    TCCR0A = (1 << WGM00) | (1 << WGM01); //fast PWM mode

    // Set OC0B & OC0A on Compare Match, clear OC0B & OC0A at BOTTOM, (inverting mode).
    TCCR0A |= (1 << COM0B1) | (1 << COM0B0) | (1 << COM0A1) | (1 << COM0A0);

    // Prescaler 256
    TCCR0B |= (1 << CS02);

    OCR0A = 0xff;
    OCR0B = 0xff;

    // TIMER 2
    TCCR2A |= (1 << WGM21) | (1 << WGM20) | (1 << COM2B1) | (1 << COM2B0);
    TCCR2B |= (1 << CS22);
    OCR2B = 0xff;

    SREG = (1 << 7);

    // BUTTON
    unsigned char last_state = PIND & (1 << PD2);

    // uint8_t colors[3] = { RED, GREEN, BLUE};
    uint8_t color = 0;
    while (1)
    {
        adcVal = adc_read();
        if (color == RED)
            OCR0A = adcVal; //red
        else if (color == GREEN)
            OCR0B = adcVal; //green
        else if (color == BLUE)
            OCR2B = adcVal; //blue
        unsigned char new_state = PIND & (1 << PD2);
        if (last_state != new_state)
        {
            if (new_state == 0)
            {
                if (color < 2)
                    color = color + 1;
                else
                    color = 0;
            }
            for (long i = 0; i < DEBOUNCE_DELAY; i++)
            {}
            last_state = new_state;
        }
        _delay_ms(100);
    }
}