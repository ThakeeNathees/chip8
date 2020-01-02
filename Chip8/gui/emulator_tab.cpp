#include "tabs.h"


sf::Vector2f EmulatorTab::getDispPosition() {
	return sf::Vector2f(MARGIN, MARGIN + TAB_HEIGHT + MARGIN);
}
sf::Vector2f EmulatorTab::getDisasPosition() {
	return getDispPosition() + sf::Vector2f(F2_DISP_WIDTH + MARGIN, 0);
}
sf::Vector2f EmulatorTab::getHexPosition() {
	return getDispPosition() + sf::Vector2f(0, F2_DISP_HEIGHT + MARGIN);
}
sf::Vector2f EmulatorTab::getRegPosition() {
	return getHexPosition() + sf::Vector2f(F2_HEXDUMP_WIDTH + MARGIN, 0);
}

void EmulatorTab::drawPixels(sf::RenderWindow& window) {
	sf::RectangleShape pix(sf::Vector2f(PIXEL_SIZE, PIXEL_SIZE));
	for (int h = 0; h < HEIGHT_PIX; h++) {
		for (int w = 0; w < WIDTH_PIX; w++) {
			if (m_pixels[h][w]) pix.setFillColor(PIX_COLOR);
			else pix.setFillColor(DISPLAY_BG_COLOR);
			pix.setPosition(sf::Vector2f(PIXEL_SIZE * w, PIXEL_SIZE * h) + getDispPosition());
			window.draw(pix);
		}
	}
}

void EmulatorTab::render(sf::RenderWindow& window) {

	// display
	drawBorder(window, getDispPosition(), sf::Vector2f(F2_DISP_WIDTH, F2_DISP_HEIGHT), DISPLAY_BG_COLOR);
	drawPixels(window);
	if (m_is_drawgrid) drawGrid(window);

	// disas
	drawBorder(window, getDisasPosition(), sf::Vector2f(F2_DISAS_WIDTH, F2_DISAS_HEIGHT));
	drawScrollBar(window, getDisasPosition() + sf::Vector2f(F2_DISAS_WIDTH, 0), F2_DISAS_HEIGHT, 0);


	// hexdump
	drawBorder(window, getHexPosition(), sf::Vector2f(F2_HEXDUMP_WIDTH, F2_HEXDUMP_HEIGHT));

	// registers
	drawBorder(window, getRegPosition(), sf::Vector2f(F2_HEXDUMP_WIDTH, F2_HEXDUMP_HEIGHT));

}

void EmulatorTab::drawGrid(sf::RenderWindow& window) {
	for (int i = 1; i < WIDTH_PIX; i++) {
		drawVertexLine(window, sf::Vector2f(PIXEL_SIZE * i, 0) + getDispPosition(), sf::Vector2f(PIXEL_SIZE * i, F2_DISP_HEIGHT) + getDispPosition(), DISPLAY_GRID_COLOR);
	}
	for (int i = 1; i < HEIGHT_PIX; i++) {
		drawVertexLine(window, sf::Vector2f(0, PIXEL_SIZE * i) + getDispPosition(), sf::Vector2f(F2_DISP_WIDTH, PIXEL_SIZE * i) + getDispPosition(), DISPLAY_GRID_COLOR);
	}
}




