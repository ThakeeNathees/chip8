#include "tabs.h"

#define HZ_60 1/6 * 100000 


void EmulatorTab::handleEvent(sf::Event& event) {
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Key::G) setGrid();
	}

	// mouse scroll page up/down
	if (event.type == sf::Event::MouseWheelScrolled && m_state == 0) {
		if (event.mouseWheelScroll.delta > 0) {
			if (m_cursor_pos >= F2_PAGEUPDOWN_LINES)
				m_cursor_pos -= F2_PAGEUPDOWN_LINES;
			else m_cursor_pos = 0;
		}
		else {
			if (m_cursor_pos + F2_PAGEUPDOWN_LINES < ROM_SIZE)
				m_cursor_pos += F2_PAGEUPDOWN_LINES;
			else m_cursor_pos = ROM_SIZE - 1;
		}
	}

	if (event.type == sf::Event::KeyPressed) {
		// navigation
		if (event.key.code == sf::Keyboard::Up) {
			if (m_cursor_pos >= 2) m_cursor_pos-=2;
		}
		if (event.key.code == sf::Keyboard::Down) {
			if (m_cursor_pos < ROM_SIZE - 2) m_cursor_pos+=2;
		}
		if (event.key.code == sf::Keyboard::PageUp) {
			if (m_cursor_pos >= 10) m_cursor_pos -= F2_PAGEUPDOWN_LINES;
			else m_cursor_pos = 0;
		}
		if (event.key.code == sf::Keyboard::PageDown) {
			if (m_cursor_pos + 10 < ROM_SIZE ) m_cursor_pos += F2_PAGEUPDOWN_LINES;
			else m_cursor_pos = ROM_SIZE - 1;
		}
		if (event.key.code == sf::Keyboard::Home) {
			m_cursor_pos = PROGRAMME_OFFSET;
			m_line_offset = PROGRAMME_OFFSET;
		}
		if (event.key.code == sf::Keyboard::End) {
			m_cursor_pos = ROM_SIZE - 1;
		}

		// clock speed 
		static int speeds[] = { 3, 5, 10, 15, 30, 60, 120, 240, 480, 960, 2000, 4000 };
		static int speed_p = 4;
		if (event.key.code == sf::Keyboard::Equal) {
			if (speed_p < sizeof(speeds)/sizeof(int) - 1)
				m_hz = speeds[++speed_p];
		}
		if (event.key.code == sf::Keyboard::Hyphen) {
			if (speed_p > 0)
				m_hz = speeds[--speed_p];

		}

		// if enter step
		if (event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::F7) {
			m_cursor_pos  = m_emulator.step();
		}

		// b - break point
		if (event.key.code == sf::Keyboard::B) {
			auto pos  = m_break_points.find(m_cursor_pos);
			if (pos != m_break_points.end()) {
				m_break_points.erase(m_cursor_pos);
			}
			else  m_break_points.insert(m_cursor_pos);
		}

		// n - clear break points
		if (event.key.code == sf::Keyboard::N) {
			m_break_points.clear();
		}

		// if F5 run
		if (event.key.code == sf::Keyboard::F5) {
			if (! m_running)
				m_cursor_pos = m_emulator.step(); // for passing break point
			m_running = !m_running;
		}

		// restart
		if (event.key.code == sf::Keyboard::T) {
			m_running = false;
			m_cursor_pos = m_emulator.restart();
		}
		

		// open = o
		if (event.key.code == sf::Keyboard::O) {
			//if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ||
			//	sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
			//{
				std::string m_working_file = browse_file();
				if (m_working_file != std::string("")) {
					std::ifstream myFile(m_working_file, std::ios::in | std::ios::binary);
					myFile.seekg(0, std::ios_base::end);
					int file_size = myFile.tellg();

					myFile.seekg(0);
					if (!myFile.read((char*)(m_emulator.getBytesToWrite()), std::min(ROM_SIZE - PROGRAMME_OFFSET, file_size))) {
						// m_state = 1;
						// m_error_msg = std::string("Error reading file : ").append(m_working_file); // TODO: replace \ with /
					}

					m_cursor_pos = m_emulator.restart();
					m_break_points.clear();
					m_emulator.disassemble();
					m_running = false;
				}
			//}
		}
	}
}

void EmulatorTab::process() {

	static sf::Clock process_clock;
	static sf::Clock timer_clock;

	if (process_clock.getElapsedTime().asSeconds() > 1/float(m_hz) ) {
		process_clock.restart();
		if (m_running) {
			for (unsigned int b : m_break_points) {
				if (b == m_emulator.getPc()) {
					m_running = false;
					return;
				}
			}
			m_cursor_pos = m_emulator.step();
		}
	}
	
	if (timer_clock.getElapsedTime().asMicroseconds() > HZ_60) {
		timer_clock.restart();
		m_emulator.degreaseDt();
		m_emulator.degreaseSt();
	}
}


void EmulatorTab::render(sf::RenderWindow& window) {

	// display
	drawBorder(window, getDispPosition(), sf::Vector2f(F2_DISP_WIDTH, F2_DISP_HEIGHT), DISPLAY_BG_COLOR);
	drawPixels(window);
	if (m_is_drawgrid) drawGrid(window);


	// disas
	drawBorder(window, getDisasPosition(), sf::Vector2f(F2_DISAS_WIDTH, F2_DISAS_HEIGHT));
	float scroll_disas = drawDisassembly(window, getDisasPosition(), m_emulator.getInstructions(), sf::Vector2f(F2_DISAS_WIDTH, F2_DISAS_HEIGHT), m_cursor_pos / 2, m_emulator.getPc()/2, &m_line_offset, &m_break_points);
	drawScrollBar(window, getDisasPosition() + sf::Vector2f(F2_DISAS_WIDTH, 0), F2_DISAS_HEIGHT, scroll_disas);


	// hexdump
	drawBorder(window, getHexPosition(), sf::Vector2f(F2_HEXDUMP_WIDTH, F2_HEXDUMP_HEIGHT));
	float scroll_p = drawHexDump(window, getHexPosition(), m_emulator.getBytes(), F2_HEX_BYTE_PER_LNE, F2_HEXDUMP_HEIGHT,  m_cursor_pos);
	drawScrollBar(window, getHexPosition() + sf::Vector2f(F2_HEXDUMP_WIDTH, 0), F2_HEXDUMP_HEIGHT, scroll_p);

	// registers ////////////////////////////////////////////////////////////////////////////////
	drawBorder(window, getRegPosition(), sf::Vector2f(F2_REG_INFO_WIDTH, F2_REG_INFO_HEIGHT));
	Res::setTextColor(EMULATOR_STATE_TEXT_COLOR);

	// pc
	Res::setTextPosition( getRegPosition() + sf::Vector2f(MARGIN, MARGIN) );
	Res::setTextString(std::string("PC: ").append( toHexString(m_emulator.getPc(), 4)));
	window.draw(Res::getText());

	// I
	Res::setTextPosition(getRegPosition() + sf::Vector2f(MARGIN + 5* FONT_SIZE, MARGIN));
	Res::setTextString(std::string("I  : ").append(toHexString(m_emulator.getI(), 4)));
	window.draw(Res::getText());

	// sp
	Res::setTextPosition(getRegPosition() + sf::Vector2f(MARGIN + 10 * FONT_SIZE, MARGIN));
	Res::setTextString(std::string("SP: ").append(toHexString(m_emulator.getSp(), 1)));
	window.draw(Res::getText());

	// dt
	Res::setTextPosition(getRegPosition() + sf::Vector2f(MARGIN + 15 * FONT_SIZE, MARGIN));
	Res::setTextString(std::string("DT: ").append(toHexString(m_emulator.getDt(), 2)));
	window.draw(Res::getText());
	// st
	Res::setTextPosition(getRegPosition() + sf::Vector2f(MARGIN + 20 * FONT_SIZE, MARGIN));
	Res::setTextString(std::string("ST: ").append(toHexString(m_emulator.getSt(), 2)));
	window.draw(Res::getText());

	// Vx
	std::string registers_0_3 = "";
	std::string registers_4_7 = "";
	std::string registers_8_b = "";
	std::string registers_c_f = "";

	for (unsigned char x = 0x0; x <= 0x3; x++) {
		registers_0_3 = registers_0_3.append("V").append( toHexString(x, 1) ).append(": ").append(toHexString(m_emulator.getV(x), 2)).append("\n");
	}
	for (unsigned char x = 0x4; x <= 0x7; x++) {
		registers_4_7 = registers_4_7.append("V").append(toHexString(x, 1)).append(": ").append(toHexString(m_emulator.getV(x), 2)).append("\n");
	}
	for (unsigned char x = 0x8; x <= 0xb; x++) {
		registers_8_b = registers_8_b.append("V").append(toHexString(x, 1)).append(": ").append(toHexString(m_emulator.getV(x), 2)).append("\n");
	}
	for (unsigned char x = 0xc; x <= 0xf; x++) {
		registers_c_f = registers_c_f.append("V").append(toHexString(x, 1)).append(": ").append(toHexString(m_emulator.getV(x), 2)).append("\n");
	}

	Res::setTextString(registers_0_3);
	Res::setTextPosition(getRegPosition() + sf::Vector2f(MARGIN, F2_REGISTER_LINE_SPACE));
	window.draw(Res::getText());

	Res::setTextString(registers_4_7);
	Res::setTextPosition(getRegPosition() + sf::Vector2f(MARGIN + 5* FONT_SIZE, F2_REGISTER_LINE_SPACE));
	window.draw(Res::getText());

	Res::setTextString(registers_8_b);
	Res::setTextPosition(getRegPosition() + sf::Vector2f(MARGIN + 10 * FONT_SIZE, F2_REGISTER_LINE_SPACE));
	window.draw(Res::getText());

	Res::setTextString(registers_c_f);
	Res::setTextPosition(getRegPosition() + sf::Vector2f(MARGIN + 15 * FONT_SIZE, F2_REGISTER_LINE_SPACE));
	window.draw(Res::getText());

	Res::setTextString(std::string("Hz: ").append(std::to_string(m_hz)));
	Res::setTextPosition(getRegPosition() + sf::Vector2f(MARGIN + 20 * FONT_SIZE, F2_REGISTER_LINE_SPACE));
	window.draw(Res::getText());


	// stack
	std::string stack_0_3 = "";
	std::string stack_4_7 = "";
	std::string stack_8_b = "";
	std::string stack_c_f = "";

	for (unsigned char x = 0x0; x <= 0x3; x++) {
		stack_0_3 = stack_0_3.append(toHexString(x, 1)).append(": ").append(toHexString(m_emulator.getStack(x), 4)).append("\n");
	}
	for (unsigned char x = 0x4; x <= 0x7; x++) {
		stack_4_7 = stack_4_7.append(toHexString(x, 1)).append(": ").append(toHexString(m_emulator.getStack(x), 4)).append("\n");
	}
	for (unsigned char x = 0x8; x <= 0xb; x++) {
		stack_8_b = stack_8_b.append(toHexString(x, 1)).append(": ").append(toHexString(m_emulator.getStack(x), 4)).append("\n");
	}
	for (unsigned char x = 0xc; x <= 0xf; x++) {
		stack_c_f = stack_c_f.append(toHexString(x, 1)).append(": ").append(toHexString(m_emulator.getStack(x), 4)).append("\n");
	}

	Res::setTextString(stack_0_3);
	Res::setTextPosition(getRegPosition() + sf::Vector2f(MARGIN, F2_STACK_LINE_SPACE));
	window.draw(Res::getText());

	Res::setTextString(stack_4_7);
	Res::setTextPosition(getRegPosition() + sf::Vector2f(MARGIN + 5 * FONT_SIZE, F2_STACK_LINE_SPACE));
	window.draw(Res::getText());

	Res::setTextString(stack_8_b);
	Res::setTextPosition(getRegPosition() + sf::Vector2f(MARGIN + 10 * FONT_SIZE, F2_STACK_LINE_SPACE));
	window.draw(Res::getText());

	Res::setTextString(stack_c_f);
	Res::setTextPosition(getRegPosition() + sf::Vector2f(MARGIN + 15 * FONT_SIZE, F2_STACK_LINE_SPACE));
	window.draw(Res::getText());
	////////////////////////////////////////////////////////////////////////////////////////////////
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




