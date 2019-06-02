#pragma once

#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <stdio.h>
#include <mutex>
#include "TerminalGraphics_win.h"
#include "FileSystemFolder.h"
#include "FileSystemFile.h"
#include "Logger.h"
#include "FileSystem.h"

#define STATE_GAME 0
#define STATE_MENU_MAIN 1

class GameGraphics
{
	public:

		GameGraphics();
		~GameGraphics();

		void init();

		void addProgram(std::string program);
		void setProgramPercent(std::string program, int percent);
		void setInputBuffer(std::string text);
		void addBufferHistory(std::string text);
		void setCurrentFolder(FileSystemFolder * folder);
		void setCurrentInventory(std::vector<FileSystemFile *> * ptr);
		void setOptionsIndex(int index);
		void setGraphicsState(int s);

		void render();
		void run();

	private:

		//logger
		Logger logger;

		std::mutex programLock;
		std::vector<std::string> curProgNames;
		std::vector<int> curProgPercent;

		char bufferText[40];
		//std::string bufferText;
		std::vector<std::string> bufferHistory;
		std::mutex bufferLock;

		FileSystemFolder * curFolder;

		TerminalGraphics_win graphics;

		std::vector<FileSystemFile *> * inventory;

		std::thread renderer;
		bool running = true;
		int state, prevState;

		int optionSelected = 0;

		int SCRN_WIDTH = 1000;
		int SCRN_HEIGHT = 600;
		int CHAR_WIDTH = 120;
		int CHAR_HEIGHT = 36;

		int curProgRow = 0;
		int curProgPercentCol = (CHAR_WIDTH / 2) - 6;
		int curBufferRow = CHAR_HEIGHT - 1;
		int bufferHistoryRow = curBufferRow - 1;
		int maxBufferHistLines = 5;
		int maxInventorySize = 7;
		int inventoryRow = 13;
		int titleRow = 6;
		int titleCol = 40;
		int optionsRow = titleRow + 18;
		int optionsCol = titleCol + 18;

		// render helper functions
		// -----------------------
		void drawPartitionLine();
		void drawRunningProgs();
		void drawBufferText();
		void drawBufferHist();
		void drawCurrentFolder();
		void drawInventory();
		// --------------------
		void drawGrayHat();
		void drawOptions();
};

