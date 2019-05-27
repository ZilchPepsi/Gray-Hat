#include "Player.h"



Player::Player(){}

Player::~Player()
{
	for (int i = 0; i < inventory.size(); i++)
	{
		if(inventory[i] != NULL)
			delete inventory[i];
	}
	
}

void Player::addInventory(FileSystemFile * file)
{
	inventory.push_back(file);
}

void Player::addPrevCommand(std::string command)
{
	prevCommands.push_back(command);
}

int Player::getInventorySize()
{
	return (int)inventory.size();
}

int Player::getPrevCommandsSize()
{
	return (int)prevCommands.size();
}

std::string Player::getPrevCommand(int index)
{
	return prevCommands[index];
}

FileSystemFile * Player::getInventoryItem(int index)
{
	return inventory[index];
}

void Player::setLocation(FileSystemFolder * location)
{
	curLocation = location;
}

FileSystemFolder * Player::getLocation()
{
	return curLocation;
}

std::vector<FileSystemFile *> * Player::getInventory()
{
	return &inventory;
}