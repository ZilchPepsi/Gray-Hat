#pragma once

#include "FileSystemObject.h"

class FileSystem
{
	public:

		FileSystem();
		~FileSystem();

		int generateSystem();
		FileSystemObject* getRoot();
		static int treeSize(FileSystemObject);
	
	private:
		
		FileSystemObject root;// ("placeholder_name", FileSystemObject::TYPE_ERR);
};

