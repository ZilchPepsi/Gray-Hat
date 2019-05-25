#pragma once
#include "KeyboardInput.h"
#include <Windows.h>
#include <thread>
#include <chrono>
#include <queue>

class KeyboardInput_win : public KeyboardInput
{
	public:
		KeyboardInput_win();
		~KeyboardInput_win();

		std::string getInputBuffer();
		std::string init();
		std::string clean();
		void run();
		void setMaxBufferSize(int l);
		std::string popBufferQueue();

private:

	void poll();
	void addToBuffer(char c);
	bool isCaps(int keycode);
	void moveCursor(int dir);
	void backspacePressed();
	void deletePressed();
	void enterPressed();

	std::string buffer;
	std::queue<std::string> prevBuffers;
	int cursorPos;

	std::thread pollingThread;
	bool running, reading;
	
	const static int numKeys = 50;
	bool curPressed[numKeys];
	bool prevPressed[numKeys];

	int MAX_BUFFER_LENGTH = 55;

	const static int IS_PRESSED = 0x8000;
	const static int IS_TOGGLED = 1;

	const static int KEY_0 = 0;
	const static int KEY_1 = 1;
	const static int KEY_2 = 2;
	const static int KEY_3 = 3;
	const static int KEY_4 = 4;
	const static int KEY_5 = 5;
	const static int KEY_6 = 6;
	const static int KEY_7 = 7;
	const static int KEY_8 = 8;
	const static int KEY_9 = 9;
	const static int KEY_A = 10;
	const static int KEY_B = 11;
	const static int KEY_C = 12;
	const static int KEY_D = 13;
	const static int KEY_E = 14;
	const static int KEY_F = 15;
	const static int KEY_G = 16;
	const static int KEY_H = 17;
	const static int KEY_I = 18;
	const static int KEY_J = 19;
	const static int KEY_K = 20;
	const static int KEY_L = 21;
	const static int KEY_M = 22;
	const static int KEY_N = 23;
	const static int KEY_O = 24;
	const static int KEY_P = 25;
	const static int KEY_Q = 26;
	const static int KEY_R = 27;
	const static int KEY_S = 28;
	const static int KEY_T = 29;
	const static int KEY_U = 30;
	const static int KEY_V = 31;
	const static int KEY_W = 32;
	const static int KEY_X = 33;
	const static int KEY_Y = 34;
	const static int KEY_Z = 35;
	const static int KEY_SPACE = 36;
	const static int KEY_SHIFT = 37;
	const static int KEY_CTRL = 38;
	const static int KEY_TAB = 39;
	const static int KEY_BACK = 40;
	const static int KEY_ENTER = 41;
	const static int KEY_CAPS = 42;
	const static int KEY_DEL = 43;
	const static int KEY_UP = 44;
	const static int KEY_DOWN = 45;
	const static int KEY_RIGHT = 46;
	const static int KEY_LEFT = 47;
	
};

