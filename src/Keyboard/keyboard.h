//
// Created by Student on 06.09.2023.
//

#ifndef ALEXZORN_CHIP8_KEYBOARD_H
#define ALEXZORN_CHIP8_KEYBOARD_H

/*
 * Function initializes the keyboard according to the layout
 */
void initKeyboard( );

/*
 * Param: keyCode ( int ) The keyCode of the key that has been pressed
 * Action: Updates the keys State according to the new key Pressed
 */
void pressKey( int keyCode );

/*
 * Param: keyCode ( int ) The keyCode of the key that has been released
 * Action: Updates the keys State according to the new key realease
 */
void releaseKey( int keyCode );

extern _Bool keyboardState[16];

#endif //ALEXZORN_CHIP8_KEYBOARD_H



