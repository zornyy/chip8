//
// Created by Student on 06.09.2023.
//

#ifndef ALEXZORN_CHIP8_DISPLAY_H
#define ALEXZORN_CHIP8_DISPLAY_H

/*
 * This function initializes the display of a window
 *  ( int pixelSize ) => Size of each pixel of the 32x64px display
 */
void initDisplay( int pixelSize );


/*
 * This function displays a new SDL window
 *  !! It should be used after int initDisplay( int w, int h )
 */
int showWindow( );

/*
 *  This function sets a pixel to be ON
 *  ( int x ) => position X of the pixel
 *  ( int y ) => position Y of the pixel
 */
void setPixel( int x, int y );

#endif //ALEXZORN_CHIP8_DISPLAY_H

