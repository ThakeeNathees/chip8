#include "linker.h"
#include "include.h"

#include "gui/tabs.h"





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

				if (event.key.code == sf::Keyboard::Key::G) tab_view.getEmulatorTab()->setGrid();
			}
			if (event.type == sf::Event::Closed)
				window.close();
		}

		tab_view.render(window);
		tab_view.getEmulatorTab()->setPixel( rand()%64, rand() % 32, rand() % 2);

		window.display();
	}

	return 0;
}