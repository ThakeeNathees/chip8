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
	
		// mouse scroll page up/down
		if (event.type == sf::Event::MouseWheelScrolled && m_state == 0) {
			if (event.mouseWheelScroll.delta > 0) {
				if (m_cursor_pos >= F4_HEX_BYTE_PER_LNE * F4_PAGEUPDOWN_LINES)
					m_cursor_pos -= F4_HEX_BYTE_PER_LNE * F4_PAGEUPDOWN_LINES;
				else m_cursor_pos = 0;
			}
			else {
				if (m_cursor_pos + F4_HEX_BYTE_PER_LNE * F4_PAGEUPDOWN_LINES < ROM_SIZE)
					m_cursor_pos += F4_HEX_BYTE_PER_LNE * F4_PAGEUPDOWN_LINES;
				else m_cursor_pos = ROM_SIZE - 1;
			}
		}

	// mouse popup close
	if (event.type == sf::Event::MouseButtonReleased && m_state != 0) { // TODO: check states
		if (mouseInPopupClose(event)) {
			m_state = 0;
		}
	}

	// mouse clicked on byte
	if (event.type == sf::Event::MouseButtonReleased && m_state == 0) {
		sf::Vector2i pos = mouseF4HexdumpByte(event, getHexPosition());
		if (pos.x < 0 || pos.y < 0); // no byte selected
		else {
			m_cursor_pos = pos.x + F4_HEX_BYTE_PER_LNE * (pos.y + m_line_offset);
			m_second_byte = false;
		}
	}

	// mouse clicked on info
	if (event.type == sf::Event::MouseButtonReleased && m_state == 0) {
		sf::Vector2f pos = getInfoPosition();
		if (pos.x < event.mouseButton.x && event.mouseButton.x < pos.x + F4_INFO_WIDTH) {
			
			if (pos.y < event.mouseButton.y && event.mouseButton.y < pos.y + FONT_SIZE) { // ctrl + o
				m_working_file = browse_file();
				if (m_working_file != std::string("")) {
					std::ifstream myFile(m_working_file, std::ios::in | std::ios::binary);
					myFile.seekg(0, std::ios_base::end);
					int file_size = myFile.tellg();

					myFile.seekg(0);
					if (!myFile.read((char*)&m_bytes, std::min(ROM_SIZE, file_size))) {
						m_state = 1;
						m_error_msg = std::string("Error reading file : ").append(m_working_file); // TODO: replace \ with /
					}
				}
			}

			if (pos.y + FONT_SIZE + MARGIN / 2 < event.mouseButton.y && event.mouseButton.y < pos.y + 2 * FONT_SIZE + MARGIN / 2) { // ctrl + s
				if (m_working_file == std::string("")) {
					m_working_file = browse_file();
				}
				if (m_working_file != std::string("")) {
					std::ofstream myFile(m_working_file, std::ios::out | std::ios::binary);
					if (!myFile.write((char*)&m_bytes, ROM_SIZE)) {
						m_state = 1;
						m_error_msg = std::string("Error writeing file : \n").append(m_working_file); // TODO: replace \ with /
					}
				}
			}
		}
		

	}


	// normal mode key bind
	if (event.type == sf::Event::KeyPressed && m_state == 0) {

		// open o
		if (event.key.code == sf::Keyboard::O) {
			// if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ||
			// 	sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
			// {
				m_working_file = browse_file();
				if (m_working_file != std::string("")) {
					std::ifstream myFile(m_working_file, std::ios::in | std::ios::binary);
					myFile.seekg(0, std::ios_base::end);
					int file_size = myFile.tellg();

					myFile.seekg(0);
					if (!myFile.read((char*)&m_bytes[PROGRAMME_OFFSET], std::min(ROM_SIZE-PROGRAMME_OFFSET, file_size))) {
						m_state = 1;
						m_error_msg = std::string("Error reading file : ").append(m_working_file); // TODO: replace \ with /
					}
				}
			// }
		}
		// save s
		if (event.key.code == sf::Keyboard::S) {
			//if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ||
			//	sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
			//{
				if (m_working_file == std::string("")) {
					m_working_file = browse_file();
				}
				if (m_working_file != std::string("")) {
					std::ofstream myFile(m_working_file, std::ios::out | std::ios::binary);
					if (!myFile.write((char*)&(m_bytes[PROGRAMME_OFFSET]), ROM_SIZE)) {
						m_state = 1;
						m_error_msg = std::string("Error writeing file : \n").append(m_working_file); // TODO: replace \ with /
					}
				}
			//}
				 
		}
		
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
			else m_cursor_pos = 0;
		}
		else if (event.key.code == sf::Keyboard::PageDown) {
			if (m_cursor_pos + F4_HEX_BYTE_PER_LNE * F4_PAGEUPDOWN_LINES < ROM_SIZE)
				m_cursor_pos += F4_HEX_BYTE_PER_LNE * F4_PAGEUPDOWN_LINES;
			else m_cursor_pos = ROM_SIZE - 1;
		}
		else if (event.key.code == sf::Keyboard::Home) {
				m_cursor_pos = PROGRAMME_OFFSET;
				m_line_offset = PROGRAMME_OFFSET / F4_HEX_BYTE_PER_LNE;
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
	}
	if (event.type == sf::Event::KeyPressed && m_state == 1) {
		if (event.key.code == sf::Keyboard::Escape) {
			m_state = 0;
		}
	}
}

void DisassemblerTab::render(sf::RenderWindow& window) {
	// hex
	drawBorder(window, getHexPosition(), sf::Vector2f(F4_HEXDUMP_WIDTH, F4_HEXDUMP_HEIGHT));
	float scroll_hex = drawHexDump(window, getHexPosition(), m_bytes, F4_HEX_BYTE_PER_LNE, F4_HEXDUMP_HEIGHT, m_cursor_pos, &m_line_offset);
	drawScrollBar(window, getHexPosition() + sf::Vector2f(F4_HEXDUMP_WIDTH, 0), F4_HEXDUMP_HEIGHT, scroll_hex);


	// disas
	drawBorder(window, getDisasPosition(), sf::Vector2f(F4_DISAS_WIDTH, F4_DISAS_HEIGHT));
	m_disassembler.disassemble();
	float scroll_disas = drawDisassembly(window, getDisasPosition(), m_disassembler.getInstructions(), sf::Vector2f(F4_DISAS_WIDTH, F4_DISAS_HEIGHT), m_cursor_pos/2);
	drawScrollBar(window, getDisasPosition() + sf::Vector2f(F4_DISAS_WIDTH, 0), F4_DISAS_HEIGHT, scroll_disas);


	// info
	Res::setTextColor(F4_HELP_TEXT_COLOR); 
	sf::RectangleShape box(sf::Vector2f(F4_INFO_WIDTH, FONT_SIZE)); box.setFillColor(COLOR_NONE); box.setOutlineThickness(BORDER_SIZE); box.setOutlineColor(BORDER_COLOR);
	Res::setTextPosition(getInfoPosition());  Res::setTextString("Ctrl-O : open file");
	box.setPosition(Res::getTextPosition());
	window.draw(Res::getText()); window.draw(box);

	Res::setTextPosition(getInfoPosition() + sf::Vector2f(0, FONT_SIZE+MARGIN/2)); Res::setTextString("Ctrl-S : save file");
	box.setPosition(Res::getTextPosition());
	window.draw(Res::getText()); window.draw(box);

	if (m_state == 1) {  // draw file navigation popup
		drawPopup(window, 1, m_error_msg);
	}
}