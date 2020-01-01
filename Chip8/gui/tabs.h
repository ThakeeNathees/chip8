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
	virtual std::string getTitle() = 0;
};

class HelpTab : public Tab
{
public:
	virtual void handleEvent(sf::Event& event) override {}
	virtual void process() override {}
	virtual void render() override {}
	virtual std::string getTitle() override { return std::string("Help (F1)"); }

};

class EmulatorTab : public Tab
{
public:
	virtual void handleEvent(sf::Event& event) override {}
	virtual void process() override {}
	virtual void render() override {}
	virtual std::string getTitle() override { return std::string("Emulator (F2)"); }
};

class AssemblerTab : public Tab
{
public:
	virtual void handleEvent(sf::Event& event) override {}
	virtual void process() override {}
	virtual void render() override {}
	virtual std::string getTitle() override { return std::string("Assembler (F3)"); }
};

class DisassemblerTab : public Tab
{
public:
	virtual void handleEvent(sf::Event& event) override {}
	virtual void process() override {}
	virtual void render() override {}
	virtual std::string getTitle() override { return std::string("Disssembler (F4)"); }
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

	}

	void render(sf::RenderWindow& window) {
		window.clear(BG_COLOR);
		drawTabBorder(window);
		drawHorizontalLine(window);
		drawTabTitle(window);
	}

	void setHelpTab() {
		m_current_tab = 0;
	}
	void setEmulatorTab() {
		m_current_tab = 1;
	}
	void setAssemblerTab() {
		m_current_tab = 2;
	}
	void setDisassemblerTab() {
		m_current_tab = 3;
	}

	int getCurrentTab() {
		return m_current_tab;
	}

private:
	sf::Font m_font;

	int m_current_tab;
	std::vector<Tab*> m_tabs;
	

private:
	void drawTabBorder( sf::RenderWindow& window) {
		for (int tab_no = 0; tab_no < m_tabs.size(); tab_no++) {
			drawVertexLine(window, sf::Vector2f(MARGIN + TAB_WIDTH * tab_no, MARGIN + TAB_HEIGHT), sf::Vector2f(MARGIN + TAB_WIDTH * tab_no, MARGIN));
			drawVertexLine(window, sf::Vector2f(MARGIN + TAB_WIDTH * tab_no, MARGIN), sf::Vector2f(MARGIN + TAB_WIDTH * (tab_no + 1), MARGIN));
			drawVertexLine(window, sf::Vector2f(MARGIN + TAB_WIDTH * (tab_no + 1), MARGIN), sf::Vector2f(MARGIN + TAB_WIDTH * (tab_no + 1), MARGIN + TAB_HEIGHT));
		}
	}
	void drawHorizontalLine(sf::RenderWindow& window) {
		drawVertexLine(window, sf::Vector2f(MARGIN, MARGIN + TAB_HEIGHT), sf::Vector2f(MARGIN + TAB_WIDTH * m_current_tab, MARGIN + TAB_HEIGHT));
		drawVertexLine(window, sf::Vector2f(MARGIN + TAB_WIDTH * (m_current_tab + 1), MARGIN + TAB_HEIGHT), sf::Vector2f(WINDOW_WIDTH - MARGIN, MARGIN + TAB_HEIGHT));
	}

	void drawTabTitle(sf::RenderWindow& window) {
		sf::Text title; title.setFont(m_font);
		title.setFillColor(TAB_TITLE_COLOR); title.setCharacterSize(10);
		for (int i = 0; i < m_tabs.size(); i++) {
			title.setString(m_tabs[i]->getTitle());
			title.setPosition(sf::Vector2f(MARGIN + i*TAB_WIDTH, MARGIN)+sf::Vector2f(MARGIN, MARGIN));
			window.draw(title);
		}
	}
};