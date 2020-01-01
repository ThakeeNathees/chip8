#pragma once

/* window dimension emulation

			margin	       width    	  margin width margin
			______________________________________________
margin		|--------------------------------------------|
tab			|--------------------------------------------|
margin      |+-------------------------------++---------+|
			||							     || 		||
height-disp	||								 ||			||
			||_______________________________||_________||
margin		|+------------------++----------------------||
			||				    ||                      ||
height-hex	||				    ||                      ||
            |+------------------++----------------------||
margin		++------------------------------------------++
                hexdump_width          total_width - hexdump
*/

#define FONT_PATH "res/font/PressStart2P-Regular.ttf"

// sizes
#define MARGIN 10

#define TAB_HEIGHT  40
#define TAB_WIDTH   200

#define PIXEL_SIZE  10
#define WIDTH_PIX   64
#define HEIGHT_PIX  32
#define DISAS_WIDTH 200
#define HEXDUMP_HEIGHT 200

#define DISP_WIDTH  PIXEL_SIZE*WIDTH_PIX
#define DISP_HEIGHT PIXEL_SIZE*HEIGHT_PIX

#define WINDOW_WIDTH  MARGIN + DISP_WIDTH  + MARGIN + DISAS_WIDTH + MARGIN
#define WINDOW_HEIGHT MARGIN + TAB_HEIGHT + MARGIN + DISP_HEIGHT + MARGIN + HEXDUMP_HEIGHT + MARGIN

#define DISAS_HEIGHT PIXEL_SIZE*HEIGHT_PIX

#define HEXDUMP_WIDTH (DISP_WIDTH  + DISAS_WIDTH) / 2

#define REG_INFO_WIDTH  (DISP_WIDTH  + DISAS_WIDTH) - HEXDUMP_WIDTH
#define REG_INFO_HEIGHT HEXDUMP_HEIGHT

// colors
#define BG_COLOR sf::Color(32,42,52)
#define TAB_TITLE_COLOR sf::Color::White