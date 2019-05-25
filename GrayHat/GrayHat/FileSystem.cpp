#include "FileSystem.h"



FileSystem::FileSystem() : root("root",FileSystemObject::TYPE_ERR)
{
	
}


FileSystem::~FileSystem()
{
}

int FileSystem::generateSystem()
{

	return 0;
}

FileSystemObject* FileSystem::getRoot()
{
	return &root;
}

int FileSystem::treeSize(FileSystemObject)
{
	return 0;
}