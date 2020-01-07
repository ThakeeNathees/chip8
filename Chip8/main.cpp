#include "linker.h"
#include "include.h"

#include "gui/tabs.h"

// static fields
sf::Font Res::s_font;
sf::Text Res::s_text;

int main()
{
	list_directory("e:/movies");
	
	Res::init();

	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Chip8", sf::Style::Titlebar | sf::Style::Close);


	TabView tab_view;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			tab_view.handleEvent(event);
			if (event.type == sf::Event::Closed)
				window.close();
		}
		tab_view.process();

		tab_view.render(window);

		tab_view.getEmulatorTab()->setPixel( rand()%64, rand() % 32, rand() % 2);

		window.display();
	}

	return 0;
}