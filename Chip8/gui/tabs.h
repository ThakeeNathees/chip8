#pragma once

#include "include.h"

#include "utils.h"
#include "conf.h"

class Tab
{
public:
	virtual void handleEvent(sf::Event& event) = 0;
	virtual void process() = 0;
	virtual void render(sf::RenderWindow& window) = 0;
	virtual std::string getTitle() = 0;

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
	void setPixel(unsigned int x, unsigned y, bool on = true) { /* todo: assert range */ m_pixels[y][x] = on; }
private:
	bool m_pixels[HEIGHT_PIX][WIDTH_PIX];
	unsigned int m_hex_cursor = 0;
	unsigned char m_hex_bytes[ROM_SIZE] = {0};

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
		for (int i = 0; i < ROM_SIZE; i++)
			m_bytes[i] = i;
		
	}
	virtual void handleEvent(sf::Event& event) override;
	virtual void process() override {}
	virtual void render(sf::RenderWindow& window) override;
	virtual std::string getTitle() override { return std::string("Disssembler (F4)"); }

private:
	int m_state = 0; // 0 = normal , 1 = file popup window
	bool m_second_byte = false; // when press any key 0-9 a-f -> is the key first or second
	unsigned int m_cursor_pos = 0;
	unsigned char m_bytes[ROM_SIZE] = {0};
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
		if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::Key::Tab) setTab( m_current_tab+1 );
			if (event.key.code == sf::Keyboard::Key::F1) setHelpTab();
			if (event.key.code == sf::Keyboard::Key::F2) setEmulatorTab();
			if (event.key.code == sf::Keyboard::Key::F3) setAssemblerTab();
			if (event.key.code == sf::Keyboard::Key::F4) setDisassemblerTab();
		}
		getCurrentTab()->handleEvent(event);
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