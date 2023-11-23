#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#include "../Stack/stack.h"
#include "../cpu/cpu.h"
#include "../Display/display.h"
#include "../debug/debug.h"
#include "../Keyboard/keyboard.h"



// SDL Utility Variables  
SDL_Window *window;
SDL_Renderer *renderer;
// SDL_Renderer *debugRenderer;

SDL_Rect drawingRect;

// Framerate variables
const int FPS = 60;
const int frameDelay = 1000 / FPS;
Uint64 frameStart;
Uint64 frameTime;


// Screen matrix
int screenState[64][32];


// Display variables
int width, height;
int pxSize;

/*
 * Function sets every pixels of the
 * screen State to 0 ( OFF )
 */
void clearDisplay( ) {
  // empty screen matrix
  for ( int x = 0; x < 64; x++ ) {
    for ( int y = 0; y < 32; y++ ) {
      screenState[x][y] = 0;
    }
  }
}

/*
 * Function renders <drawingRect> at it's 
 * current position in the frame buffer
 */ 
void drawRect( ) {
    SDL_SetRenderDrawColor( renderer, 222, 27, 199, 255); // Pink
    SDL_RenderFillRect( renderer, &drawingRect );
}


/*
 * Function loops through every element of the
 * screen matrix which represents the state of
 * every pixels and renders that pixel to the 
 * frame buffer
 */
void drawEmulatorScreen( ) {
  for ( int x = 0; x < 64; x++ ) { // rows
    for ( int y = 0; y < 32; y++ ) { // cols
      if ( screenState[x][y] ) {

        // Set the coordinates to draw the pixel
        drawingRect.x = x * pxSize;
        drawingRect.y = y * pxSize;
        
        // Render rect
        drawRect( );
      }
    }
  }
}


/*
* Function calculates and applies the
* necessary delay to fix the framerate 
* of the program
*/
void fixFramerate(  ) {
    frameTime = SDL_GetTicks() - frameStart;

    if ( frameDelay > frameTime ) {
        SDL_Delay( frameDelay - frameTime );
    }
}

int setPixel( int x, int y ) {

    // wrap the pixel on the other side of the screen if out display

    if ( x > 63 ) {
        x -= 64;
    } else if ( x < 0 ) {
        x += 64;
    }

    if ( y > 31 ) {
        y -= 32;
    } else if ( y < 0 ) {
        y += 32;
    }

    // update the screenState
    screenState[x][y] ^= 1;

    // Draw or erase the pixel
    if ( screenState[x][y] == 1 ) {
      return 0;
    }
    return 1;
}


void initDisplay( int pixelSize ) {
    SDL_Init( SDL_INIT_EVERYTHING );
    if( TTF_Init( ) == -1 ) {
      SDL_Log("Could not initialize TTF");
    } else {
      SDL_Log("TTF initialized");
  }
    pxSize = pixelSize;
    drawingRect.h = pixelSize - 1, drawingRect.w = pixelSize - 1;
    // Adding proportional size on each side so that we have space to display the logs
    width = pixelSize * 64 + pixelSize * 30, height = pixelSize * 32 + pixelSize * 25;
    

    // init debug display
    initDebug( pixelSize, 25, 30 );

    clearDisplay( );  

    SDL_Log("Display initialized with 64x32px\n");
}

void programCycle( ) {
  cpuCycle( );

}

/*
* Function initializes and starts the main program loop
*/
int showWindow( ) {
    window = SDL_CreateWindow( 
    "AlexZorn - Chip-8 Emulator", 
    SDL_WINDOWPOS_UNDEFINED, 
    SDL_WINDOWPOS_UNDEFINED, 
    width, height, 
    SDL_WINDOW_ALLOW_HIGHDPI );
    
    if ( NULL == window ) {
      SDL_Log("Could not create window");
      return 1;
    }

    renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
    if ( NULL == renderer ) {
      SDL_Log("Could not create renderer");
      return 1;
    }


    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
    SDL_RenderClear( renderer );
    SDL_RenderPresent( renderer );

    SDL_Event windowEvent;

    bool isRunning = true;

    SDL_Log("Starting Main loop fixed at %dHz\n", FPS);
    // Main Program LOOP
    while ( isRunning )
    {
        frameStart = SDL_GetTicks();
        while ( SDL_PollEvent( &windowEvent ) )
        {
            // Check for SDL Events
            if ( SDL_QUIT == windowEvent.type )
            {
                isRunning = false;
            } else if ( SDL_KEYDOWN == windowEvent.type ) {
                pressKey( windowEvent.key.keysym.sym );
            } else if ( SDL_KEYUP == windowEvent.type ) {
                releaseKey( windowEvent.key.keysym.sym );
            }
        }

        // Content of the program cycle
        programCycle( );
        
        // Clear full screen
        SDL_SetRenderDrawColor( renderer, 0, 0, 0, SDL_ALPHA_OPAQUE );
        SDL_RenderClear( renderer ); 
        
        // Draw emulator screen
        drawEmulatorScreen( );

        // display debug infos
        displayDebugInfo( renderer );
        drawPerformances( renderer, frameTime, FPS ); 

        // Display the buffer on the screen
        SDL_RenderPresent( renderer );

        // Fix the framerate
        fixFramerate( );
    }

    SDL_DestroyWindow( window );
    SDL_Quit( );

    return EXIT_SUCCESS;
}

