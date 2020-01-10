#include "tabs.h"



void EmulatorTab::handleEvent(sf::Event& event) {
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Key::G) setGrid();
	}

	if (event.type == sf::Event::KeyPressed) {
		// navigation
		if (event.key.code == sf::Keyboard::Up) {
			if (m_cursor_pos >= 2) m_cursor_pos-=2;
		}
		if (event.key.code == sf::Keyboard::Down) {
			if (m_cursor_pos < ROM_SIZE - 2) m_cursor_pos+=2;
		}

		// if enter step
		if (event.key.code == sf::Keyboard::Enter) {
			m_emulator.step();
			m_cursor_pos = m_emulator.getPc();
		}

		// ctrl + o
		if (event.key.code == sf::Keyboard::O) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ||
				sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
			{
				std::string m_working_file = browse_file();
				if (m_working_file != std::string("")) {
					std::ifstream myFile(m_working_file, std::ios::in | std::ios::binary);
					myFile.seekg(0, std::ios_base::end);
					int file_size = myFile.tellg();

					myFile.seekg(0);
					if (!myFile.read((char*)(m_emulator.getBytesToWrite()), std::min(ROM_SIZE, file_size))) {
						// m_state = 1;
						// m_error_msg = std::string("Error reading file : ").append(m_working_file); // TODO: replace \ with /
					}

					m_emulator.reset();
					m_emulator.disassemble();
				}
			}
		}
	}
}

void EmulatorTab::process() {
	
}


void EmulatorTab::render(sf::RenderWindow& window) {

	// display
	drawBorder(window, getDispPosition(), sf::Vector2f(F2_DISP_WIDTH, F2_DISP_HEIGHT), DISPLAY_BG_COLOR);
	drawPixels(window);
	if (m_is_drawgrid) drawGrid(window);

	// disas
	drawBorder(window, getDisasPosition(), sf::Vector2f(F2_DISAS_WIDTH, F2_DISAS_HEIGHT));
	float scroll_disas = drawDisassembly(window, getDisasPosition(), m_emulator.getInstructions(), sf::Vector2f(F2_DISAS_WIDTH, F2_DISAS_HEIGHT), m_cursor_pos / 2);
	drawScrollBar(window, getDisasPosition() + sf::Vector2f(F2_DISAS_WIDTH, 0), F2_DISAS_HEIGHT, scroll_disas);


	// hexdump
	drawBorder(window, getHexPosition(), sf::Vector2f(F2_HEXDUMP_WIDTH, F2_HEXDUMP_HEIGHT));
	float scroll_p = drawHexDump(window, getHexPosition(), m_emulator.getBytes(), F2_HEX_BYTE_PER_LNE, F2_HEXDUMP_HEIGHT,  m_cursor_pos);
	drawScrollBar(window, getHexPosition() + sf::Vector2f(F2_HEXDUMP_WIDTH, 0), F2_HEXDUMP_HEIGHT, scroll_p);

	// registers
	int x;
	x = WINDOW_WIDTH;
	x = F2_HEXDUMP_WIDTH;
	x = F2_REG_INFO_WIDTH;
	drawBorder(window, getRegPosition(), sf::Vector2f(F2_REG_INFO_WIDTH, F2_REG_INFO_HEIGHT));

}

/********************************** PRIVATE METHODS ******************************************************/

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
			if ( m_emulator.getPixels()[h][w]) pix.setFillColor(PIX_COLOR);
			else pix.setFillColor(DISPLAY_BG_COLOR);
			pix.setPosition(sf::Vector2f(PIXEL_SIZE * w, PIXEL_SIZE * h) + getDispPosition());
			window.draw(pix);
		}
	}
}

void EmulatorTab::drawGrid(sf::RenderWindow& window) {
	for (int i = 1; i < WIDTH_PIX; i++) {
		drawVertexLine(window, sf::Vector2f(PIXEL_SIZE * i, 0) + getDispPosition(), sf::Vector2f(PIXEL_SIZE * i, F2_DISP_HEIGHT) + getDispPosition(), DISPLAY_GRID_COLOR);
	}
	for (int i = 1; i < HEIGHT_PIX; i++) {
		drawVertexLine(window, sf::Vector2f(0, PIXEL_SIZE * i) + getDispPosition(), sf::Vector2f(F2_DISP_WIDTH, PIXEL_SIZE * i) + getDispPosition(), DISPLAY_GRID_COLOR);
	}
}




