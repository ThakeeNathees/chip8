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
inline float drawHexDump(sf::RenderWindow& window, sf::Vector2f draw_pos, unsigned char bytes[], unsigned int bytes_per_line, float window_height, unsigned int cursor_pos = 0) 
{
	unsigned int line_spacing	= HEX_LINE_SPACEING;
	sf::Color cursor_color		= F4_CURSOR_COLOR;
	float byte_spacing			= HEX_BYTE_SPACEING;

	// draw bytes loop
	static unsigned int line_offset = 0;
	unsigned int total_lines = ROM_SIZE / bytes_per_line;
	unsigned int lines = (window_height - 2 * FONT_SIZE) / (FONT_SIZE + line_spacing);
	unsigned int cursor_line = cursor_pos / bytes_per_line;
	if (cursor_line >= lines + line_offset) line_offset = cursor_line - lines + 1;
	if (cursor_line < line_offset) line_offset = cursor_line;

			float scroll_p = (float)cursor_line / (float)total_lines;

	sf::Vector2f pos = draw_pos + sf::Vector2f(MARGIN, MARGIN);
	sf::RectangleShape cursor(sf::Vector2f(FONT_SIZE * 2, FONT_SIZE));
	cursor.setFillColor(cursor_color);

	// draw title offset
	Res::s_text.setFillColor(BYTES_COLOR);
	pos += sf::Vector2f(FONT_SIZE + byte_spacing * 4, 0);
	for (unsigned int i = 0; i < bytes_per_line; i++) {
		Res::s_text.setString(toHexString(i)); Res::s_text.setPosition(pos); window.draw(Res::s_text);
		pos += sf::Vector2f(FONT_SIZE + byte_spacing, 0);
	}
	// draw title border line
	drawVertexLine(window, sf::Vector2f(draw_pos.x + MARGIN, pos.y + FONT_SIZE + line_spacing * 2), sf::Vector2f(pos.x, pos.y + FONT_SIZE + line_spacing * 2), BORDER_COLOR);
	drawVertexLine(window, sf::Vector2f(draw_pos.x + FONT_SIZE + byte_spacing * 4, pos.y + FONT_SIZE + line_spacing * 2), sf::Vector2f(draw_pos.x + FONT_SIZE + byte_spacing * 4, draw_pos.y + window_height - MARGIN), BORDER_COLOR);
	pos = sf::Vector2f(draw_pos.x + MARGIN, pos.y + FONT_SIZE + line_spacing*3);

	for (unsigned int i = bytes_per_line * line_offset; i < lines * bytes_per_line + bytes_per_line * line_offset; i++) { // TODO: MAX LINE NO -> min(total lines, claculated)

		// draw address
		Res::s_text.setFillColor(BYTES_COLOR);
		if (i % bytes_per_line == 0) {
			Res::s_text.setString(toHexString(i, 4));
			Res::s_text.setPosition(pos);
			window.draw(Res::s_text);
			pos += sf::Vector2f(FONT_SIZE + byte_spacing * 4, 0);
		}

		// draw bytes
		Res::s_text.setString(toHexString(bytes[i]));
		Res::s_text.setPosition(pos);

		if (i == cursor_pos) { // draw cursor
			Res::s_text.setFillColor(SELECTED_BYTE_COLOR);
			cursor.setPosition(Res::s_text.getPosition());
			window.draw(cursor);
		}
		window.draw(Res::s_text);

		if (i % bytes_per_line == bytes_per_line - 1) {
			pos = sf::Vector2f(draw_pos.x + MARGIN, pos.y + FONT_SIZE + line_spacing);
		}
		else pos += sf::Vector2f(FONT_SIZE + byte_spacing, 0);
	}
	return scroll_p;
}
