CC=avr-gcc
CFLAGS=-g -Os -Wall -mcall-prologues -mmcu=at90usb162 -DF_CPU="8000000UL"

OBJ2HEX=avr-objcopy 
TARGET=xmas_led

program: $(TARGET).hex 
	scp xmas_led.hex robert@10.1.1.149:xmas_led

$(TARGET).hex: $(TARGET).obj 
	$(OBJ2HEX) -R .eeprom -O ihex $< $@

$(TARGET).obj: xmas_lib.o main.o fbuffer.o
	$(CC) $(CFLAGS) -o $@ -Wl,-Map,$(TARGET).map $^

xmas_lib.o: xmas_lib.c  xmas_lib.h
	$(CC) -c $(CFLAGS) -o xmas_lib.o xmas_lib.c

main.o: main.c
	$(CC) -c $(CFLAGS) -o main.o main.c

fbuffer.o: fbuffer.c fbuffer.h
	$(CC) -c $(CFLAGS) -o fbuffer.o fbuffer.c

clean:
	rm -f *.hex *.obj *.o *.map
