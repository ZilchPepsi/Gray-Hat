#include "KeyboardHandler.h"

#include <windows.h>
//#include <WinUser.h>


KeyboardHandler::KeyboardHandler() : logger("KeyboardHandler")
{
}


KeyboardHandler::~KeyboardHandler()
{
}

void KeyboardHandler::start() {

	thread = std::thread(&KeyboardHandler::run, this);
}

void KeyboardHandler::stop() {
	state = KEYBOARD_STOPPED;
}

void KeyboardHandler::run() {
	state = KEYBOARD_RUNNING;

	while (state == KEYBOARD_RUNNING) {
		std::string input = getKeyBuffer();
		bufferLock.lock();
		buffer += input;
		bufferLock.unlock();

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

}

std::string KeyboardHandler::getBuffer() {
	bufferLock.lock();
	std::string s(buffer);
	buffer.clear();
	bufferLock.unlock();
	return s;
}

std::string KeyboardHandler::getKeyBuffer() {
	std::string ret;

	for (int key = 'A'; key <= 'Z'; key++) {
		if (GetKeyState(key) & KEY_DOWN) {
			if (!keyState[key - 'A']) {
				keyState[key - 'A'] = 1;
				if (GetKeyState(VK_SHIFT) & KEY_DOWN) {
					ret += key;
				}
				else {
					ret += (key + 32);
				}
			}
		}
		else {
			keyState[key - 'A'] = 0;
		}
	}

	if (GetKeyState(VK_BACK) & KEY_DOWN) {
		if (!backspace) {
			backspace = 1;
			ret += "#" KEYBOARD_BACKSPACE;
		}
	}
	else {
		backspace = 0;
	}

	if (GetKeyState(VK_SPACE) & KEY_DOWN) {
		if (!space) {
			space = 1;
			ret += ' ';
		}
	}
	else {
		space = 0;
	}

	if (GetKeyState(VK_RETURN) & KEY_DOWN) {
		if (!enter) {
			enter = 1;
			ret += "#" KEYBOARD_ENTER;
		}
	}
	else {
		enter = 0;
	}

	return ret;
}
