#pragma once
#include <string>
#include <vector>
#include "FileSystemFile.h"
#include "FileSystemFolder.h"

class Player
{

	public:

		Player();
		~Player();

		void addInventory(FileSystemFile * file);
		void addPrevCommand(std::string command);

		int getInventorySize();
		int getPrevCommandsSize();

		std::string getPrevCommand(int index);
		FileSystemFile * getInventoryItem(int index);
		std::vector<FileSystemFile *> * getInventory();

		void setLocation(FileSystemFolder * location);
		FileSystemFolder * getLocation();

	private:
		
		std::vector<std::string> prevCommands;
		std::vector<FileSystemFile *> inventory;
		FileSystemFolder * curLocation;

		static int PLAYER_MAX_INVENTORY;
};

