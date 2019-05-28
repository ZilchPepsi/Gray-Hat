#pragma once

#include "FileSystemFolder.h"
#include "Logger.h"

#define MAX_TREE_HEIGHT 20
#define MAX_DIRS 3
#define MAX_CONTENTS 10

#define MIN_TREE_HEIGHT 5
#define MIN_CONTENTS 3
#define MIN_DIRS 1
#define MIN_ROOT_DIRS 2

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

