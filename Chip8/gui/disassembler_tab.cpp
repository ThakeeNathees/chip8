#include "tabs.h"

sf::Vector2f DisassemblerTab::getHexPosition() {
	return sf::Vector2f(MARGIN, MARGIN + TAB_HEIGHT + MARGIN);
}
sf::Vector2f DisassemblerTab::getDisasPosition() {
	return getHexPosition() + sf::Vector2f(F4_HEXDUMP_WIDTH+MARGIN, 0);
}
sf::Vector2f DisassemblerTab::getInfoPosition() {
	return getHexPosition() + sf::Vector2f(0, F4_HEXDUMP_HEIGHT + MARGIN);
}

void DisassemblerTab::render(sf::RenderWindow& window) {
	// hex
	drawBorder(window, getHexPosition(), sf::Vector2f(F4_HEXDUMP_WIDTH, F4_HEXDUMP_HEIGHT));
	drawScrollBar(window, getHexPosition() + sf::Vector2f(F4_HEXDUMP_WIDTH, 0), F4_HEXDUMP_HEIGHT, 0);

	sf::Vector2f pos = getHexPosition();
	for (int i = 0; i < ROM_SIZE; i++) {
		m_text.setString(toHexString(m_bytes[i]));
		m_text.setPosition(pos);
		window.draw(m_text);
		if (i == 0x11f) break;
		if (i % 0x10 == 0x10 - 1) pos = sf::Vector2f(getHexPosition().x, pos.y + FONT_SIZE);
		else pos += sf::Vector2f(FONT_SIZE + 10, 0);
	}


	// disas
	drawBorder(window, getDisasPosition(), sf::Vector2f(F4_DISAS_WIDTH, F4_DISAS_HEIGHT));
	drawScrollBar(window, getDisasPosition() + sf::Vector2f(F4_DISAS_WIDTH, 0), F4_DISAS_HEIGHT, 0);


	// info
	m_text.setPosition(getInfoPosition());
	m_text.setString("Ctrl-O : open new file");
	window.draw(m_text);
}