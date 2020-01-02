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

void DisassemblerTab::handleEvent(sf::Event& event) {
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Right && m_cursor_pos < ROM_SIZE-1) m_cursor_pos++;
		if (event.key.code == sf::Keyboard::Left && m_cursor_pos > 0) m_cursor_pos--;
		if (event.key.code == sf::Keyboard::Down) {
			if (m_cursor_pos + F4_DISAS_HEX_BYTE_PER_LNE < ROM_SIZE) m_cursor_pos += F4_DISAS_HEX_BYTE_PER_LNE;
		}
		if (event.key.code == sf::Keyboard::Up) {
			if (m_cursor_pos >= F4_DISAS_HEX_BYTE_PER_LNE ) m_cursor_pos -= F4_DISAS_HEX_BYTE_PER_LNE;
		}

	}
}

void DisassemblerTab::render(sf::RenderWindow& window) {
	// hex
	drawBorder(window, getHexPosition(), sf::Vector2f(F4_HEXDUMP_WIDTH, F4_HEXDUMP_HEIGHT));

	// draw bytes loop
	static unsigned int total_lines = ROM_SIZE / F4_DISAS_HEX_BYTE_PER_LNE;
	static unsigned int line_offset = 0;
	unsigned int lines = (F4_HEXDUMP_HEIGHT - 2 * FONT_SIZE) / (FONT_SIZE + F4_DISAS_HEX_LINE_SPACEING);
	unsigned int cursor_line = m_cursor_pos / F4_DISAS_HEX_BYTE_PER_LNE;
	if (cursor_line >= lines + line_offset) line_offset = cursor_line - lines + 1;
	if (cursor_line < line_offset) line_offset = cursor_line;

	float scroll_p = (float)cursor_line / (float)total_lines;

	sf::Vector2f pos = getHexPosition()+ sf::Vector2f(MARGIN, MARGIN);
	sf::RectangleShape cursor(sf::Vector2f(FONT_SIZE * 2, FONT_SIZE));
	cursor.setFillColor(F4_CURSOR_COLOR);
	for (unsigned int i = F4_DISAS_HEX_BYTE_PER_LNE * line_offset; i < lines * F4_DISAS_HEX_BYTE_PER_LNE + F4_DISAS_HEX_BYTE_PER_LNE * line_offset; i++) { // TODO: MAX LINE NO -> min(total lines, claculated)

		// draw address
		m_text.setFillColor(BYTES_COLOR);
		if (i % F4_DISAS_HEX_BYTE_PER_LNE == 0) {
			m_text.setString(toHexString(i, 4));
			m_text.setPosition(pos);
			window.draw(m_text);
			pos += sf::Vector2f(FONT_SIZE + F4_HEX_BYTE_SPACEING*4, 0);
		}

		// draw bytes
		m_text.setString(toHexString(m_bytes[i]));
		m_text.setPosition(pos); 
		
		if (i == m_cursor_pos) { // draw cursor
			m_text.setFillColor(SELECTED_BYTE_COLOR);
			cursor.setPosition(m_text.getPosition());
			window.draw(cursor);
		}
		window.draw(m_text);

		if (i % F4_DISAS_HEX_BYTE_PER_LNE == F4_DISAS_HEX_BYTE_PER_LNE - 1) {
			pos = sf::Vector2f(getHexPosition().x + MARGIN, pos.y + FONT_SIZE + F4_DISAS_HEX_LINE_SPACEING);
		}
		else pos += sf::Vector2f(FONT_SIZE + F4_HEX_BYTE_SPACEING, 0);
	}
	drawScrollBar(window, getHexPosition() + sf::Vector2f(F4_HEXDUMP_WIDTH, 0), F4_HEXDUMP_HEIGHT, scroll_p);



	// disas
	drawBorder(window, getDisasPosition(), sf::Vector2f(F4_DISAS_WIDTH, F4_DISAS_HEIGHT));
	drawScrollBar(window, getDisasPosition() + sf::Vector2f(F4_DISAS_WIDTH, 0), F4_DISAS_HEIGHT, 0);


	// info
	m_text.setPosition(getInfoPosition());
	m_text.setString("Ctrl-O : open new file");
	window.draw(m_text);
}