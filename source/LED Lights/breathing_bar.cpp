// Mbed program to test LPD8806-based RGB LED Strips
// (c) 2011 Jelmer Tiete
// This library is ported from the Arduino implementation of Adafruit Industries
// found at: http://github.com/adafruit/LPD8806
// and their strips: http://www.adafruit.com/products/306
// Released under the MIT License: http://mbed.org/license/mit
//
// standard connected to 1st hardware SPI
// LPD8806  <> MBED
// DATA     -> P5
// CLOCK    -> p7
/*****************************************************************************/

#include "LPD8806.h"
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#define ROWS 8
#define COLS 66


LPD8806 strip = LPD8806(528);
Serial pc(USBTX,USBRX);

static int grid[8][66] = {10};

void init_grid()
{
    int row;
    int col;
    
    for(col = 0; col < strip.numPixels()/ROWS; col++)
        grid[0][col] = col;
        
    for(col = 0; col < strip.numPixels()/ROWS; col++)
        grid[1][col] = strip.numPixels()/ROWS * 2 - 1 - col;
        
    for(col = 0; col < strip.numPixels()/ROWS-1; col++)
        grid[2][col] = strip.numPixels()/ROWS * 2 + col;
        
    for(col = 0; col < strip.numPixels()/ROWS-1; col++)
        grid[3][col] = strip.numPixels()/ROWS * 4 - 1 - col;
        
    for(col = 0; col < strip.numPixels()/ROWS-1; col++)
        grid[4][col] = strip.numPixels()/ROWS * 4 + col;
        
    for(col = 0; col < strip.numPixels()/ROWS-1; col++)
        grid[5][col] = strip.numPixels()/ROWS * 6 - 1 - col;
        
    for(col = 0; col < strip.numPixels()/ROWS-1; col++)
        grid[6][col] = strip.numPixels()/ROWS * 6 + col;
        
    for(col = 0; col < strip.numPixels()/ROWS-1; col++)
        grid[7][col] = strip.numPixels()/ROWS * 8 - 1 - col;        
}

// Chase a dot down the strip
// good for testing purposes
void colorChase(uint32_t c, uint8_t delay) {
    int i;

    //pc.printf("Number of LEDs are %d\r\n",strip.numPixels());
    for (i=0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, 0);  // turn all pixels off
    }

    for (i=0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, c);
        if (i == 0) {
            strip.setPixelColor(strip.numPixels()-1, 0);
        } else {
            strip.setPixelColor(i-1, 0);
        }
        strip.show();
        wait_ms(delay);
    }
}




void switchOn(uint32_t c)
{
    int i;
    
    //pc.printf("Number of LEDs are %d\r\n",strip.numPixels());
    for (i=0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, c);  // turn all pixels off
    }
    strip.show();
}

// fill the dots one after the other with said color
// good for testing purposes
void colorWipe(uint32_t c, uint8_t delay) {
    int i;

    for (i=0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, c);
        strip.show();
        wait_ms(delay);
    }
}

//Input a value 0 to 384 to get a color value.
//The colours are a transition r - g -b - back to r

uint32_t Wheel(uint16_t WheelPos) {
    uint8_t b=0;
    uint8_t g=0;
    uint8_t r = 0;
    switch (WheelPos / 128) {
        case 0:
            r = 127 - WheelPos % 128;   //Red down
            g = WheelPos % 128;      // Green up
            b = 0;                  //blue off
            break;
        case 1:
            g = 127 - WheelPos % 128;  //green down
            b = WheelPos % 128;      //blue up
            r = 0;                  //red off
            break;
        case 2:
            b = 127 - WheelPos % 128;  //blue down
            r = WheelPos % 128;      //red up
            g = 0;                  //green off
            break;
    }
    return(strip.Color(r,g,b));
}

void rainbow(uint8_t delay) {
    int i, j;

    for (j=0; j < 384; j++) {     // 3 cycles of all 384 colors in the wheel
        for (i=0; i < strip.numPixels(); i++) {
            strip.setPixelColor(i, Wheel( (i + j) % 384));
        }
        strip.show();   // write all the pixels out
        wait_ms(delay);
    }
}

// Slightly different, this one makes the rainbow wheel equally distributed
// along the chain
void rainbowCycle(uint8_t delay) {
    uint16_t i, j;

    for (j=0; j < 384 * 5; j++) {     // 5 cycles of all 384 colors in the wheel
        for (i=0; i < strip.numPixels(); i++) {
            // tricky math! we use each pixel as a fraction of the full 384-color wheel
            // (thats the i / strip.numPixels() part)
            // Then add in j which makes the colors go around per pixel
            // the % 384 is to make the wheel cycle around
            strip.setPixelColor(i, Wheel( ((i * 384 / strip.numPixels()) + j) % 384) );
        }
        strip.show();   // write all the pixels out
        wait_ms(delay);
    }
}

// "Larson scanner" = Cylon/KITT bouncing light effect
void scanner(uint8_t r, uint8_t g, uint8_t b, uint8_t delay) {
    int i, j, pos, dir;

    pos = 0;
    int pos2 = pos+71;
    int pos3 = pos2+1;
    int pos4 = pos3+71;
    dir = 1;
    int dir2 = -1;
    int dir3 = 1;
    int dir4 = -1;

    for (i=0; i<((strip.numPixels()/4)-1 * 8); i++) 
    {
        strip.setPixelColor(pos-4, strip.Color(r/16, g/16, b/16));
        strip.setPixelColor(pos-3, strip.Color(r/8, g/8, b/8));
        strip.setPixelColor(pos-2, strip.Color(r/4, g/4, b/4));
        strip.setPixelColor(pos-1, strip.Color(r/2, g/2, b/2));
        strip.setPixelColor(pos, strip.Color(r, g, b));
        
        strip.setPixelColor(pos2-4, strip.Color(r/16, g/16, b/16));
        strip.setPixelColor(pos2-3, strip.Color(r/8, g/8, b/8));
        strip.setPixelColor(pos2-2, strip.Color(r/4, g/4, b/4));
        strip.setPixelColor(pos2-1, strip.Color(r/2, g/2, b/2));
        strip.setPixelColor(pos2, strip.Color(r, g, b));
        
        
        strip.setPixelColor(pos3-4, strip.Color(r/16, g/16, b/16));
        strip.setPixelColor(pos3-3, strip.Color(r/8, g/8, b/8));
        strip.setPixelColor(pos3-2, strip.Color(r/4, g/4, b/4));
        strip.setPixelColor(pos3-1, strip.Color(r/2, g/2, b/2));
        strip.setPixelColor(pos3, strip.Color(r, g, b));
        
        strip.setPixelColor(pos4-4, strip.Color(r/16, g/16, b/16));
        strip.setPixelColor(pos4-3, strip.Color(r/8, g/8, b/8));
        strip.setPixelColor(pos4-2, strip.Color(r/4, g/4, b/4));
        strip.setPixelColor(pos4-1, strip.Color(r/2, g/2, b/2));
        strip.setPixelColor(pos4, strip.Color(r, g, b));
    
        strip.show();
        wait_ms(delay);
        // If we wanted to be sneaky we could erase just the tail end
        // pixel, but it's much easier just to erase the whole thing
        // and draw a new one next time.
        for (j=0; j< 5; j++)
        {
            strip.setPixelColor(pos-j, strip.Color(0,0,0));
            strip.setPixelColor(pos2-j, strip.Color(0,0,0));
            strip.setPixelColor(pos3-j, strip.Color(0,0,0));
            strip.setPixelColor(pos4-j, strip.Color(0,0,0));
            
            }
        // Bounce off ends of strip
        pos += dir;
        pos2 += dir2;
        pos3 += dir3;
        pos4 += dir4;
        
        if(pos < 0)
        {
            pos = 1;
            dir = -dir;   
        }
        else if (pos >= strip.numPixels()/4)
        {
            pos = strip.numPixels()/4 - 2;
            dir = -dir;
        }
        
        if(pos2 < 36)
        {
            pos2 = 36;
            dir2 = -dir2;   
        }
        else if (pos2 >= 71)
        {
            pos2 = 71;
            dir2 = -dir2;
        }
        
        if(pos3 < 72)
        {
            pos3 = 72;
            dir3 = -dir3;   
        }
        else if (pos3 >= 107)
        {
            pos3 = 107;
            dir3 = -dir3;
        }
        
        if(pos4 < 108)
        {
            pos4 = 108;
            dir4 = -dir4;   
        }
        else if (pos4 >= 143)
        {
            pos4 = 143;
            dir4 = -dir4;
        }
        
    }
}

void strobe(uint32_t c, uint8_t delay)
{
    int i;
            
                // turn the LEDs off for time delay
                for (i=0; i < strip.numPixels(); i++) {
                    strip.setPixelColor(i, 0);  // turn all pixels off
                }
                strip.show();
                wait_ms(delay);
                // turn them back on again for time delay
                for (i=0; i < strip.numPixels(); i++) {
                    strip.setPixelColor(i, c);  // turn all pixels off
                }
                strip.show();
                wait_ms(delay);
}    

void turnOffStrip()
{
    int i;
    pc.printf("turnOffStrip\r\n");
    //pc.printf("Number of LEDs are %d\r\n",strip.numPixels());
    for (i=0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, 0);  // turn all pixels off     
    }   
    strip.show();
}




int main() {
    printf("starting\r\n");    // Start up the LED strip
    strip.begin();
    pc.baud(9600);
    srand (time(NULL));
   
    int i;
    int row, col;

    init_grid();
         
    for (i=0; i<((strip.numPixels())); i++)
        strip.setPixelColor(i, strip.Color(0,0,0));
    strip.show();
 
    while (1) 
    {
      for(col = 0; col < 25; col++)
        {
            strip.setPixelColor(col, strip.Color(0,125,125));
            strip.show();
            wait_ms(100); 
        }
        wait(1);

        
        for(col = 26-1; col >=0 ; col--)
        {
            strip.setPixelColor(col, strip.Color(0,0,0));
            strip.show();
            wait_ms(100); 
        }    
        wait(1);

        
    }
}