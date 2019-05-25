#pragma once

#include "FileSystemObject.h"


class FileSystemFolder : public FileSystemObject
{

	public:
		FileSystemFolder();
		FileSystemFolder(std::string str, int typ);
		~FileSystemFolder();

		int encrypt(int val);

	private:
		

};

