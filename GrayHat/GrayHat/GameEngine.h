#pragma once

#include "GameGraphics.h"
#include "KeyboardInput_win.h"
#include "Player.h"
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
		void executeCommand(std::string command);

	private:
		
		Logger logger;

		GameGraphics gg;
		KeyboardInput_win ki;
		FileSystem fs;
		Player player;
		
		bool running;
		std::string currentBuffer;
		int testVar;
};

