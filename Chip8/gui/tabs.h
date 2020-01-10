#pragma once

#include "include.h"

#include "utils.h"
#include "conf.h"

#include "Disassembler.h"
#include "Emulator.h"

class Tab
{
public:
	virtual void handleEvent(sf::Event& event) = 0;
	virtual void process() = 0;
	virtual void render(sf::RenderWindow& window) = 0;
	virtual std::string getTitle() = 0;
	int getTabState() { return m_state; }
protected:
	int m_state = 0; // 0 = normal , 1 = file popup window
};

class HelpTab : public Tab
{
public:
	virtual void handleEvent(sf::Event& event) override {}
	virtual void process() override {};
	virtual void render(sf::RenderWindow& window) override {}
	virtual std::string getTitle() override { return std::string("Help (F1)"); }

};

class EmulatorTab : public Tab
{
public:

	virtual void handleEvent(sf::Event& event);
	virtual void process() override;
	virtual void render(sf::RenderWindow& window) override;
	virtual std::string getTitle() override { return std::string("Emulator (F2)"); }

	void setGrid() { m_is_drawgrid = !m_is_drawgrid; }
	void setDispPixel(unsigned int x, unsigned y, bool on = true) { /* todo: assert range */
		m_emulator.setDispPixel(x, y, on);
	}
private:
	Emulator m_emulator;
	unsigned int m_cursor_pos = 0;

	unsigned int m_hex_cursor = 0;

	void drawGrid(sf::RenderWindow& window);
	sf::Vector2f getDispPosition();
	sf::Vector2f getDisasPosition();
	sf::Vector2f getHexPosition();
	sf::Vector2f getRegPosition();
	void drawPixels(sf::RenderWindow& window);
	bool m_is_drawgrid = true;
};

class AssemblerTab : public Tab
{
public:
	virtual void handleEvent(sf::Event& event) override {}
	virtual void process() override {}
	virtual void render(sf::RenderWindow& window) override {}
	virtual std::string getTitle() override { return std::string("Assembler (F3)"); }
};

class DisassemblerTab : public Tab
{
public:
	DisassemblerTab() {
		m_disassembler.setBytesArray(m_bytes);
	}

	virtual void handleEvent(sf::Event& event) override;
	virtual void process() override {}
	virtual void render(sf::RenderWindow& window) override;
	virtual std::string getTitle() override { return std::string("Disssembler (F4)"); }

private:
	std::string m_working_file = "";
	std::string m_error_msg = "";
	bool m_second_byte = false; // when press any key 0-9 a-f -> is the key first or second
	unsigned int m_cursor_pos = 0;
	unsigned int m_line_offset = 0;
	unsigned char m_bytes[ROM_SIZE] = {0};
	Disassembler m_disassembler;

	sf::Vector2f getHexPosition();
	sf::Vector2f getDisasPosition();
	sf::Vector2f getInfoPosition();
};

class TabView 
{
public:

	TabView() {
		m_font.loadFromFile(FONT_PATH);
		HelpTab*		  help_tab			= new HelpTab();
		EmulatorTab*	  emulator_tab		= new EmulatorTab();
		AssemblerTab*	  assembler_tab		= new AssemblerTab();
		DisassemblerTab*  disassembler_tab	= new DisassemblerTab();

		m_tabs.push_back(help_tab);
		m_tabs.push_back(emulator_tab);
		m_tabs.push_back(assembler_tab);
		m_tabs.push_back(disassembler_tab);

		m_current_tab = 0;
	}

	void handleEvent(sf::Event& event) {

		// change tab - keyboard
		if (event.type == sf::Event::KeyPressed && getCurrentTab()->getTabState() == 0 ) {
			if (event.key.code == sf::Keyboard::Key::Tab) setTab( m_current_tab+1 );
			if (event.key.code == sf::Keyboard::Key::F1) setHelpTab();
			if (event.key.code == sf::Keyboard::Key::F2) setEmulatorTab();
			if (event.key.code == sf::Keyboard::Key::F3) setAssemblerTab();
			if (event.key.code == sf::Keyboard::Key::F4) setDisassemblerTab();
		}
		getCurrentTab()->handleEvent(event);

		// change tab - mouse
		if (event.type == sf::Event::MouseButtonReleased && getCurrentTab()->getTabState() == 0) {
			if (event.mouseButton.button == sf::Mouse::Left) {
				int x = event.mouseButton.x, y = event.mouseButton.y;
				if (mouseposToTabNum(x, y) == 1) setHelpTab();
				if (mouseposToTabNum(x, y) == 2) setEmulatorTab();
				if (mouseposToTabNum(x, y) == 3) setAssemblerTab();
				if (mouseposToTabNum(x, y) == 4) setDisassemblerTab();
			}
		}
	}

	void process() {

	}

	void render(sf::RenderWindow& window) {
		window.clear(BG_COLOR);
		drawTabBorder(window);
		drawHorizontalLine(window);
		drawTabTitle(window);
		getCurrentTab()->render(window);
	}

	void setTab(unsigned int tab_no) {
		m_current_tab = tab_no % m_tabs.size();
	}

	void setHelpTab() { m_current_tab = 0; }
	void setEmulatorTab() { m_current_tab = 1; }
	void setAssemblerTab() { m_current_tab = 2; }
	void setDisassemblerTab() { m_current_tab = 3; }

	HelpTab* getHelpTab() { return  static_cast<HelpTab*>(m_tabs[0]); }
	EmulatorTab* getEmulatorTab() { return  static_cast<EmulatorTab*>(m_tabs[1]); }
	AssemblerTab* getAssemblerTab() { return  static_cast<AssemblerTab*>(m_tabs[2]); }
	DisassemblerTab* getDisassemblerTab() { return  static_cast<DisassemblerTab*>(m_tabs[3]); }


	Tab* getCurrentTab() {
		return m_tabs[m_current_tab];
	}

private:
	sf::Font m_font;
	int m_current_tab;
	std::vector<Tab*> m_tabs;
	

private:

	int mouseposToTabNum(int x, int y) { // returns -1 if not in any tab
		bool y_in_range = MARGIN <= y && y <= MARGIN + TAB_HEIGHT;
		if (!y_in_range) return -1;

		int mapped_x = (x - MARGIN) / TAB_WIDTH;
		if (mapped_x < 0 || mapped_x > 4) return -1;
		if (mapped_x < 1) return 1;
		if (mapped_x < 2) return 2;
		if (mapped_x < 3) return 3;
		if (mapped_x < 4) return 4;
		
		return -1;
	}

	void drawTabBorder( sf::RenderWindow& window) {
		for (int tab_no = 0; tab_no < m_tabs.size(); tab_no++) {
			drawVertexLine(window, sf::Vector2f(MARGIN + TAB_WIDTH * tab_no, MARGIN + TAB_HEIGHT), sf::Vector2f(MARGIN + TAB_WIDTH * tab_no, MARGIN), BORDER_COLOR);
			drawVertexLine(window, sf::Vector2f(MARGIN + TAB_WIDTH * tab_no, MARGIN), sf::Vector2f(MARGIN + TAB_WIDTH * (tab_no + 1), MARGIN), BORDER_COLOR);
			drawVertexLine(window, sf::Vector2f(MARGIN + TAB_WIDTH * (tab_no + 1), MARGIN), sf::Vector2f(MARGIN + TAB_WIDTH * (tab_no + 1), MARGIN + TAB_HEIGHT), BORDER_COLOR);
		}
	}
	void drawHorizontalLine(sf::RenderWindow& window) {
		drawVertexLine(window, sf::Vector2f(MARGIN, MARGIN + TAB_HEIGHT), sf::Vector2f(MARGIN + TAB_WIDTH * m_current_tab, MARGIN + TAB_HEIGHT),BORDER_COLOR);
		drawVertexLine(window, sf::Vector2f(MARGIN + TAB_WIDTH * (m_current_tab + 1), MARGIN + TAB_HEIGHT), sf::Vector2f(WINDOW_WIDTH - MARGIN, MARGIN + TAB_HEIGHT), BORDER_COLOR);
	}

	void drawTabTitle(sf::RenderWindow& window) {
		sf::Text title; title.setFont(m_font);
		title.setCharacterSize(TAB_TITLE_SIZE);
		for (int i = 0; i < m_tabs.size(); i++) {
			if (m_current_tab == i) title.setFillColor(TAB_SELECTED_TITLE_COLOR); 
			else title.setFillColor(TAB_TITLE_COLOR);
			title.setString(m_tabs[i]->getTitle());
			title.setPosition(sf::Vector2f(MARGIN + i*TAB_WIDTH, MARGIN)+sf::Vector2f(MARGIN, TAB_HEIGHT/2 - TAB_TITLE_SIZE/2));
			window.draw(title);
		}
	}
};