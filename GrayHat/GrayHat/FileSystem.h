#pragma once

#include "FileSystemFolder.h"
#include "Logger.h"

#define MAX_TREE_HEIGHT 10
#define MAX_DIRS 4
#define MAX_CONTENTS 10

#define MIN_TREE_HEIGHT 5
#define MIN_CONTENTS 3
#define MIN_DIRS 2
#define MIN_ROOT_DIRS 2

#define MAX_KEYS 1

class FileSystem
{
	public:
		FileSystem();
		~FileSystem();

		int generateSystem();
		FileSystemFolder* getRoot();
		std::string getKeyPath();
		std::string getObjPath();
		std::string getPEMPath();
		std::string getFilePath(std::string);
		std::string getFilePath(const char*);
	
	private:
		FileSystemFolder root;

		Logger logger;
};

