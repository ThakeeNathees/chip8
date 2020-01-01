#pragma once


#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include <assert.h>

#define COLOR_NONE sf::Color(0,0,0,0)
#define COLOR_BORDER sf::Color(200,200,200)

// main display
#define DISP_WIDTH  64
#define DISP_HEIGHT 32
#define PIXEL_SIZE  10
#define VIEW_MARGIN sf::Vector2f(10, 10)

#define PIX_COLOR sf::Color::Black
#define BG_COLOR  sf::Color(143, 143, 133)
#define GRID_COLOR sf::Color::Black

// disas display
#define DAVIEW_WIDTH 200
// height equ main disp height


class Display
{
private:
	sf::Sprite m_disp_sprite;
	sf::RenderTexture m_render_texture;

public:
	Display() {
		m_render_texture.create(DISP_WIDTH * PIXEL_SIZE, DISP_HEIGHT * PIXEL_SIZE);
		m_render_texture.clear(BG_COLOR);
		m_disp_sprite.move(VIEW_MARGIN);
	}

	void drawGrid() {
		sf::RectangleShape grid_box(sf::Vector2f(PIXEL_SIZE, PIXEL_SIZE));
		grid_box.setFillColor(sf::Color(0, 0, 0, 0));
		grid_box.setOutlineThickness(.5); grid_box.setOutlineColor(GRID_COLOR);
		for (int i = 0; i < DISP_WIDTH; i++) {
			for (int j = 0; j < DISP_HEIGHT; j++) {
				grid_box.setPosition(sf::Vector2f(i*PIXEL_SIZE, j*PIXEL_SIZE));
				m_render_texture.draw(grid_box);
			}
		}
	}

	void setPixel(unsigned int x, unsigned int y, bool is_white = true) {

		assert(x < DISP_WIDTH && y < DISP_HEIGHT && "at Display::setPixel x, y are out of range");

		sf::RectangleShape grid_box(sf::Vector2f(PIXEL_SIZE, PIXEL_SIZE));
		if (is_white) grid_box.setFillColor(PIX_COLOR);
		else grid_box.setFillColor(BG_COLOR);
		grid_box.setPosition( x * PIXEL_SIZE, y * PIXEL_SIZE);
		m_render_texture.draw(grid_box);

	}

	void setPosition(sf::Vector2f v) {
		setPosition(v.x, v.y);
	}
	void setPosition(float x, float y) {
		m_disp_sprite.setPosition(x, y);
		m_disp_sprite.move(VIEW_MARGIN);
	}

	sf::Vector2f getSize() {
		return sf::Vector2f(DISP_WIDTH * PIXEL_SIZE, DISP_HEIGHT * PIXEL_SIZE) + 2.f * VIEW_MARGIN;
	}
	sf::Vector2f getPosition() {
		return m_disp_sprite.getPosition() - VIEW_MARGIN;
	}
	sf::RectangleShape getBorder() {
		sf::RectangleShape border(sf::Vector2f(getSize() - 2.f * VIEW_MARGIN));
		border.setPosition(getPosition() + VIEW_MARGIN);
		border.setFillColor(COLOR_NONE);
		border.setOutlineThickness(1); border.setOutlineColor(COLOR_BORDER);
		return border;
	}

	sf::Sprite& getView(bool draw_grid = false) {
		if (draw_grid) drawGrid();
		m_render_texture.display();
		m_disp_sprite.setTexture(m_render_texture.getTexture(), false);
		return m_disp_sprite;
	}
};


class DisasView
{
private:
	sf::RenderTexture m_render_texture;
	sf::Sprite m_disp_sprite;

public:
	DisasView() {
		m_render_texture.create(DAVIEW_WIDTH, DISP_HEIGHT*PIXEL_SIZE);
		//m_render_texture.clear(BG_COLOR);
		m_disp_sprite.move(VIEW_MARGIN);
	}

	// setter
	void setPosition(float x, float y) {
		m_disp_sprite.setPosition(x, y);
		m_disp_sprite.move(VIEW_MARGIN);
	}

	// getter
	sf::Vector2f getSize() {
		return sf::Vector2f(DAVIEW_WIDTH, DISP_HEIGHT * PIXEL_SIZE) + 2.f * VIEW_MARGIN;
	}
	sf::Vector2f getPosition() {
		return m_disp_sprite.getPosition() - VIEW_MARGIN;
	}
	sf::RectangleShape getBorder() {
		sf::RectangleShape border(sf::Vector2f(getSize() - 2.f * VIEW_MARGIN));
		border.setPosition(getPosition() + VIEW_MARGIN);
		border.setFillColor(COLOR_NONE);
		border.setOutlineThickness(1); border.setOutlineColor(COLOR_BORDER);
		return border;
	}

	sf::Sprite& getView() {
		sf::Font font;
		font.loadFromFile("res/font/PressStart2P-Regular.ttf");
		
		float size = 14;
		sf::Text t; t.setFont(font); t.setCharacterSize(size); t.setFillColor(sf::Color::White);
		t.setString("00 2f 45 2a 3b \nbb 22 65 aa 25 \nc8 d4 44 29 0e ff");
		m_render_texture.draw(t);
		m_render_texture.display();
		m_disp_sprite.setTexture(m_render_texture.getTexture(), false);
		return m_disp_sprite;
	}
};