chip8: main.o cpu.o display.o keyboard.o stack.o
	gcc main.o cpu.o display.o keyboard.o stack.o -o chip8 -lSDL2

main.o: main.c 
	gcc -c main.c

cpu.o: main.c src/cpu/cpu.c src/cpu/cpu.h 
	gcc -c src/cpu/cpu.c -lSDL2

display.o: main.c src/Display/display.c src/Display/display.h 
	gcc -c src/Display/display.c -lSDL2

keyboard.o: main.c src/Keyboard/keyboard.c src/Keyboard/keyboard.h 
	gcc -c src/Keyboard/keyboard.c -lSDL2

stack.o: main.c src/Stack/stack.c src/Stack/stack.h 
	gcc -c src/Stack/stack.c -lSDL2
