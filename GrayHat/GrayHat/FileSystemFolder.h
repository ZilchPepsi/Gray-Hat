#pragma once

#include "FileSystemObject.h"
#include "FileSystemFile.h"
#include <vector>

class FileSystemFolder : public FileSystemObject
{

	public:
		FileSystemFolder();
		FileSystemFolder(std::string str, int typ);
		~FileSystemFolder();

		int encrypt(int val);

		void addSubFolder(std::string name, int type);
		void addFile(std::string name, int type);

	private:
		
		std::vector<FileSystemFile *> files;
		std::vector<FileSystemFolder *> folders;
};

