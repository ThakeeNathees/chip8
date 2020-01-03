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
	
	// normal mode key bind
	if (event.type == sf::Event::KeyPressed && m_state == 0) {
		
		// navigation
		if (event.key.code == sf::Keyboard::Right && m_cursor_pos < ROM_SIZE - 1) { m_cursor_pos++; m_second_byte = false; }
		else if (event.key.code == sf::Keyboard::Left && m_cursor_pos > 0) { m_cursor_pos--; m_second_byte = false; }
		else if (event.key.code == sf::Keyboard::Down) {
			if (m_cursor_pos + F4_HEX_BYTE_PER_LNE < ROM_SIZE) m_cursor_pos += F4_HEX_BYTE_PER_LNE;
			m_second_byte = false;
		}
		else if (event.key.code == sf::Keyboard::Up) {
			if (m_cursor_pos >= F4_HEX_BYTE_PER_LNE ) m_cursor_pos -= F4_HEX_BYTE_PER_LNE;
			m_second_byte = false;
		}
		else if (event.key.code == sf::Keyboard::PageUp) {
			if (m_cursor_pos >= F4_HEX_BYTE_PER_LNE * F4_PAGEUPDOWN_LINES )
				m_cursor_pos -= F4_HEX_BYTE_PER_LNE * F4_PAGEUPDOWN_LINES;
		}
		else if (event.key.code == sf::Keyboard::PageDown) {
			if (m_cursor_pos + F4_HEX_BYTE_PER_LNE * F4_PAGEUPDOWN_LINES < ROM_SIZE)
				m_cursor_pos += F4_HEX_BYTE_PER_LNE * F4_PAGEUPDOWN_LINES;
		}
		else if (event.key.code == sf::Keyboard::Home) {
				m_cursor_pos = 0;
		}
		else if (event.key.code == sf::Keyboard::End) {
			m_cursor_pos = ROM_SIZE - 1;
		}

		// hex write 
		else if (event.key.code == sf::Keyboard::Num0 || event.key.code == sf::Keyboard::Numpad0) {
			if (!m_second_byte) { m_bytes[m_cursor_pos] = 0x00 | (m_bytes[m_cursor_pos] & 0x0f); m_second_byte = true; }
			else { m_bytes[m_cursor_pos] = 0x00 | (m_bytes[m_cursor_pos] & 0xf0); m_second_byte = false; m_cursor_pos++; }
		}
		else if (event.key.code == sf::Keyboard::Num1 || event.key.code == sf::Keyboard::Numpad1) {
			if (!m_second_byte) { m_bytes[m_cursor_pos] = 0x10 | (m_bytes[m_cursor_pos] & 0x0f); m_second_byte = true; }
			else { m_bytes[m_cursor_pos] = 0x01 | (m_bytes[m_cursor_pos] & 0xf0); m_second_byte = false; m_cursor_pos++; }
		}
		else if (event.key.code == sf::Keyboard::Num2 || event.key.code == sf::Keyboard::Numpad2) {
			if (!m_second_byte) { m_bytes[m_cursor_pos] = 0x20 | (m_bytes[m_cursor_pos] & 0x0f); m_second_byte = true; }
			else { m_bytes[m_cursor_pos] = 0x02 | (m_bytes[m_cursor_pos] & 0xf0); m_second_byte = false; m_cursor_pos++; }
		}
		else if (event.key.code == sf::Keyboard::Num3 || event.key.code == sf::Keyboard::Numpad3) {
			if (!m_second_byte) { m_bytes[m_cursor_pos] = 0x30 | (m_bytes[m_cursor_pos] & 0x0f); m_second_byte = true; }
			else { m_bytes[m_cursor_pos] = 0x03 | (m_bytes[m_cursor_pos] & 0xf0); m_second_byte = false; m_cursor_pos++; }
		}
		else if (event.key.code == sf::Keyboard::Num4 || event.key.code == sf::Keyboard::Numpad4) {
			if (!m_second_byte) { m_bytes[m_cursor_pos] = 0x40 | (m_bytes[m_cursor_pos] & 0x0f); m_second_byte = true; }
			else { m_bytes[m_cursor_pos] = 0x04 | (m_bytes[m_cursor_pos] & 0xf0); m_second_byte = false; m_cursor_pos++; }
		}
		else if (event.key.code == sf::Keyboard::Num5 || event.key.code == sf::Keyboard::Numpad5) {
			if (!m_second_byte) { m_bytes[m_cursor_pos] = 0x50 | (m_bytes[m_cursor_pos] & 0x0f); m_second_byte = true; }
			else { m_bytes[m_cursor_pos] = 0x05 | (m_bytes[m_cursor_pos] & 0xf0); m_second_byte = false; m_cursor_pos++; }
		}
		else if (event.key.code == sf::Keyboard::Num6 || event.key.code == sf::Keyboard::Numpad6) {
			if (!m_second_byte) { m_bytes[m_cursor_pos] = 0x60 | (m_bytes[m_cursor_pos] & 0x0f); m_second_byte = true; }
			else { m_bytes[m_cursor_pos] = 0x06 | (m_bytes[m_cursor_pos] & 0xf0); m_second_byte = false; m_cursor_pos++; }
		}
		else if (event.key.code == sf::Keyboard::Num7 || event.key.code == sf::Keyboard::Numpad7) {
			if (!m_second_byte) { m_bytes[m_cursor_pos] = 0x70 | (m_bytes[m_cursor_pos] & 0x0f); m_second_byte = true; }
			else { m_bytes[m_cursor_pos] = 0x07 | (m_bytes[m_cursor_pos] & 0xf0); m_second_byte = false; m_cursor_pos++; }
		}
		else if (event.key.code == sf::Keyboard::Num8 || event.key.code == sf::Keyboard::Numpad8) {
			if (!m_second_byte) { m_bytes[m_cursor_pos] = 0x80 | (m_bytes[m_cursor_pos] & 0x0f); m_second_byte = true; }
			else { m_bytes[m_cursor_pos] = 0x08 | (m_bytes[m_cursor_pos] & 0xf0); m_second_byte = false; m_cursor_pos++; }
		}
		else if (event.key.code == sf::Keyboard::Num9 || event.key.code == sf::Keyboard::Numpad9) {
			if (!m_second_byte) { m_bytes[m_cursor_pos] = 0x90 | (m_bytes[m_cursor_pos] & 0x0f); m_second_byte = true; }
			else { m_bytes[m_cursor_pos] = 0x09 | (m_bytes[m_cursor_pos] & 0xf0); m_second_byte = false; m_cursor_pos++; }
		}

		else if (event.key.code == sf::Keyboard::A) {
			if (!m_second_byte) { m_bytes[m_cursor_pos] = 0xa0 | (m_bytes[m_cursor_pos] & 0x0f); m_second_byte = true; }
			else { m_bytes[m_cursor_pos] = 0x0a | (m_bytes[m_cursor_pos] & 0xf0); m_second_byte = false; m_cursor_pos++; }
		}
		else if (event.key.code == sf::Keyboard::B) {
			if (!m_second_byte) { m_bytes[m_cursor_pos] = 0xb0 | (m_bytes[m_cursor_pos] & 0x0f); m_second_byte = true; }
			else { m_bytes[m_cursor_pos] = 0x0b | (m_bytes[m_cursor_pos] & 0xf0); m_second_byte = false; m_cursor_pos++; }
		}
		else if (event.key.code == sf::Keyboard::C) {
			if (!m_second_byte) { m_bytes[m_cursor_pos] = 0xc0 | (m_bytes[m_cursor_pos] & 0x0f); m_second_byte = true; }
			else { m_bytes[m_cursor_pos] = 0x0c | (m_bytes[m_cursor_pos] & 0xf0); m_second_byte = false; m_cursor_pos++; }
		}
		else if (event.key.code == sf::Keyboard::D) {
			if (!m_second_byte) { m_bytes[m_cursor_pos] = 0xd0 | (m_bytes[m_cursor_pos] & 0x0f); m_second_byte = true; }
			else { m_bytes[m_cursor_pos] = 0x0d | (m_bytes[m_cursor_pos] & 0xf0); m_second_byte = false; m_cursor_pos++; }
		}
		else if (event.key.code == sf::Keyboard::E) {
			if (!m_second_byte) { m_bytes[m_cursor_pos] = 0xe0 | (m_bytes[m_cursor_pos] & 0x0f); m_second_byte = true; }
			else { m_bytes[m_cursor_pos] = 0x0e | (m_bytes[m_cursor_pos] & 0xf0); m_second_byte = false; m_cursor_pos++; }
		}
		else if (event.key.code == sf::Keyboard::F) {
			if (!m_second_byte) { m_bytes[m_cursor_pos] = 0xf0 | (m_bytes[m_cursor_pos] & 0x0f); m_second_byte = true; }
			else { m_bytes[m_cursor_pos] = 0x0f | (m_bytes[m_cursor_pos] & 0xf0); m_second_byte = false; m_cursor_pos++; }
		}

		// popup mode key bind
		if (event.type == sf::Event::KeyPressed && m_state == 1) {

		}
		

	}
}

void DisassemblerTab::render(sf::RenderWindow& window) {
	// hex
	drawBorder(window, getHexPosition(), sf::Vector2f(F4_HEXDUMP_WIDTH, F4_HEXDUMP_HEIGHT));
	float scroll_p = drawHexDump(window, getHexPosition(), m_bytes, F4_HEX_BYTE_PER_LNE, F4_HEXDUMP_HEIGHT, m_cursor_pos);
	drawScrollBar(window, getHexPosition() + sf::Vector2f(F4_HEXDUMP_WIDTH, 0), F4_HEXDUMP_HEIGHT, scroll_p);


	// disas
	drawBorder(window, getDisasPosition(), sf::Vector2f(F4_DISAS_WIDTH, F4_DISAS_HEIGHT));
	drawScrollBar(window, getDisasPosition() + sf::Vector2f(F4_DISAS_WIDTH, 0), F4_DISAS_HEIGHT, 0);


	// info
	Res::s_text.setPosition(getInfoPosition()); Res::s_text.setFillColor(F4_HELP_TEXT_COLOR);
	Res::s_text.setString("Ctrl-O : open new file");
	window.draw(Res::s_text);
}