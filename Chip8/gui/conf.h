#pragma once
#include "include.h"

#define ROM_SIZE 0x1000
#define PROGRAMME_OFFSET 0x200

#define FONT_PATH "res/font/west_england/west_england.ttf"
//#define FONT_PATH "res/font/PressStart2P-Regular.ttf"
#define FONT_SIZE 20

#define FONT_ADJUSTMENT sf::Vector2f(5, -5)

class Res {
public:
	static void init() {
		s_font.loadFromFile(FONT_PATH);
		s_text.setFont(s_font);
		s_text.setCharacterSize(FONT_SIZE);
	}
	static void setTextPosition(sf::Vector2f pos) {
		s_text.setPosition(pos + FONT_ADJUSTMENT);
	}
	static sf::Vector2f getTextPosition() {
		return s_text.getPosition() - FONT_ADJUSTMENT;
	}
	static void setTextColor(const sf::Color& color) {
		s_text.setFillColor(color);
	}
	static void setTextString(std::string str) {
		s_text.setString(str);
	}

	static const sf::Text& getText() {
		return s_text;
	}

private:
	static sf::Font s_font;
	static sf::Text s_text;
};


// colors
#define COLOR_NONE sf::Color(0,0,0,0)
#define BORDER_COLOR sf::Color(70, 80, 100)
#define BG_COLOR sf::Color(32,42,52)

#define DISPLAY_BG_COLOR  sf::Color(143, 143, 133)
#define PIX_COLOR sf::Color(32,42,52)
//sf::Color::Black
#define DISPLAY_GRID_COLOR sf::Color::Black

#define TAB_TITLE_COLOR sf::Color(200,200,200)
#define TAB_SELECTED_TITLE_COLOR sf::Color(200,165,0)

#define DISAS_CURSOR_COLOR sf::Color::White
#define DISAS_LINE_COLOR sf::Color::White
#define DISAS_SELECTED_LINE_COLOR sf::Color::Black

#define F2_REGISTER_LINE_SPACE  MARGIN + 2*FONT_SIZE
#define F2_STACK_LINE_SPACE 2*MARGIN + 5* FONT_SIZE

#define F4_CURSOR_COLOR sf::Color::White
#define F4_HELP_TEXT_COLOR sf::Color::White
#define BYTES_COLOR sf::Color::White
#define SELECTED_BYTE_COLOR sf::Color::Black

#define EMULATOR_STATE_TEXT_COLOR sf::Color::White

#define POPUP_BG_COLOR sf::Color(170, 170, 170, 50)
#define CLOSE_BUTTON_COLOR sf::Color(230, 90, 90)

// sizes
#define MARGIN 10
#define BORDER_SIZE 1

#define TAB_TITLE_SIZE 20
#define TAB_HEIGHT  40
#define TAB_WIDTH   200


// other values
#define HEX_BYTE_SPACEING 15
#define HEX_LINE_SPACEING 5
#define DISAS_LINE_SPACING 5

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


#define PIXEL_SIZE  10
#define WIDTH_PIX   64
#define HEIGHT_PIX  32
#define F2_DISAS_WIDTH 310
#define F2_HEXDUMP_HEIGHT 200
#define F2_HEX_BYTE_PER_LNE 0x08

#define F2_PAGEUPDOWN_LINES 6

#define F2_DISP_WIDTH  PIXEL_SIZE*WIDTH_PIX
#define F2_DISP_HEIGHT PIXEL_SIZE*HEIGHT_PIX

#define WINDOW_WIDTH  (MARGIN + F2_DISP_WIDTH  + MARGIN + F2_DISAS_WIDTH + MARGIN)
#define WINDOW_HEIGHT (MARGIN + TAB_HEIGHT + MARGIN + F2_DISP_HEIGHT + MARGIN + F2_HEXDUMP_HEIGHT + MARGIN)

#define F2_DISAS_HEIGHT PIXEL_SIZE*HEIGHT_PIX

#define F2_HEXDUMP_WIDTH (FONT_SIZE + (HEX_BYTE_SPACEING * 4 +(FONT_SIZE + HEX_BYTE_SPACEING)* F2_HEX_BYTE_PER_LNE) + 4*MARGIN )

#define F2_REG_INFO_WIDTH  (WINDOW_WIDTH - F2_HEXDUMP_WIDTH - 3*MARGIN)
#define F2_REG_INFO_HEIGHT F2_HEXDUMP_HEIGHT


#define POPUP_WIDTH WINDOW_WIDTH * .8
#define POPUP_HEIGHT WINDOW_HEIGHT * .8
#define POPUP_POS_X (WINDOW_WIDTH-POPUP_WIDTH)/2
#define POPUP_POS_Y (WINDOW_HEIGHT-POPUP_HEIGHT)/2
/* window dimension emulation
scroll bar
			margin	       width    	  margin width margin
			______________________________________________
margin		|--------------------------------------------|
tab			|--------------------------------------------|
margin      |+-------------------------------++---------+|
			||							     || 		||
hex-disp	||								 ||			||
			||								 ||			||
			||								 ||			||
			||_______________________________||_________||
margin		|+------------------------------------------||
bottom_info |											 |
margin		++------------------------------------------++
				hexdump_width          total_width - hexdump
*/

#define F4_HEX_BYTE_PER_LNE 0x10
#define F4_PAGEUPDOWN_LINES 10

#define F4_HEXDUMP_WIDTH (FONT_SIZE + (HEX_BYTE_SPACEING * 4 +(FONT_SIZE + HEX_BYTE_SPACEING)* F4_HEX_BYTE_PER_LNE) + 4*MARGIN)
#define F4_HEXDUMP_HEIGHT (WINDOW_HEIGHT - 150)

#define F4_DISAS_WIDTH  (WINDOW_WIDTH - ( MARGIN + F4_HEXDUMP_WIDTH + MARGIN + /* disas */ + MARGIN ))
#define F4_DISAS_HEIGHT F4_HEXDUMP_HEIGHT

#define F4_INFO_WIDTH 400