#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <cstdlib>



inline std::string toHexString(unsigned int value, unsigned int fill = 2) {
	std::string hexStr;
	std::stringstream sstream;
	sstream << std::setfill('0') << std::setw(fill) << std::hex << (unsigned int)value;
	hexStr = sstream.str();
	sstream.clear();
	return hexStr;
}

#define print(x) std::cout<< x << std::endl;
