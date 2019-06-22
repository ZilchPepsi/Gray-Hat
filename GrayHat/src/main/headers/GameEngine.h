#pragma once

#include "Player.h"
#include "FileSystem.h"
#include "FileSystemFile.h"
#include "ConsoleHandler.h"
#include <string>
#include <vector>
#include <algorithm>
#include <locale>

#define NUM_CMDS 4
#define CMD_MOVE "cd"
#define CMD_ENCRYPT "encrypt"
#define CMD_DECRYPT "decrypt"
#define CMD_CPY "cp"

class GameEngine
{
	public:
		GameEngine(ConsoleHandler*);
		~GameEngine();

		int init();
		void start();


		std::string executeCommand(std::string command);

	private:
		
		Logger logger;

		ConsoleHandler* ch;
		std::thread thread;

		FileSystem fs;
		Player player;

		//int state, optionsIndex;
		
		bool running;
		std::string currentBuffer;
		unsigned int testVar;

		void handleArrowKeys();
		void handleAutocomplete();
		int update();
		int mainLoop();

		std::string CMDS[NUM_CMDS] = {
			CMD_MOVE,
			CMD_ENCRYPT,
			CMD_DECRYPT,
			CMD_MOVE
		};
};

