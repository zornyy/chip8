#include <SDL2/SDL.h>

#include "src/Display/display.h"
#include "src/Keyboard/keyboard.h"
#include "src/cpu/cpu.h"


int initProgram( char *rom ) {
    initKeyboard( );
    initDisplay( 30 );
    initCPU( );

    loadSpritesIntoMemory( );
    return loadRom( rom );
}


/*
 *  This function calls a new window from the display.h library.
 *  The main loop of the program runs in the showWindow function.
 */
int main( int argc, char *argv[] ) {
    SDL_Log("%s", argv[1]);
    // Initialize every dependencies
    if ( initProgram( argv[1] ) == 1 ) {
        return 1;
    }

    // Start the main Loop and show the window
    showWindow( );
    return 0;
}
