#pragma once

#ifndef DEFINE_CONSOLEV2_PROPERTIES
#define DEFINE_CONSOLEV2_PROPERTIES
#endif

#define ESC "\x1b"

//states
#define STATE_EXIT -1
#define STATE_MENU 0
#define STATE_GAME 1


#include <Windows.h>
#include <string>
#include "FileSystemFolder.h"
#include "KeyboardHandler.h"


//#include <wchar.h>
//#include <stdlib.h>
#include <stdio.h>
#include <queue>

class ConsoleHandler
{
public:
	ConsoleHandler();
	~ConsoleHandler();


	void start(HWND);
	int getYSize();
	int getXSize();

	void setCurrentDirectory(FileSystemFolder*);
	void setPlayerInventory(std::vector<FileSystemFile*>*);

	std::string getNextCommand();
	char getState();

private:

	Logger logger;

	HWND windowHandle;
	RECT windowSize;
	HANDLE hOut;
	CONSOLE_SCREEN_BUFFER_INFO ScreenBufferInfo;

	volatile char state = STATE_MENU;
	char menuSelection;
	const char* options[2] = {"play","quit"};
	int optionsLen = 2;

	FileSystemFolder* curDir;
	std::vector<FileSystemFile*>* playerInventory;


	std::queue<std::string> commandQueue;
	std::mutex queueLock;

	std::string inputBuffer;

	KeyboardHandler kbh;

	std::string itoa(int);
	bool enableVTMode();

	std::string getBorder(bool);
	std::string getCurrentDir();
	std::string getInventory();
	std::string getKeyboardInput_GAME_STATE();
	int         getKeyboardInput_MENU_STATE(bool*);
	std::string getMenuArt();
	std::string getMenuOptions();
	void handleTab();
	bool startsWith(std::string,std::string);
};