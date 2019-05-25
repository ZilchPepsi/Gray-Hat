#include "GameEngine.h"



GameEngine::GameEngine()
{ 
	init();
}


GameEngine::~GameEngine(){}

int GameEngine::init()
{
	running = true;
	testVar = 0;

	gg.addProgram("test_program");
	gg.setProgramPercent("test program", 53);

	gg.addProgram("test_multithreading");

	// init FileSystem
	fs.generateSystem();

	return 0;
}

int GameEngine::update()
{
	gg.setProgramPercent("test_multithreading", testVar % 100);
	testVar++;

	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	//test keyboard input
	currentBuffer = ki.getInputBuffer();
	gg.setInputBuffer(currentBuffer);

	//add prev buffers
	std::string prevBuffer = ki.popBufferQueue();
	if (prevBuffer != "")
		gg.addBufferHistory(prevBuffer);

	return 0;
}

int GameEngine::mainLoop()
{
	while (running)
	{
		update();

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	return 0;
}