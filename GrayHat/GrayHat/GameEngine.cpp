#include "GameEngine.h"

GameEngine::GameEngine() :logger("GameEngine")
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

	player.setLocation(fs.getRoot());

	return 0;
}

int GameEngine::update()
{
	gg.setProgramPercent("test_multithreading", testVar % 100);
	testVar++;

	//std::this_thread::sleep_for(std::chrono::milliseconds(100));

	//test keyboard input
	currentBuffer = ki.getInputBuffer();
	gg.setInputBuffer(currentBuffer);

	//add prev buffers
	std::string prevBuffer = ki.popBufferQueue();
	if (prevBuffer != "")
	{
		// Execute command
		std::string retCMD = executeCommand(prevBuffer);

		player.addPrevCommand(prevBuffer);
		gg.addBufferHistory(retCMD);
	}

	// update current directory information
	gg.setCurrentFolder(player.getLocation());

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
		int whtspaceIndex = cmdLower.find(' ') + 1;
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
	
	return "Invalid command: " + command;
}