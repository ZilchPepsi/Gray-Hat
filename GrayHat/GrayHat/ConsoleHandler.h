#pragma once

#ifndef DEFINE_CONSOLEV2_PROPERTIES
#define DEFINE_CONSOLEV2_PROPERTIES
#endif

#define WINDOW_DOWN 0
#define WINDOW_UP 1
#define ESC "\x1b"


#include <Windows.h>
#include <string>
#include "FileSystemFolder.h"
#include "KeyboardHandler.h"


//#include <wchar.h>
//#include <stdlib.h>
#include <stdio.h>


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

private:

	Logger logger;

	HWND windowHandle;
	RECT windowSize;
	HANDLE hOut;
	CONSOLE_SCREEN_BUFFER_INFO ScreenBufferInfo;

	FileSystemFolder* curDir;
	std::vector<FileSystemFile*>* playerInventory;

	std::string inputBuffer;

	KeyboardHandler kbh;

	int windowStatus;

	std::string itoa(int);
	bool enableVTMode();

	std::string getBorder();
	std::string getCurrentDir();
	std::string getInventory();
	std::string getKeyboardInput();
};