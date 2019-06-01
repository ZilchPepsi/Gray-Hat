#include "GameGraphics.h"

GameGraphics::GameGraphics() :logger("GameGraphics")
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

	//bufferText = "";
	running = true;
	editing = false;

	curFolder = NULL;

	inventory = NULL;

	state = STATE_MENU_MAIN;
	prevState = STATE_MENU_MAIN;
	
	optionSelected = 0;

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
		std::this_thread::sleep_for(std::chrono::milliseconds(15)); // don't overwhelm this thread
	}
}

/*
	Main draw method. Clear screen and re-draw all text
*/
void GameGraphics::render()
{
	//graphics.clearScreen();
	graphics.setCursorVisible(false);

	if (state != prevState)
		graphics.clearScreen();

	if (state == STATE_GAME)
	{
		drawPartitionLine();
		drawRunningProgs();
		drawBufferHist();
		drawCurrentFolder();
		drawInventory();
		drawBufferText();
	}
	else if (state == STATE_MENU_MAIN)
	{
		drawGrayHat();
		drawOptions();
	}
	
	graphics.setCursorVisible(true);
	prevState = state;
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

void GameGraphics::drawBufferHist()
{
	char bufferStr[62];
	int histRow = 1;
	for (int i = (int)bufferHistory.size() - 1; i >= 0; i--)
	{
		sprintf_s(bufferStr, "%-60s", bufferHistory[i].c_str());;
		graphics.writeText(bufferStr, CHAR_HEIGHT - 1 - histRow, (CHAR_WIDTH / 2) + 1, TerminalGraphics::CC_DEFAULT);
		histRow++;
	}
}

void GameGraphics::drawCurrentFolder()
{
	graphics.writeText("Current directory: ", 0, (CHAR_WIDTH / 2) + 1, TerminalGraphics::CC_FORE_CYN);
	char line[64];
	int contentsRow = 2;

	if (curFolder != NULL)
	{
		sprintf_s(line, "-> %-30s", curFolder->getName().c_str());
		graphics.writeText(line, contentsRow, (CHAR_WIDTH / 2) + 1, TerminalGraphics::CC_FORE_MAG);
		sprintf_s(line, "%28d", curFolder->getSize());
		graphics.writeText(line, contentsRow, (CHAR_WIDTH * 3 / 4), TerminalGraphics::CC_FORE_WHT);
		graphics.writeText("kB", contentsRow, (CHAR_WIDTH * 3 / 4) + 29, TerminalGraphics::CC_FORE_GRN);

		contentsRow++;

		if (curFolder->getParent() != NULL)
		{
			sprintf_s(line, "\t-> %-30s", "..");
			graphics.writeText(line, contentsRow, (CHAR_WIDTH / 2) + 1, TerminalGraphics::CC_FORE_YEL);
			sprintf_s(line, "%28s", "?");
			graphics.writeText(line, contentsRow, (CHAR_WIDTH * 3 / 4), TerminalGraphics::CC_FORE_YEL);
			graphics.writeText("kB", contentsRow, (CHAR_WIDTH * 3 / 4) + 29, TerminalGraphics::CC_FORE_GRN);
			contentsRow++;
		}

		std::vector<FileSystemObject *> * contents = curFolder->getContents();
		for (int i = 0; i < contents->size(); i++)
		{
			int contentColor = TerminalGraphics::CC_FORE_WHT;
			if (contents->at(i)->getType() == TYPE_DIR)
				contentColor = TerminalGraphics::CC_FORE_YEL;
			sprintf_s(line, "\t-> %-30s", contents->at(i)->getName().c_str());
			graphics.writeText(line, contentsRow + i, (CHAR_WIDTH / 2) + 1, contentColor);
			sprintf_s(line, "%28d", contents->at(i)->getSize());
			graphics.writeText(line, contentsRow + i, (CHAR_WIDTH * 3 / 4), contentColor);
			graphics.writeText("kB", contentsRow + i, (CHAR_WIDTH * 3 / 4) + 29, TerminalGraphics::CC_FORE_GRN);
		}
		contentsRow += (int)contents->size();
		delete contents;
	}

	for (int i = 0; i < MAX_CONTENTS + contentsRow; i++)
	{
		sprintf_s(line, "%-60s", " ");
		graphics.writeText(line, contentsRow + i, (CHAR_WIDTH / 2) + 1);
	}
}

void GameGraphics::drawInventory()
{
	graphics.writeText("Current Inventory: ", inventoryRow, (CHAR_WIDTH / 2) + 1, TerminalGraphics::CC_FORE_CYN);
	
	char line[64];

	for (size_t i = 0; i < maxInventorySize; i++)
	{
		if (inventory != NULL && i < inventory->size()) // draw inventory item
		{
			sprintf_s(line, "(%d) %-30s", (int)i, inventory->at(i)->getName().c_str());
			graphics.writeText(line, inventoryRow + (int)i + 1, (CHAR_WIDTH / 2) + 1, TerminalGraphics::CC_FORE_MAG);
		}
		else // overwrite prev lines / make blank
		{
			sprintf_s(line, "%-40s", " ");
			graphics.writeText(line, inventoryRow + (int)i + 1, (CHAR_WIDTH / 2) + 1, TerminalGraphics::CC_FORE_WHT);
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
	sprintf_s(bufferText, "%-37s", text.c_str());;
	editing = false;
}

void GameGraphics::addBufferHistory(std::string text)
{
	editing = true;
	bufferHistory.push_back(text);
	if (bufferHistory.size() > maxBufferHistLines)
	{
		bufferHistory.erase(bufferHistory.begin());
	}
	editing = false;
}

void GameGraphics::setCurrentFolder(FileSystemFolder * folder)
{
	curFolder = folder;
}

void GameGraphics::setCurrentInventory(std::vector<FileSystemFile *> * ptr)
{
	inventory = ptr;
}

void GameGraphics::drawGrayHat()
{
	graphics.writeText("           =================/------         ", titleRow + 0, titleCol);
	graphics.writeText("           |###############/      |         ", titleRow + 1, titleCol);
	graphics.writeText("           |##############/       |         ", titleRow + 2, titleCol);
	graphics.writeText("           |#############/        |         ", titleRow + 3, titleCol);
	graphics.writeText("           |############/         |         ", titleRow + 4, titleCol);
	graphics.writeText("           |###########/          |         ", titleRow + 5, titleCol);
	graphics.writeText("         =============/---------------      ", titleRow + 6, titleCol);
	graphics.writeText("       ___    ___    / ___                  ", titleRow + 7, titleCol);
	graphics.writeText("      |      |   |  / |   |  |   |          ", titleRow + 8, titleCol);
	graphics.writeText("      |  __  |___| /  |___|  |___|          ", titleRow + 9, titleCol);
	graphics.writeText("      |___|  |  \\ /   |   |     |           ", titleRow + 10, titleCol);
	graphics.writeText("                 /            ___   ____    ", titleRow + 11, titleCol);
	graphics.writeText("                /     |   |  |   |    |     ", titleRow + 12, titleCol);
	graphics.writeText("               /      |___|  |___|    |     ", titleRow + 13, titleCol);
	graphics.writeText("              /       |   |  |   |    |     ", titleRow + 14, titleCol);
}

void GameGraphics::drawOptions()
{
	graphics.writeText(" P1 v AI ", optionsRow,     optionsCol, TerminalGraphics::CC_FORE_CYN);
	graphics.writeText(" P1 v P2 ", optionsRow + 2, optionsCol, TerminalGraphics::CC_FORE_CYN);
	graphics.writeText("   Exit  ", optionsRow + 4, optionsCol, TerminalGraphics::CC_FORE_CYN);

	// draw selection brackets
	for (int i = 0; i < 3; i++)
	{
		if (i == optionSelected)
		{
			graphics.writeText("[", (i * 2) + optionsRow, optionsCol - 2, TerminalGraphics::CC_FORE_YEL);
			graphics.writeText("]", (i * 2) + optionsRow, optionsCol + 10, TerminalGraphics::CC_FORE_YEL);
		}
		else
		{
			graphics.writeText(" ", (i * 2) + optionsRow, optionsCol - 2);
			graphics.writeText(" ", (i * 2) + optionsRow, optionsCol + 10);
		}
	}
	
}

void GameGraphics::setOptionsIndex(int index)
{
	if (state == STATE_MENU_MAIN)
	{
		if (0 <= index && index <= 2)
			optionSelected = index;
	}
}

void GameGraphics::setGraphicsState(int s)
{
	state = s;
}