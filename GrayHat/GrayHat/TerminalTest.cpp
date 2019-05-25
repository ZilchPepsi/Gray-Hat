// TerminalTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <chrono>
#include "GameGraphics.h"
#include "KeyboardInput_win.h"

/*int main()
{
	//unitTestTerminal();
	return 0;
}*/

int unitTestTerminal()
{
	GameGraphics gg;
	KeyboardInput_win ki;

	gg.addProgram("test_program");
	gg.setProgramPercent("test program", 53);

	std::string currentBuffer;

	gg.addProgram("test_multithreading");
	for (int i = 0; i < 100000; i++)
	{
		gg.setProgramPercent("test_multithreading", i % 100);

		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		//test keyboard input
		currentBuffer = ki.getInputBuffer();
		gg.setInputBuffer(currentBuffer);
	}

	system("pause");
	return 0;
}

