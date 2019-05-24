#pragma once

#include <stdlib.h>
#include <string>

class KeyboardInput
{

	public:
		KeyboardInput();
		~KeyboardInput();

		std::string getInputBuffer();
		std::string init();
		std::string clean();
};

