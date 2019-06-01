#pragma once

#include <stdlib.h>
#include <string>

#define ARR_KEY_NONE 0
#define ARR_KEY_UP 1
#define ARR_KEY_DOWN 2
#define ARR_KEY_LEFT 3
#define ARR_KEY_RIGHT 4

class KeyboardInput
{

	public:
		KeyboardInput();
		~KeyboardInput();

		std::string getInputBuffer();
		std::string init();
		std::string clean();
};

