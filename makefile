CC = gcc
CFLAGS = -I/usr/include/SDL2
LDFLAGS = -lSDL2 -lSDL2_ttf -ldl

chip8: main.o cpu.o display.o keyboard.o stack.o debug.o
	$(CC) main.o cpu.o display.o keyboard.o stack.o debug.o -o chip8 $(LDFLAGS)

main.o: main.c 
	$(CC) -c main.c $(CFLAGS)

cpu.o: src/cpu/cpu.c src/cpu/cpu.h 
	$(CC) -c src/cpu/cpu.c $(CFLAGS)

display.o: src/Display/display.c src/Display/display.h 
	$(CC) -c src/Display/display.c $(CFLAGS)

keyboard.o: src/Keyboard/keyboard.c src/Keyboard/keyboard.h 
	$(CC) -c src/Keyboard/keyboard.c $(CFLAGS)

stack.o: src/Stack/stack.c src/Stack/stack.h 
	$(CC) -c src/Stack/stack.c $(CFLAGS)

debug.o: src/debug/debug.c src/debug/debug.h 
	$(CC) -c src/debug/debug.c $(CFLAGS)

clean:
	rm -f *.o chip8
