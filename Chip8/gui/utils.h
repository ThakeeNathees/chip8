#pragma once
#include "conf.h"
#include "include.h"

inline void drawVertexLine(sf::RenderTarget& render_target, sf::Vector2f point1, sf::Vector2f point2, sf::Color color = sf::Color::White) {
	sf::VertexArray line(sf::LinesStrip, 2);
	line[0].position = point1;
	line[0].color = color;
	line[1].position = point2;
	line[1].color = color;
	render_target.draw(line);
}

inline void drawVertexBox(sf::RenderTarget& rendet_target, sf::Vector2f point, sf::Vector2f size, sf::Color color = sf::Color::White) {
	drawVertexLine(rendet_target, sf::Vector2f(point.x, point.y), sf::Vector2f(point.x + size.x, point.y), color);
	drawVertexLine(rendet_target, sf::Vector2f(point.x + size.x, point.y), sf::Vector2f(point.x + size.x, point.y + size.y), color);
	drawVertexLine(rendet_target, sf::Vector2f(point.x + size.x, point.y + size.y), sf::Vector2f(point.x, point.y + size.y), color);
	drawVertexLine(rendet_target, sf::Vector2f(point.x, point.y + size.y), sf::Vector2f(point.x, point.y), color);
}

inline void drawBorder(sf::RenderTarget& render_target, sf::Vector2f position, sf::Vector2f size, sf::Color bg_color=COLOR_NONE) {
	sf::RectangleShape hex_border(size);
	hex_border.setPosition(position); hex_border.setFillColor(bg_color);
	hex_border.setOutlineThickness(BORDER_SIZE); hex_border.setOutlineColor(BORDER_COLOR);
	render_target.draw(hex_border);
}

// position - right top point , percentage (0, 1), scrbox top left = -2*MARGIN
inline void drawScrollBar(sf::RenderTarget& render_target, sf::Vector2f position, float height, float percentage) {

	static float scr_margin		= 5;
	static float scr_box_width  = 2 * MARGIN - scr_margin;
	static float scr_box_height = 4 * MARGIN;

	drawVertexLine(render_target, position + sf::Vector2f(-MARGIN - scr_margin/2, MARGIN), position + sf::Vector2f(-MARGIN - scr_margin/2, -MARGIN + height), BORDER_COLOR);
	sf::RectangleShape box(sf::Vector2f(scr_box_width, scr_box_height)); 
	box.setFillColor(BG_COLOR); box.setOutlineThickness(BORDER_SIZE); box.setOutlineColor(BORDER_COLOR);
	box.setPosition(position + sf::Vector2f(-2 * MARGIN , MARGIN + scr_margin + (height -2*MARGIN -scr_box_height - 2*scr_margin)*percentage));
	render_target.draw(box);
}


inline std::string toHexString(unsigned int value, unsigned int fill = 2) {
	std::string hexStr;
	std::stringstream sstream;
	sstream << std::setfill('0') << std::setw(fill) << std::hex << (unsigned int)value;
	hexStr = sstream.str();
	sstream.clear();
	return hexStr;
}