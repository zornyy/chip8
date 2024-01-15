#include <SDL2/SDL.h>

#include "src/Display/display.h"
#include "src/Keyboard/keyboard.h"
#include "src/cpu/cpu.h"

int sizeFactor = 25;

int keyboardLayout[16] = {
  SDLK_1,
  SDLK_2,
  SDLK_3,
  SDLK_4,
  SDLK_q,
  SDLK_w,
  SDLK_e,
  SDLK_r,
  SDLK_a,
  SDLK_s,
  SDLK_d,
  SDLK_f,
  SDLK_y,
  SDLK_x,
  SDLK_c,
  SDLK_v
};


int initProgram( char *rom ) {
    
    initKeyboard( keyboardLayout );
    initDisplay( sizeFactor );

    initCPU( );

    loadSpritesIntoMemory( );
    return loadRom( rom );
}


/*
 *  This function calls a new window from the display.h library.
 *  The main loop of the program runs in the showWindow function.
 */
int main( int argc, char *argv[] ) {
    SDL_Log( "Booting on: %s", argv[1] );
    // Initialize every dependencies
    
    if ( argv[2] ) {
      sizeFactor = atoi(argv[2]);     
    }

    SDL_Log( "Pixel size at display: %d", sizeFactor );
    if ( initProgram( argv[1] ) == 1 ) {
      return 1;
    }

    // Start the main Loop and show the window
    showWindow( );
    return 0;
}
