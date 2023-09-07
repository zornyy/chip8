#include "display.h"
#include "keyboard.h"



/*
 *  This function calls a new window from the display.h library.
 *  The main loop of the program runs in the showWindow function.
 */
int main() {
    // Initialize every dependencies
    initKeyboard();
    initDisplay( 10 );

    // Start the main Loop and show the window
    showWindow( );
    return 0;
}
