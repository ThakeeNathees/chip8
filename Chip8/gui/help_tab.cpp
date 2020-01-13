#include "include.h"
#include "tabs.h"


void HelpTab::handleEvent(sf::Event& even) {

}

void HelpTab::process() {

}

void HelpTab::render(sf::RenderWindow& window) {
	Res::setTextString("\nKEYBOARD\n-----------------------------------\nT - restart\nG - grid\nB - set/remove break point \nN - clear all break points\nS - save file\nO - open file\nF5    - run\nenter - step\n\n\nnavigation keys :\n    F1, F2, F3, F4,  tab, arrow keys,  page up, page down\n\nmapped emulator keys :\n    1   2   3   C       1   2   3   4\n    4   5   6   D       Q   W   E   R\n    7   8   9   E       A   S   D   F\n    A   0   B   F       Z   X   C   V\n\n");
	Res::setTextPosition(sf::Vector2f(MARGIN, MARGIN + TAB_HEIGHT + MARGIN));
	window.draw(Res::getText());
}