#pragma once

#include "FileSystemObject.h"

class FileSystem
{
	public:

		FileSystem();
		~FileSystem();

		int generateSystem();
		FileSystemObject* getRoot();
		int treeSize(FileSystemObject);
	
	private:
		
		FileSystemObject root;// ("placeholder_name", FileSystemObject::TYPE_ERR);

		const float PROB_SUBFOLDER = 0.1f;
		const float PROB_FILE_EXE = 0.1f;
		const float PROB_FILE_KEY = 0.1f;
		const float PROB_FILE_SYM = 0.1f;
		const float PROB_FILE_MISC = 0.1f;
};

