#include "FileSystemFolder.h"

FileSystemFolder::FileSystemFolder() : FileSystemObject(DEFAULT_DIR_NAME, TYPE_DIR){}

FileSystemFolder::FileSystemFolder(const std::string str, const int typ) : FileSystemObject(str,typ){}

FileSystemFolder::~FileSystemFolder()
{
	for (int i = 0; i < folders.size(); i++)
	{
		delete folders[i];
	}

	for (int i = 0; i < files.size(); i++)
	{
		delete files[i];
	}
}

int FileSystemFolder::encrypt(int val)
{
	// TODO: implement encryt
	int encryptDuration;

	if (val != 0)
		encryptDuration = size / val;
	else
		encryptDuration = size;
	
	return encryptDuration;
}

void FileSystemFolder::addSubFolder(std::string name, int type)
{
	FileSystemFolder * folder = new FileSystemFolder(name, type);
	folders.push_back(folder);
}

void FileSystemFolder::addFile(std::string name, int type)
{
	FileSystemFile * file = new FileSystemFile(name, type);
	files.push_back(file);
}

std::vector<FileSystemObject*>* FileSystemFolder::getContents() {
	std::vector<FileSystemObject*>* contents = new std::vector<FileSystemObject*>();

	for (std::vector<FileSystemFile*>::iterator it = files.begin(); it != files.end(); it++) {
		contents->push_back(*it);
	}
	for (std::vector<FileSystemFolder*>::iterator it = folders.begin(); it != folders.end(); it++) {
		contents->push_back(*it);
	}
	return contents;
}