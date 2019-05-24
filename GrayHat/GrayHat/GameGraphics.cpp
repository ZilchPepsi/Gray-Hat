#include "GameGraphics.h"

GameGraphics::GameGraphics()
{
	init();
}

GameGraphics::~GameGraphics()
{
	running = false;
	if(renderer.joinable())
		renderer.join();

}

void GameGraphics::init()
{
	std::string retval = graphics.init();
	graphics.setScreenSize(SCRN_HEIGHT, SCRN_WIDTH);

	bufferText = "";
	running = true;
	editing = false;

	//graphics.writeText(retval, 0, 0);
	renderer = std::thread(&GameGraphics::run,this); // start rendering thread
}

void GameGraphics::run()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(100)); // give things a chance to instantiate

	while (running)
	{
		if(!editing)
			render();
		std::this_thread::sleep_for(std::chrono::milliseconds(10)); // don't overwhelm this thread
	}
}

/*
	Main draw method. Clear screen and re-draw all text
*/
void GameGraphics::render()
{
	//graphics.clearScreen();
	graphics.setCursorVisible(false);

	drawPartitionLine();
	drawRunningProgs();
	drawBufferText();

	graphics.setCursorVisible(true);
}

void GameGraphics::drawBufferText()
{
	// draw buffer
	graphics.writeText("> ", CHAR_HEIGHT - 1, (CHAR_WIDTH / 2) + 1, TerminalGraphics::CC_FORE_GRN);
	graphics.writeText(bufferText, CHAR_HEIGHT - 1, (CHAR_WIDTH / 2) + 3, TerminalGraphics::CC_DEFAULT);
}

void GameGraphics::drawPartitionLine()
{
	// draw middle partition line
	for (int row = 0; row < CHAR_HEIGHT; row++)
		graphics.writeText("|", row, CHAR_WIDTH / 2);
}

void GameGraphics::addProgram(std::string program)
{
	editing = true;
	curProgNames.push_back(program);
	curProgPercent.push_back(0);
	editing = false;
}

void GameGraphics::drawRunningProgs()
{
	//draw currently running programs
	graphics.writeText("Programs running:", curProgRow, 0, TerminalGraphics::CC_FORE_CYN);
	char nameStr[40];
	char percentStr[5];
	for (int i = 0; i < curProgNames.size(); i++)
	{
		if (curProgNames[i].length() < curProgPercentCol)
		{
			sprintf_s(percentStr, "%02d", curProgPercent[i]);
			sprintf_s(nameStr, "%-30s", curProgNames[i].c_str());
			graphics.writeText(nameStr, i + 2 + curProgRow, 3, TerminalGraphics::CC_DEFAULT);
			graphics.writeText("[", i + 2 + curProgRow, curProgPercentCol, TerminalGraphics::CC_DEFAULT);
			graphics.writeText(percentStr, i + 2 + curProgRow, curProgPercentCol + 1, TerminalGraphics::CC_FORE_GRN);
			graphics.writeText("%%", i + 2 + curProgRow, curProgPercentCol + 3, TerminalGraphics::CC_FORE_GRN);
			graphics.writeText("]", i + 2 + curProgRow, curProgPercentCol + 4, TerminalGraphics::CC_DEFAULT);
		}
	}
}

void GameGraphics::setProgramPercent(std::string program, int percent)
{
	editing = true;
	for (int i = 0; i < curProgNames.size(); i++)
	{
		if (curProgNames[i] == program)
		{
			curProgPercent[i] = percent;
		}
	}
	editing = false;
}

void GameGraphics::setInputBuffer(std::string text)
{
	editing = true;

	editing = false;
}

void GameGraphics::addBufferHistory(std::string text)
{
	editing = true;

	editing = false;
}