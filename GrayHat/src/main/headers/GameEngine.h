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
#define CMD_CP "cp"

class GameEngine
{
	public:
		GameEngine(ConsoleHandler*);
		~GameEngine();

		int init();
		void start();



	private:
		
		Logger logger;

		ConsoleHandler* ch;
		std::thread thread;

		FileSystem fs;
		Player player;

		int update();
		int mainLoop();
		void executeCommand(std::string);
		void cmd_move	(std::vector<std::string>*);
		void cmd_cp		(std::vector<std::string>*);
		void cmd_encrypt(std::vector<std::string>*);
		void cmd_decrypt(std::vector<std::string>*);
		std::vector<std::string>* parseArgs(std::string);

		std::string CMDS[NUM_CMDS] = {
			CMD_MOVE,
			CMD_ENCRYPT,
			CMD_DECRYPT,
			CMD_MOVE
		};
};

