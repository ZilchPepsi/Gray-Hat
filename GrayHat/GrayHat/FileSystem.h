#pragma once

#include "FileSystemFolder.h"
#include "Logger.h"

#define MAX_TREE_HEIGHT 20
#define MAX_DIRS 3
#define MAX_CONTENTS 6

#define MIN_TREE_HEIGHT 3
#define MIN_CONTENTS 2

#define MAX_KEYS 1

class FileSystem
{
	public:
		FileSystem();
		~FileSystem();

		int generateSystem();
		FileSystemFolder* getRoot();
		int treeSize(FileSystemObject*);
	
	private:
		FileSystemFolder root;

		Logger logger;
};

