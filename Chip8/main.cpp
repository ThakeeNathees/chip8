#include "linker.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 500), "SFML works!");
	
	sf::Font font;
	font.loadFromFile("res/font/PressStart2P-Regular.ttf");
	
	/* test */
	sf::RectangleShape s(sf::Vector2f(100, 100));
	s.setFillColor(sf::Color(143, 145, 133));
	s.setPosition(sf::Vector2f(10, 10));
	
	float size = 15;
	sf::Vector2f pos(200, 20);

	sf::RectangleShape b(sf::Vector2f(300, size));
	b.setPosition(pos);
	b.setFillColor(sf::Color(0x4285ce));

	sf::Text t;
	t.setFont(font);
	t.setCharacterSize(size);
	t.setFillColor(sf::Color::White);
	t.setString("lda v3 %32\nlda v3 %32\nlda v3 %32");
	t.setPosition(pos);
	/* ************* */


	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(sf::Color(0x131b23));
		
		// test
		window.draw(s);
		window.draw(b);
		window.draw(t);
		/////

		window.display();
	}

	return 0;
}