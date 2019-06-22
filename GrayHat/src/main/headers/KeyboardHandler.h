#pragma once

#include <thread>
#include <mutex>
#include <vector>
#include "Logger.h"


#define KEYBOARD_RUNNING 1
#define KEYBOARD_STOPPED 0
#define KEY_DOWN 0x8000
#define KEYBOARD_BACKSPACE "B"
#define KEYBOARD_ENTER "E"
#define KEYBOARD_TAB "T"
#define KEYBOARD_UP "U"
#define KEYBOARD_DOWN "D"
#define KEYBOARD_LEFT "L"
#define KEYBOARD_RIGHT "R"


class KeyboardHandler
{
public:
	KeyboardHandler();
	~KeyboardHandler();


	void start();
	void stop();

	std::string getBuffer();

private:
	Logger logger;
	std::thread thread;
	volatile int state;
	std::string buffer;
	std::mutex bufferLock;

	char keyState[26];
	char arrowKeyState[4];
	char backspace = 0;
	char space = 0;
	char enter = 0;
	char period = 0;
	char underscore = 0;
	char tab = 0;

	void run();
	std::string getKeyBuffer();



};

