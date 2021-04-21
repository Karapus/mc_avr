MCU = atmega8535
CPU_F = 1200000
CC = avr-gcc
OBJCOPY = avr-objcopy
CFLAGS = -Wall -g -Os -mmcu=${MCU} -DCPU_F=${CPU_F}
TARGET = main
SRCS = main.c

all:
	${CC} ${CFLAGS} -o ${TARGET}.bin ${SRCS}
	${OBJCOPY} -j .text -j .data -O ihex ${TARGET}.bin ${TARGET}.hex

flash:
	@avrdude -p ${MCU} -c usbasp -U flash:w:${TARGET}.hex:i -F -P usb -B12 -v

clean:
	rm -f *.bin *.hex
