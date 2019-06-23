#include "GameEngine.h"

GameEngine::GameEngine(ConsoleHandler* ch):logger("GameEngine"),ch(ch)
{ 
	init();
}

GameEngine::~GameEngine(){}

int GameEngine::init()
{
	logger.log("game engine initializing");

	// init FileSystem
	logger.log("generating file system");
	fs.generateSystem();

	logger.log(fs.getKeyPath());
	logger.log(fs.getObjPath());
	logger.log(fs.getPEMPath());

	player.setLocation(fs.getRoot());


	ch->setPlayerInventory(player.getInventory());

	logger.log("game engine initialized");

	return 0;
}

int GameEngine::update()
{
	ch->setCurrentDirectory(player.getLocation());
	executeCommand(ch->getNextCommand());
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

std::vector<std::string>* GameEngine::parseArgs(std::string args){
	std::vector<std::string>* ret = new std::vector<std::string>();
	if(args.find(' ') == std::string::npos)
	{
		ret->push_back(args);
		return ret;
	}
	int pos = 0;	
	for(int x= 0; x< args.size(); x++){
		if(args[x] == ' ' || args[x] == '\t'){
			if(x > pos)
			{
				ret->push_back(args.substr(pos,x-pos));
			}
			while(x+1 < args.size() && (args[x+1] == ' ' || args[x+1] == '\t')){
				x++;
			}
			pos = x+1;
		}
	}
	return ret;
}

void GameEngine::executeCommand(std::string command)
{
	if(command.empty())
		return;

	std::string commandCode;
	std::vector<std::string>* args = NULL;
	if(command.find(" ") != std::string::npos){
		commandCode = command.substr(0, command.find_first_of(' '));
		args = parseArgs(command.substr(command.find_first_of(' ')+1));
	}else{
		commandCode = command;
	}

	if(commandCode == CMD_MOVE)
		cmd_move(args);
	else if(commandCode == CMD_CP)
		cmd_cp(args);
	else if(commandCode == CMD_ENCRYPT)
		cmd_encrypt(args);
	else if(commandCode == CMD_DECRYPT)
		cmd_decrypt(args);
	delete args;
}
void GameEngine::cmd_move(std::vector<std::string>* args){

		if(args->empty())
			return;
		std::string dirName = (*args)[0];

		if (dirName == "..")
		{
			if (player.getLocation()->getParent() != NULL)
			{
				player.setLocation(dynamic_cast<FileSystemFolder*>(player.getLocation()->getParent()));
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
					break;
				}
				else if (contents->at(i)->getType() == TYPE_FILE_SYM && contents->at(i)->getName() == dirName)
				{
					FileSystemObject * symTarget = (dynamic_cast<FileSystemFile*>(contents->at(i)))->getSymlink();
					if(symTarget->getType() == TYPE_DIR) // targeting a folder
						player.setLocation(dynamic_cast<FileSystemFolder*>(symTarget));
					else //targeting a file, jump to parent directory
						player.setLocation(dynamic_cast<FileSystemFolder*>(symTarget->getParent()));
					break;
				}
			}
			delete contents;
		}
}

void GameEngine::cmd_cp(std::vector<std::string>* args){

	std::string filename = (*args)[0];		
	std::vector<FileSystemObject *> * contents = player.getLocation()->getContents();
	for (size_t i = 0; i < contents->size(); i++)
	{
		if ((contents->at(i)->getType() == TYPE_FILE_EXE || contents->at(i)->getType() == TYPE_FILE_MISC) && contents->at(i)->getName() == filename)
		{
			player.addInventory(dynamic_cast<FileSystemFile*>(contents->at(i)));
			break;
		}
	}
	delete contents;
}

void GameEngine::cmd_encrypt(std::vector<std::string>*){
	//TODO
}

void GameEngine::cmd_decrypt(std::vector<std::string>*){
	//TODO
}