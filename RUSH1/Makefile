RM = rm 
RF = -rf
CC= avr-gcc
TARGET = main

F_CPU=16000000
MMCU=atmega328p

all: hex flash

hex:
	$(CC) -g -Os -D F_CPU=$(F_CPU) -mmcu=$(MMCU) -c $(TARGET).c
	$(CC) -g -mmcu=$(MMCU) -o $(TARGET).bin $(TARGET).o
	avr-objcopy -j .text -j .data -O ihex $(TARGET).bin $(TARGET).hex

flash:
	avrdude -p $(MMCU) -c arduino -P /dev/ttyUSB0 -U flash:w:$(TARGET).hex:i
	# avrdude -p $(MMCU) -c arduino -P /dev/ttyUSB1 -U flash:w:$(TARGET).hex:i


screen:
	screen -S bob /dev/ttyUSB0 115200

quit:
	screen -X -S bob kill
clean:
	$(RM) $(RF) $(TARGET).o
	$(RM) $(RF) $(TARGET).hex
	$(RM) $(RF) $(TARGET).bin
