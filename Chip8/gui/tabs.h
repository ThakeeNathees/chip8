#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include "utils.h"
#include "conf.h"


class Tab
{
public:
	virtual void handleEvent(sf::Event& event) = 0;
	virtual void process() = 0;
	virtual void render() = 0;
};

class HelpTab : public Tab
{
public:
	virtual void handleEvent(sf::Event& event) override {}
	virtual void process() override {}
	virtual void render() override {}

};

class EmulatorTab : public Tab
{
public:
	virtual void handleEvent(sf::Event& event) override {}
	virtual void process() override {}
	virtual void render() override {}
};

class AssemblerTab : public Tab
{
public:
	virtual void handleEvent(sf::Event& event) override {}
	virtual void process() override {}
	virtual void render() override {}
};

class DisassemblerTab : public Tab
{
public:
	virtual void handleEvent(sf::Event& event) override {}
	virtual void process() override {}
	virtual void render() override {}
};

class TabView 
{
public:

	TabView() {
		m_current_tab = &m_help_tab;
		m_font.loadFromFile(FONT_PATH);
	}

	void handleEvent(sf::Event& event) {

	}

	void render(sf::RenderWindow& window) {
		window.clear(BG_COLOR);
		drawTabBorder(window);

		int i = getTabIndex();
		drawVertexLine(window, sf::Vector2f(MARGIN, MARGIN + TAB_HEIGHT), sf::Vector2f(MARGIN + TAB_WIDTH * i, MARGIN + TAB_HEIGHT));
		drawVertexLine(window, sf::Vector2f(MARGIN + TAB_WIDTH * (i + 1), MARGIN + TAB_HEIGHT), sf::Vector2f(WINDOW_WIDTH - MARGIN, MARGIN + TAB_HEIGHT));

	}

	void setHelpTab() {
		m_current_tab = &m_help_tab;
	}
	void setEmulatorTab() {
		m_current_tab = &m_emulator_tab;
	}
	void setAssemblerTab() {
		m_current_tab = &m_assembler_tab;
	}
	void setDisassemblerTab() {
		m_current_tab = &m_disassembler_tab;
	}

	Tab& getCurrentTab() {
		return *m_current_tab;
	}

private:
	sf::Font m_font;

	Tab*			m_current_tab;
	HelpTab			m_help_tab;
	EmulatorTab		m_emulator_tab;
	AssemblerTab	m_assembler_tab;
	DisassemblerTab m_disassembler_tab;

private:
	int getTabIndex() {
		if (m_current_tab == &m_help_tab) return 0;
		else if (m_current_tab == &m_emulator_tab) return 1;
		else if (m_current_tab == &m_assembler_tab) return 2;
		else if (m_current_tab == &m_disassembler_tab) return 3;
		return 0;
		
	}
	void drawTabBorder( sf::RenderWindow& window) {
		for (int tab_no = 0; tab_no < 4; tab_no++) {
			drawVertexLine(window, sf::Vector2f(MARGIN + TAB_WIDTH * tab_no, MARGIN + TAB_HEIGHT), sf::Vector2f(MARGIN + TAB_WIDTH * tab_no, MARGIN));
			drawVertexLine(window, sf::Vector2f(MARGIN + TAB_WIDTH * tab_no, MARGIN), sf::Vector2f(MARGIN + TAB_WIDTH * (tab_no + 1), MARGIN));
			drawVertexLine(window, sf::Vector2f(MARGIN + TAB_WIDTH * (tab_no + 1), MARGIN), sf::Vector2f(MARGIN + TAB_WIDTH * (tab_no + 1), MARGIN + TAB_HEIGHT));
		}
	}
};