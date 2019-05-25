#pragma once

#include "GameGraphics.h"
#include "KeyboardInput_win.h"
//#include "Player.h"
#include "FileSystem.h"
#include <string>
#include <vector>
#include <chrono>

class GameEngine
{
	public:
		GameEngine();
		~GameEngine();

		int init();
		int update();
		int mainLoop();

	private:
		
		GameGraphics gg;
		KeyboardInput_win ki;
		FileSystem fs;
		//std::vector<Player> players;
		
		bool running;
		std::string currentBuffer;
		int testVar;
};

