#pragma once

#include "FileSystemObject.h"
#include "FileSystemFile.h"
#include <vector>

#define DEFAULT_DIR_NAME "util"

class FileSystemFolder : public FileSystemObject
{

	public:
		FileSystemFolder();
		FileSystemFolder(const std::string str, int typ);
		~FileSystemFolder();

		int encrypt(int val);

		void addSubFolder(std::string name, int type);
		void addFile(std::string name, int type);

		std::vector<FileSystemObject*>* getContents();
		
		std::vector<FileSystemFolder*> getFolders();
		std::vector<FileSystemFile*> getFiles();



	private:
		
		std::vector<FileSystemFile*> files;
		std::vector<FileSystemFolder*> folders;
};

