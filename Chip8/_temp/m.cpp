#include "linker.h"


#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>


#include <cstdlib>
#include "Display.h"

#define BG_COLOR sf::Color(32,42,52)


int main()
{
	sf::RenderWindow window(sf::VideoMode( DISP_WIDTH * PIXEL_SIZE + VIEW_MARGIN.x * 3 + DAVIEW_WIDTH, 600), "SFML works!");
	
	// sf::Font font;
	// font.loadFromFile("res/font/PressStart2P-Regular.ttf");

	Display disp;
	DisasView disas_view;

	disas_view.setPosition(disp.getPosition().x + disp.getSize().x - VIEW_MARGIN.x, disp.getPosition().y );


	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		disp.setPixel(rand() % 64, rand() % 32, rand() % 2);
		
		window.clear(BG_COLOR);

		window.draw(disp.getView(true));
		//window.draw(disp.getBorder());

		window.draw(disas_view.getView());
		window.draw(disas_view.getBorder());

		window.display();
	}

	return 0;
}