#pragma once

#include "GameGraphics.h"
#include "KeyboardInput.h"
#include "KeyboardInput_win.h"
#include "Player.h"
#include "FileSystem.h"
#include "FileSystemFile.h"
#include <string>
#include <vector>
#include <chrono>
#include <algorithm>
#include <locale>

#define CMD_MOVE "cd"
#define CMD_ENCRYPT "encrypt"
#define CMD_DECRYPT "decrypt"
#define CMD_CPY "cpy"

class GameEngine
{
	public:
		GameEngine();
		~GameEngine();

		int init();
		int update();
		int mainLoop();
		std::string executeCommand(std::string command);

	private:
		
		Logger logger;

		GameGraphics gg;
		KeyboardInput_win ki;
		FileSystem fs;
		Player player;

		int state, optionsIndex;
		
		bool running;
		std::string currentBuffer;
		int testVar;

		void handleArrowKeys();
};

