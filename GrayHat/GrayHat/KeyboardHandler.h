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
	char backspace =0;
	char space = 0;
	char enter = 0;

	void run();
	std::string getKeyBuffer();



};

