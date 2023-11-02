CC = gcc
CFLAGS = -lSDL2

chip8: main.o cpu.o display.o keyboard.o stack.o
	$(CC) main.o cpu.o display.o keyboard.o stack.o -o chip8 $(CFLAGS)

main.o: main.c 
	$(CC) -c main.c

cpu.o: main.c src/cpu/cpu.c src/cpu/cpu.h 
	$(CC) -c src/cpu/cpu.c $(CFLAGS)

display.o: main.c src/Display/display.c src/Display/display.h 
	$(CC) -c src/Display/display.c $(CFLAGS)

keyboard.o: main.c src/Keyboard/keyboard.c src/Keyboard/keyboard.h 
	$(CC) -c src/Keyboard/keyboard.c $(CFLAGS)

stack.o: main.c src/Stack/stack.c src/Stack/stack.h 
	$(CC) -c src/Stack/stack.c $(CFLAGS)

clean:
	rm -f *.o chip8
