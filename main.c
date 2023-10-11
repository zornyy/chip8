#include "display.h"
#include "keyboard.h"
#include "cpu.h"


int initProgram( ) {
    initKeyboard( );
    initDisplay( 20 );
    initCPU( );

    loadSpritesIntoMemory( );
    return loadRom();
}


/*
 *  This function calls a new window from the display.h library.
 *  The main loop of the program runs in the showWindow function.
 */
int main() {
    // Initialize every dependencies
    if ( initProgram() == 1 ) {
        return 1;
    }

    // Start the main Loop and show the window
    showWindow( );
    return 0;
}
