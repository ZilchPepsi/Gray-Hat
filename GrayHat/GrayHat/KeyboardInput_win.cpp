#include "KeyboardInput_win.h"


KeyboardInput_win::KeyboardInput_win()
{
	init();
}

KeyboardInput_win::~KeyboardInput_win()
{
	running = false;
	if (pollingThread.joinable())
	{
		pollingThread.join();
	}
}

std::string KeyboardInput_win::getInputBuffer()
{
	return buffer;
}

void KeyboardInput_win::setMaxBufferSize(int l)
{
	MAX_BUFFER_LENGTH = l;
}

std::string KeyboardInput_win::init()
{
	for (int i = 0; i < numKeys; i++)
	{
		curPressed[i] = false;
		prevPressed[i] = false;
	}
	cursorPos = 0;
	
	buffer = "";
	//prevBuffers.empty();

	//setup polling thread
	running = true;
	reading = false;
	pollingThread = std::thread(&KeyboardInput_win::run, this);

	return "KYBD INIT\n";
}

std::string KeyboardInput_win::clean()
{
	return "KYBD CLEAN\n";
}


void KeyboardInput_win::run()
{
	//setup loop for polling
	while (running) 
	{
		if (!reading)
		{
			poll();
		}
	}
}


bool KeyboardInput_win::isCaps(int keycode)
{
	if (curPressed[KEY_SHIFT] != curPressed[KEY_CAPS])
		return true;
	return false;
}


void KeyboardInput_win::addToBuffer(char c)
{
	if (buffer.length() < MAX_BUFFER_LENGTH)
	{
		buffer += c;
		//buffer.insert(cursorPos, std::to_string(c));
		cursorPos++;
	}
}

void KeyboardInput_win::moveCursor(int dir)
{
	cursorPos += dir;

	if (cursorPos > buffer.length())
		cursorPos = buffer.length();
	if (cursorPos < 0)
		cursorPos = 0;
}

void KeyboardInput_win::backspacePressed()
{
	if (cursorPos > 0)
	{
		buffer = buffer.substr(0, buffer.length() - 2);
		cursorPos--;
		/*if (cursorPos == 1)
		{
			buffer = buffer.substr(1, buffer.length() - 1);
		}
		else if (cursorPos == buffer.length())
		{
			buffer = buffer.substr(0, buffer.length() - 1);
		}
		else
		{
			//buffer = buffer.substr()
		}*/
	}
}

void KeyboardInput_win::deletePressed()
{
	if (cursorPos < buffer.length())
	{
		//buffer = buffer.substr(0,)
	}
}

void KeyboardInput_win::enterPressed()
{
	prevBuffers.push(buffer);
	buffer = "";
	cursorPos = 0;
}

std::string KeyboardInput_win::popBufferQueue()
{
	std::string retval = ""; 
	
	if (!(prevBuffers.empty()))
	{
		retval = prevBuffers.front();
		prevBuffers.pop();
	}

	return retval;
}

void KeyboardInput_win::poll()
{
	// check for special keys
	if (GetKeyState(VK_SHIFT) & IS_PRESSED)
	{
		curPressed[KEY_SHIFT] = true;
	}
	if (GetKeyState(VK_CONTROL) & IS_PRESSED)
	{
		curPressed[KEY_CTRL] = true;
	}
	if (GetKeyState(VK_CAPITAL) & IS_TOGGLED)
	{
		curPressed[KEY_CAPS] = true;
	}
	if (GetKeyState(VK_RETURN) & IS_PRESSED)
	{
		curPressed[KEY_ENTER] = true;
	}
	if (GetKeyState(VK_BACK) & IS_PRESSED)
	{
		curPressed[KEY_BACK] = true;
	}
	if (GetKeyState(VK_DELETE) & IS_PRESSED)
	{
		curPressed[KEY_DEL] = true;
	}
	if (GetKeyState(VK_SPACE) & IS_PRESSED)
	{
		curPressed[KEY_SPACE] = true;
	}

	// set number curPressed
	if (GetKeyState('0') & IS_PRESSED)
	{
		curPressed[KEY_0] = true;
	}
	if (GetKeyState('1') & IS_PRESSED)
	{
		curPressed[KEY_1] = true;
	}
	if (GetKeyState('2') & IS_PRESSED)
	{
		curPressed[KEY_2] = true;
	}
	if (GetKeyState('3') & IS_PRESSED)
	{
		curPressed[KEY_3] = true;
	}
	if (GetKeyState('4') & IS_PRESSED)
	{
		curPressed[KEY_4] = true;
	}
	if (GetKeyState('5') & IS_PRESSED)
	{
		curPressed[KEY_5] = true;
	}
	if (GetKeyState('6') & IS_PRESSED)
	{
		curPressed[KEY_6] = true;
	}
	if (GetKeyState('7') & IS_PRESSED)
	{
		curPressed[KEY_7] = true;
	}
	if (GetKeyState('8') & IS_PRESSED)
	{
		curPressed[KEY_8] = true;
	}
	if (GetKeyState('9') & IS_PRESSED)
	{
		curPressed[KEY_9] = true;
	}

	// check for letters keyPressed
	if (GetKeyState('A') & IS_PRESSED)
	{
		curPressed[KEY_A] = true;
	}
	if (GetKeyState('B') & IS_PRESSED)
	{
		curPressed[KEY_B] = true;
	}
	if (GetKeyState('C') & IS_PRESSED)
	{
		curPressed[KEY_C] = true;
	}
	if (GetKeyState('D') & IS_PRESSED)
	{
		curPressed[KEY_D] = true;
	}
	if (GetKeyState('E') & IS_PRESSED)
	{
		curPressed[KEY_E] = true;
	}
	if (GetKeyState('F') & IS_PRESSED)
	{
		curPressed[KEY_F] = true;
	}
	if (GetKeyState('G') & IS_PRESSED)
	{
		curPressed[KEY_G] = true;
	}
	if (GetKeyState('H') & IS_PRESSED)
	{
		curPressed[KEY_H] = true;
	}
	if (GetKeyState('I') & IS_PRESSED)
	{
		curPressed[KEY_I] = true;
	}
	if (GetKeyState('J') & IS_PRESSED)
	{
		curPressed[KEY_J] = true;
	}
	if (GetKeyState('K') & IS_PRESSED)
	{
		curPressed[KEY_K] = true;
	}
	if (GetKeyState('L') & IS_PRESSED)
	{
		curPressed[KEY_L] = true;
	}
	if (GetKeyState('M') & IS_PRESSED)
	{
		curPressed[KEY_M] = true;
	}
	if (GetKeyState('N') & IS_PRESSED)
	{
		curPressed[KEY_N] = true;
	}
	if (GetKeyState('O') & IS_PRESSED)
	{
		curPressed[KEY_O] = true;
	}
	if (GetKeyState('P') & IS_PRESSED)
	{
		curPressed[KEY_P] = true;
	}
	if (GetKeyState('Q') & IS_PRESSED)
	{
		curPressed[KEY_Q] = true;
	}
	if (GetKeyState('R') & IS_PRESSED)
	{
		curPressed[KEY_R] = true;
	}
	if (GetKeyState('S') & IS_PRESSED)
	{
		curPressed[KEY_S] = true;
	}
	if (GetKeyState('T') & IS_PRESSED)
	{
		curPressed[KEY_T] = true;
	}
	if (GetKeyState('U') & IS_PRESSED)
	{
		curPressed[KEY_U] = true;
	}
	if (GetKeyState('V') & IS_PRESSED)
	{
		curPressed[KEY_V] = true;
	}
	if (GetKeyState('W') & IS_PRESSED)
	{
		curPressed[KEY_W] = true;
	}
	if (GetKeyState('X') & IS_PRESSED)
	{
		curPressed[KEY_X] = true;
	}
	if (GetKeyState('Y') & IS_PRESSED)
	{
		curPressed[KEY_Y] = true;
	}
	if (GetKeyState('Z') & IS_PRESSED)
	{
		curPressed[KEY_Z] = true;
	}

	// add numbers, account for shift
	if (curPressed[KEY_0] && !(prevPressed[KEY_0]))
	{
		if (curPressed[KEY_SHIFT])
			addToBuffer(')');
		else
			addToBuffer('0');
	}
	if (curPressed[KEY_1] && !(prevPressed[KEY_1]))
	{
		if (curPressed[KEY_SHIFT])
			addToBuffer('!');
		else
			addToBuffer('1');
	}
	if (curPressed[KEY_2] && !(prevPressed[KEY_2]))
	{
		if (curPressed[KEY_SHIFT])
			addToBuffer('@');
		else
			addToBuffer('2');
	}
	if (curPressed[KEY_3] && !(prevPressed[KEY_3]))
	{
		if (curPressed[KEY_SHIFT])
			addToBuffer('#');
		else
			addToBuffer('3');
	}
	if (curPressed[KEY_4] && !(prevPressed[KEY_4]))
	{
		if (curPressed[KEY_SHIFT])
			addToBuffer('$');
		else
			addToBuffer('4');
	}
	if (curPressed[KEY_5] && !(prevPressed[KEY_5]))
	{
		if (curPressed[KEY_SHIFT])
			addToBuffer('%');
		else
			addToBuffer('5');
	}
	if (curPressed[KEY_6] && !(prevPressed[KEY_6]))
	{
		if (curPressed[KEY_SHIFT])
			addToBuffer('^');
		else
			addToBuffer('6');
	}
	if (curPressed[KEY_7] && !(prevPressed[KEY_7]))
	{
		if (curPressed[KEY_SHIFT])
			addToBuffer('&');
		else
			addToBuffer('7');
	}
	if (curPressed[KEY_8] && !(prevPressed[KEY_8]))
	{
		if (curPressed[KEY_SHIFT])
			addToBuffer('*');
		else
			addToBuffer('8');
	}
	if (curPressed[KEY_9] && !(prevPressed[KEY_9]))
	{
		if (curPressed[KEY_SHIFT])
			addToBuffer('(');
		else
			addToBuffer('9');
	}

	// add letters to buffer
	if (curPressed[KEY_A] && !(prevPressed[KEY_A]))
	{
		if (isCaps(KEY_A))
			addToBuffer('A');
		else
			addToBuffer('a');
	}
	if (curPressed[KEY_B] && !(prevPressed[KEY_B]))
	{
		if (isCaps(KEY_B))
			addToBuffer('B');
		else
			addToBuffer('b');
	}
	if (curPressed[KEY_C] && !(prevPressed[KEY_C]))
	{
		if (isCaps(KEY_C))
			addToBuffer('C');
		else
			addToBuffer('c');
	}
	if (curPressed[KEY_D] && !(prevPressed[KEY_D]))
	{
		if (isCaps(KEY_D))
			addToBuffer('D');
		else
			addToBuffer('d');
	}
	if (curPressed[KEY_E] && !(prevPressed[KEY_E]))
	{
		if (isCaps(KEY_E))
			addToBuffer('E');
		else
			addToBuffer('e');
	}
	if (curPressed[KEY_F] && !(prevPressed[KEY_F]))
	{
		if (isCaps(KEY_F))
			addToBuffer('F');
		else
			addToBuffer('f');
	}
	if (curPressed[KEY_G] && !(prevPressed[KEY_G]))
	{
		if (isCaps(KEY_G))
			addToBuffer('G');
		else
			addToBuffer('g');
	}
	if (curPressed[KEY_H] && !(prevPressed[KEY_H]))
	{
		if (isCaps(KEY_H))
			addToBuffer('H');
		else
			addToBuffer('h');
	}
	if (curPressed[KEY_I] && !(prevPressed[KEY_I]))
	{
		if (isCaps(KEY_I))
			addToBuffer('I');
		else
			addToBuffer('i');
	}
	if (curPressed[KEY_J] && !(prevPressed[KEY_J]))
	{
		if (isCaps(KEY_J))
			addToBuffer('J');
		else
			addToBuffer('j');
	}
	if (curPressed[KEY_K] && !(prevPressed[KEY_K]))
	{
		if (isCaps(KEY_K))
			addToBuffer('K');
		else
			addToBuffer('k');
	}
	if (curPressed[KEY_L] && !(prevPressed[KEY_L]))
	{
		if (isCaps(KEY_L))
			addToBuffer('L');
		else
			addToBuffer('l');
	}
	if (curPressed[KEY_M] && !(prevPressed[KEY_M]))
	{
		if (isCaps(KEY_M))
			addToBuffer('M');
		else
			addToBuffer('m');
	}
	if (curPressed[KEY_N] && !(prevPressed[KEY_N]))
	{
		if (isCaps(KEY_N))
			addToBuffer('N');
		else
			addToBuffer('n');
	}
	if (curPressed[KEY_O] && !(prevPressed[KEY_O]))
	{
		if (isCaps(KEY_O))
			addToBuffer('O');
		else
			addToBuffer('o');
	}
	if (curPressed[KEY_P] && !(prevPressed[KEY_P]))
	{
		if (isCaps(KEY_P))
			addToBuffer('P');
		else
			addToBuffer('p');
	}
	if (curPressed[KEY_Q] && !(prevPressed[KEY_Q]))
	{
		if (isCaps(KEY_Q))
			addToBuffer('Q');
		else
			addToBuffer('q');
	}
	if (curPressed[KEY_R] && !(prevPressed[KEY_R]))
	{
		if (isCaps(KEY_R))
			addToBuffer('R');
		else
			addToBuffer('r');
	}
	if (curPressed[KEY_S] && !(prevPressed[KEY_S]))
	{
		if (isCaps(KEY_S))
			addToBuffer('S');
		else
			addToBuffer('s');
	}
	if (curPressed[KEY_T] && !(prevPressed[KEY_T]))
	{
		if (isCaps(KEY_T))
			addToBuffer('T');
		else
			addToBuffer('t');
	}
	if (curPressed[KEY_U] && !(prevPressed[KEY_U]))
	{
		if (isCaps(KEY_U))
			addToBuffer('U');
		else
			addToBuffer('u');
	}
	if (curPressed[KEY_V] && !(prevPressed[KEY_V]))
	{
		if (isCaps(KEY_V))
			addToBuffer('V');
		else
			addToBuffer('v');
	}
	if (curPressed[KEY_W] && !(prevPressed[KEY_W]))
	{
		if (isCaps(KEY_W))
			addToBuffer('W');
		else
			addToBuffer('w');
	}
	if (curPressed[KEY_X] && !(prevPressed[KEY_X]))
	{
		if (isCaps(KEY_X))
			addToBuffer('X');
		else
			addToBuffer('x');
	}
	if (curPressed[KEY_Y] && !(prevPressed[KEY_Y]))
	{
		if (isCaps(KEY_Y))
			addToBuffer('Y');
		else
			addToBuffer('y');
	}
	if (curPressed[KEY_Z] && !(prevPressed[KEY_Z]))
	{
		if (isCaps(KEY_Z))
			addToBuffer('Z');
		else
			addToBuffer('z');
	}

	// unique actions
	if (curPressed[KEY_BACK] && !(prevPressed[KEY_BACK]))
	{
		backspacePressed();
	}
	if (curPressed[KEY_SPACE] && !(prevPressed[KEY_SPACE]))
	{
		addToBuffer(' ');
	}
	if (curPressed[KEY_ENTER] && !(prevPressed[KEY_ENTER]))
	{
		enterPressed();
	}

	//push cur to prev and clear cur
	for (int i = 0; i < numKeys; i++)
	{
		prevPressed[i] = false;
		prevPressed[i] = curPressed[i];
		curPressed[i] = false;
	}
}

