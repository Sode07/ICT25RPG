#include <iostream>
#include <string>

#include "isomap.h"
#include "console.h"

extern bool console_running;

void konsoli()
{
	while (console_running)
	{
		std::string input;
		std::string komento;
		std::string kartta;
		std::getline(std::cin, input);
		size_t spacePos = input.find(' ');

		komento = input.substr(0, spacePos);

		kartta = input.substr(spacePos + 1);
		if (komento == "map") {
			load_map_from_file(kartta.c_str());
		}
	}
}
