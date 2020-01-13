#include "include.h"
#include "tabs.h"

void AssemblerTab::process() {

}

void AssemblerTab::handleEvent(sf::Event& event) {

}

void AssemblerTab::render(sf::RenderWindow& window) {
	Res::setTextString("\n//TODO:");
	Res::setTextPosition(sf::Vector2f(MARGIN, MARGIN + TAB_HEIGHT + MARGIN));
	window.draw(Res::getText());
}