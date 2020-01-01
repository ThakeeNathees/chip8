#include "linker.h"


#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include "gui/tabs.h"

#include <iostream>
#include <cstdlib>



int main()
{
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Chip8");

	// sf::Font font;
	// font.loadFromFile("res/font/PressStart2P-Regular.ttf");

	TabView tab_view;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Key::F1) tab_view.setHelpTab();
				if (event.key.code == sf::Keyboard::Key::F2) tab_view.setEmulatorTab();
				if (event.key.code == sf::Keyboard::Key::F3) tab_view.setAssemblerTab();
				if (event.key.code == sf::Keyboard::Key::F4) tab_view.setDisassemblerTab();
			}
			if (event.type == sf::Event::Closed)
				window.close();
		}

		tab_view.render(window);

		window.display();
	}

	return 0;
}