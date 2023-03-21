RM = rm 
RF = -rf
CC= avr-gcc

all: main.c
    hex
    flash

hex: main.c
	$(CC) -g -Os -D F_CPU=16000000 -mmcu=atmega328p -c main.c
	$(CC) -g -mmcu=atmega328p -o main.bin main.o
	avr-objcopy -O main.bin main.hex

flash: hex
    avrdude -p m328p -i main.hex

clean:
    $(RM) $(RF) main.hex
    $(RM) $(RF) main.bin
