#include "GameEngine.h"

GameEngine::GameEngine(ConsoleHandler* ch):logger("GameEngine"),ch(ch)
{ 
	init();
}

GameEngine::~GameEngine(){}

int GameEngine::init()
{
	logger.log("game engine initializing");
	running = true;
	testVar = 0;
	//state = STATE_MENU_MAIN;
	//optionsIndex = 0;

	//gg.addProgram("test_program");
	//gg.setProgramPercent("test program", 53);

	//gg.addProgram("test_multithreading");

	logger.log("generating file system");
	// init FileSystem
	fs.generateSystem();

	logger.log(fs.getKeyPath());
	logger.log(fs.getObjPath());
	logger.log(fs.getPEMPath());

	player.setLocation(fs.getRoot());

	//gg.setGraphicsState(state);

	//gg.setCurrentInventory(player.getInventory());
	ch->setPlayerInventory(player.getInventory());

	logger.log("game engine initialized");

	return 0;
}

int GameEngine::update()
{
	if (ch->getState() == STATE_GAME)
	{

		//gg.setProgramPercent("test_multithreading", testVar);
		//testVar++;
		//testVar %= 100;
		//test keyboard input
		//currentBuffer = ki.getInputBuffer();
		//gg.setInputBuffer(currentBuffer);

		//add prev buffers
		//std::string prevBuffer = ki.popBufferQueue();
		//if (prevBuffer != "")
		//{
		//	// Execute command
		//	std::string retCMD = executeCommand(prevBuffer);

		//	player.addPrevCommand(prevBuffer);
		//	//gg.addBufferHistory(retCMD);
		//}

		//check for tab / autocomplete
		//if (ki.hasTabPressed())
		//{
		//	ki.resetTabbed();
		//	handleAutocomplete();
		//}

		// update current directory information
		//gg.setCurrentFolder(player.getLocation());

		ch->setCurrentDirectory(player.getLocation());
		executeCommand(ch->getNextCommand());
	}
	else if (ch->getState() == STATE_MENU)
	{
		/*handleArrowKeys();
		if (ki.hasEntered())
		{
			if (optionsIndex == 0)
			{
				state = STATE_GAME;
			}
			else if (optionsIndex == 1)
			{
				state = STATE_GAME;
			}
			else if (optionsIndex == 2)
			{
				running = false;
			}
			ki.resetEntered();
		}*/
	}
	//gg.setGraphicsState(state);
	return 0;
}

int GameEngine::mainLoop()
{
	while (ch->getState() != STATE_EXIT)
	{
		update();

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	return 0;
}

void GameEngine::start() {
	thread = std::thread(&GameEngine::mainLoop, this);
}

std::string GameEngine::executeCommand(std::string command)
{
	// TODO
	//extract lower case string up until 1st white space
	std::locale loc;
	std::string cmdLower = "";
	for (size_t i = 0; i < command.length(); i++)
		cmdLower += std::tolower(command[i], loc); // convert to lower case
	std::string commandCode = cmdLower.substr(0, cmdLower.find(' ')); // get string up until 1st whitespace

	if (commandCode == CMD_MOVE)
	{
		bool moved = false;
		//get string after 1st whitespace
		int whtspaceIndex = (int)(cmdLower.find(' ') + 1);
		std::string dirName = cmdLower.substr(whtspaceIndex, cmdLower.length() - whtspaceIndex);

		if (dirName == "..")
		{
			if (player.getLocation()->getParent() != NULL)
			{
				player.setLocation(dynamic_cast<FileSystemFolder*>(player.getLocation()->getParent()));
				moved = true;
			}
		}
		else
		{
			std::vector<FileSystemObject *> * contents = player.getLocation()->getContents();
			for (size_t i = 0; i < contents->size(); i++)
			{
				if (contents->at(i)->getType() == TYPE_DIR && contents->at(i)->getName() == dirName)
				{
					player.setLocation(dynamic_cast<FileSystemFolder*>(contents->at(i)));
					moved = true;
				}
				else if (contents->at(i)->getType() == TYPE_FILE_SYM && contents->at(i)->getName() == dirName)
				{
					FileSystemObject * symTarget = (dynamic_cast<FileSystemFile*>(contents->at(i)))->getSymlink();
					if(symTarget->getType() == TYPE_DIR) // targetting a folder
						player.setLocation(dynamic_cast<FileSystemFolder*>(symTarget));
					else //targetting a file, jump to parent directory
						player.setLocation(dynamic_cast<FileSystemFolder*>(symTarget->getParent()));
					moved = true;
				}
			}
			delete contents;
		}
		if(moved)
			return "Directory moved to: " + dirName;
		return "Invalid directory: " + dirName;
	}
	else if (commandCode == CMD_CPY)
	{
		logger.log("executing copy command: "+cmdLower);
		bool copied = false;
		//get string after 1st whitespace
		int whtspaceIndex = (int)(cmdLower.find(' ') + 1);
		std::string filename = cmdLower.substr(whtspaceIndex, cmdLower.length() - whtspaceIndex);
		
		std::vector<FileSystemObject *> * contents = player.getLocation()->getContents();
		for (size_t i = 0; i < contents->size(); i++)
		{
			if ((contents->at(i)->getType() == TYPE_FILE_EXE || contents->at(i)->getType() == TYPE_FILE_MISC) && contents->at(i)->getName() == filename)
			{
				player.addInventory(dynamic_cast<FileSystemFile*>(contents->at(i)));
				copied = true;
			}
		}
		delete contents;
		
		if (copied)
			return "File copied to inventory: " + filename;
		return "File not found: " + filename;
	}
	
	return "Invalid command: " + command;
}

void GameEngine::handleArrowKeys()
{
	//int action = ki.popArrowKeyQueue();
	/*while (action != ARR_KEY_NONE)
	{
		if (action == ARR_KEY_UP)
		{
			if (optionsIndex > 0)
				optionsIndex--;
		}
		else if (action == ARR_KEY_DOWN)
		{
			if (optionsIndex < 2)
				optionsIndex++;
		}
		action = ki.popArrowKeyQueue();
	}*/
	//gg.setOptionsIndex(optionsIndex);
}

void GameEngine::handleAutocomplete()
{
	/*std::string curBuffer = ki.getInputBuffer();
	std::string newBuffer = curBuffer;

	//check if no whitespace (still typing cmd)
	if (curBuffer.find(" ") == std::string::npos)
	{
		// identify CMD
		for (size_t i = 0; i < NUM_CMDS; i++)
		{
			if (CMDS[i].find(curBuffer) != std::string::npos)
			{
				newBuffer = CMDS[i] + " ";
			}
		}
	}
	else if (curBuffer.find(" ") < curBuffer.length() - 1) // first whitespace is before end of buffer string
	{
		std::string secStr = curBuffer.substr(curBuffer.find(" ") + 1, std::string::npos); // get from whitespace to end of str
		std::vector<FileSystemObject *> * contents = player.getLocation()->getContents();
		for (size_t i = 0; i < contents->size(); i++)
		{
			if (contents->at(i)->getName().find(curBuffer) != std::string::npos)
			{
				newBuffer = curBuffer.substr(0, curBuffer.length() - curBuffer.find(" ") + 1) + contents->at(i)->getName();
			}
		}
	}

	
	ki.setInputBuffer(newBuffer);
	*/
}
