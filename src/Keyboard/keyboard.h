//
// Created by Student on 06.09.2023.
//

#ifndef ALEXZORN_CHIP8_KEYBOARD_H
#define ALEXZORN_CHIP8_KEYBOARD_H

void initKeyboard( );

void pressKey( int keyCode );
void releaseKey( int keyCode );

extern _Bool keyboardState[16];

#endif //ALEXZORN_CHIP8_KEYBOARD_H



