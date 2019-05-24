// TerminalTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <chrono>
#include "GameGraphics.h"

int main()
{
	GameGraphics gg;

	gg.addProgram("test_program");
	gg.setProgramPercent("test program", 53);

	//gg.run();
	
	gg.addProgram("test_multithreading");
	for (int i=0; i<100000; i++)
	{
		gg.setProgramPercent("test_multithreading", i % 100);

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	system("pause");
	return 0;
}

