#include "display.h"
#include "keyboard.h"
#include "logs_lib.h"


/*
 *  This function calls a new window from the display.h library.
 *  The main loop of the program runs in the showWindow function.
 */
int main() {
    initDisplay( 10 );
    showWindow( );
    return 0;
}
