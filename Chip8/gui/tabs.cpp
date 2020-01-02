#include "tabs.h"


sf::Vector2f EmulatorTab::getDispPosition() {
	return sf::Vector2f(MARGIN, MARGIN + TAB_HEIGHT + MARGIN);
}
sf::Vector2f EmulatorTab::getDisasPosition() {
	return getDispPosition() + sf::Vector2f(DISP_WIDTH + MARGIN, 0);
}
sf::Vector2f EmulatorTab::getHexPosition() {
	return getDispPosition() + sf::Vector2f(0, DISP_HEIGHT + MARGIN);
}
sf::Vector2f EmulatorTab::getRegPosition() {
	return getHexPosition() + sf::Vector2f(HEXDUMP_WIDTH+MARGIN, 0);
}

void EmulatorTab::drawPixels(sf::RenderWindow& window) {
	sf::RectangleShape pix(sf::Vector2f(PIXEL_SIZE, PIXEL_SIZE));
	for (int h = 0; h < HEIGHT_PIX; h++) {
		for (int w = 0; w < WIDTH_PIX; w++) {
			if (m_pixels[h][w]) pix.setFillColor(PIX_COLOR);
			else pix.setFillColor(DISPLAY_BG_COLOR);
			pix.setPosition( sf::Vector2f(PIXEL_SIZE*w, PIXEL_SIZE*h) + getDispPosition() );
			window.draw(pix);
		}
	}
}

void EmulatorTab::render(sf::RenderWindow& window) {

	// display
	sf::RectangleShape disp_area(sf::Vector2f(DISP_WIDTH, DISP_HEIGHT));
	disp_area.setPosition(getDispPosition()); disp_area.setFillColor(DISPLAY_BG_COLOR); 
	disp_area.setOutlineThickness(BORDER_SIZE); disp_area.setOutlineColor(BORDER_COLOR);
	window.draw(disp_area);
	drawPixels(window);
	if (m_is_drawgrid) drawGrid(window);

	// disas
	sf::RectangleShape disas_border(sf::Vector2f(DISAS_WIDTH, DISAS_HEIGHT));
	disas_border.setPosition(getDisasPosition()); disas_border.setFillColor(COLOR_NONE);
	disas_border.setOutlineThickness(BORDER_SIZE); disas_border.setOutlineColor(BORDER_COLOR);
	window.draw(disas_border);

	// hexdump
	sf::RectangleShape hex_border(sf::Vector2f(HEXDUMP_WIDTH, HEXDUMP_HEIGHT));
	hex_border.setPosition(getHexPosition()); hex_border.setFillColor(COLOR_NONE);
	hex_border.setOutlineThickness(BORDER_SIZE); hex_border.setOutlineColor(BORDER_COLOR);
	window.draw(hex_border);

	// registers
	sf::RectangleShape reg_border(sf::Vector2f(HEXDUMP_WIDTH, HEXDUMP_HEIGHT));
	reg_border.setPosition(getRegPosition()); reg_border.setFillColor(COLOR_NONE);
	reg_border.setOutlineThickness(BORDER_SIZE); reg_border.setOutlineColor(BORDER_COLOR);
	window.draw(reg_border);

}

void EmulatorTab::drawGrid(sf::RenderWindow& window) {
	for (int i = 1; i < WIDTH_PIX; i++) {
		drawVertexLine(window, sf::Vector2f(PIXEL_SIZE * i, 0) + getDispPosition(), sf::Vector2f(PIXEL_SIZE * i, DISP_HEIGHT) + getDispPosition(), DISPLAY_GRID_COLOR);
	}
	for (int i = 1; i < HEIGHT_PIX; i++) {
		drawVertexLine( window, sf::Vector2f(0, PIXEL_SIZE*i) + getDispPosition(), sf::Vector2f(DISP_WIDTH, PIXEL_SIZE*i) + getDispPosition(), DISPLAY_GRID_COLOR );
	}
}




