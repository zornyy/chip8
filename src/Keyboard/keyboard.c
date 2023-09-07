//
// Created by Student on 06.09.2023.
//

#include <stdbool.h>
#include <SDL.h>

#include "keyboard.h"

bool keyboardState[16];



void initKeyboard( ) {
    // Set keys to false
    for ( int i=0; i<16; i++ ) {
        keyboardState[i] = false;
    }
}

void pressKey( int keyCode ) {
    switch ( keyCode ) {
        case SDLK_1 :
            keyboardState[0] = true;
            break;
        case SDLK_2 :
            keyboardState[1] = true;
            break;
        case SDLK_3 :
            keyboardState[2] = true;
            break;
        case SDLK_4 :
            keyboardState[3] = true;
            break;
        case SDLK_q :
            keyboardState[4] = true;
            break;
        case SDLK_w :
            keyboardState[5] = true;
            break;
        case SDLK_e :
            keyboardState[6] = true;
            break;
        case SDLK_r :
            keyboardState[7] = true;
            break;
        case SDLK_a :
            keyboardState[8] = true;
            break;
        case SDLK_s :
            keyboardState[9] = true;
            break;
        case SDLK_d :
            keyboardState[10] = true;
            break;
        case SDLK_f :
            keyboardState[11] = true;
            break;
        case SDLK_y :
            keyboardState[12] = true;
            break;
        case SDLK_x :
            keyboardState[13] = true;
            break;
        case SDLK_c :
            keyboardState[14] = true;
            break;
        case SDLK_v :
            keyboardState[15] = true;
            break;
    }
}

void releaseKey( int keyCode ) {
    switch ( keyCode ) {
        case SDLK_1 :
            keyboardState[0] = false;
            break;
        case SDLK_2 :
            keyboardState[1] = false;
            break;
        case SDLK_3 :
            keyboardState[2] = false;
            break;
        case SDLK_4 :
            keyboardState[3] = false;
            break;
        case SDLK_q :
            keyboardState[4] = false;
            break;
        case SDLK_w :
            keyboardState[5] = false;
            break;
        case SDLK_e :
            keyboardState[6] = false;
            break;
        case SDLK_r :
            keyboardState[7] = false;
            break;
        case SDLK_a :
            keyboardState[8] = false;
            break;
        case SDLK_s :
            keyboardState[9] = false;
            break;
        case SDLK_d :
            keyboardState[10] = false;
            break;
        case SDLK_f :
            keyboardState[11] = false;
            break;
        case SDLK_y :
            keyboardState[12] = false;
            break;
        case SDLK_x :
            keyboardState[13] = false;
            break;
        case SDLK_c :
            keyboardState[14] = false;
            break;
        case SDLK_v :
            keyboardState[15] = false;
            break;
    }
}