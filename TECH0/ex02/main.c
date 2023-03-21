#include <avr/io.h>
#include <avr/eeprom.h>

# define EEPROM_SIZE 1024
# define BUFFER_SIZE 126
# define WRITE_COMMAND "write"
# define DUMP_COMMAND "dump"

uint8_t write_command = 0;
char buffer[128];
uint8_t index = 0;

void uart_init(uint8_t baud)
{
    UBRR0H = (baud >> 8); // set baud rate .see Code 20.5 & Table 20-7
    UBRR0L = baud;
    
    TIMSK1 |= ( 1 << OCIE1A);

    //set frame format 15200 8N1
    UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);

    //enable the Transmitter
    UCSR0B |= ( 1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0);
    SREG = ( 1 << SREG_I); //enable global interrupt flag 20.5 
}

void uart_tx(char c)
{
    while (!(UCSR0A & (1 << UDRE0))) ;
    UDR0 = c;
}

char uart_rx(void) //read
{
    while (!(UCSR0A & (1 << RXC0)));    // Wait for data to be received
    return UDR0;    // Get and return received data from buffer
}

void uart_printstr(const char *str)
{
    uint8_t i = 0;

    while (str[i])
    {
        uart_tx(str[i]);
        i++;
    }
}

void EEPROM_write(unsigned int uiAddress, unsigned char ucData)
{
    
    while (EECR & (1 << EEPE)) ;

    EEAR = uiAddress;
    EEDR = ucData;

    EECR |= ( 1 << EEMPE);

    EECR |= ( 1 << EEPE);

}

unsigned char EEPROM_read(unsigned int uiAddress)
{

    while (EECR & (1 << EEPE)) ;

    EEAR = uiAddress;

    EECR |= ( 1 << EERE );

    return EEDR;
}

void EEPROM_dump()
{
    for (int i = 0; i < EEPROM_SIZE; i++ )
    {
        uart_tx(EEPROM_read(0x0 + i));
    }
}

int ft_strncmp(char *s1, char *s2, int n)
{
	size_t		i;

	i = 0;
	while (i < n)
	{
		if ((s1[i] - s2[i]) != 0 || s1[i] == '\0' || s2[i] == '\0')
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

void clear_buffer() {

    for (int i = 0; i < BUFFER_SIZE; i++)
        buffer[i] = '\0';
}

__attribute__((signal)) void USART_RX_vect()
{
    char c = UDR0;

    if (index > 0 && c == 127)
    {
        buffer[index] = '\0';
        index--;
    }
    else if (c == 13)
    {
        if (ft_strncmp(buffer, DUMP_COMMAND, 4) == 0)
        {
            EEPROM_dump();    
        } else if (write_command)
        {
            int i = 0;
            while (buffer[i] != '\0') {
                EEPROM_write(0x1 + i, buffer[i]);
                i++;
            }
            write_command = 0;
        }
        clear_buffer();
        index = 0;
    }
    else if (c == ' ')
    {    
        if (ft_strncmp(buffer, WRITE_COMMAND, 5) == 0)
        {
            write_command = 1;
        }
        clear_buffer();
        index = 0;
    }
    else if ( index > -1 && index < 126 )
    {
        buffer[index] = c;
        index++;
    }
}

void main(void)
{
    uart_init(8); // 115200 baud rate, table 20-7 p.199
	
    DDRB = 0xff; //set all pins to output mode
    DDRD = 0; //set PD2 as button
    
    // INTERRUPT SETUP
    UCSR0B |= (1 << RXCIE0); // Receive complete interrupt flag
    SREG |= (1 << SREG_I);   // Enable global interrupt

    if (EEPROM_read(0x0) == 0) { //will read 
        EEPROM_write(0x0, 1); //
        EEPROM_write(0x1, 0);
    }


	uint8_t last_state = (PIND >> PD2) & 1;
	for (;;)
	{
		uint8_t new_state = (PIND >> PD2) & 1;
		if (last_state != new_state)
		{
			if (new_state == 0) {
                EEPROM_dump();
                //On dump le contenu de EEPROM sur la connexion UART
            }
			for (uint32_t delay = 0; delay < 50000; delay++) ;
			last_state = new_state;
		}
	}	
}
