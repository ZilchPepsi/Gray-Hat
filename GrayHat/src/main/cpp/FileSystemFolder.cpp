#include "FileSystemFolder.h"
#include <stack>

FileSystemFolder::FileSystemFolder() : FileSystemObject(DEFAULT_DIR_NAME, TYPE_DIR){}

FileSystemFolder::FileSystemFolder(const std::string str, int typ) : FileSystemObject(str,typ){}

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
	folder->setParent(this);
	folders.push_back(folder);
}

void FileSystemFolder::addFile(std::string name, int type)
{
	FileSystemFile * file = new FileSystemFile(name, type);
	file->setParent(this);
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

std::vector<FileSystemFolder*> FileSystemFolder::getFolders() {
	return folders;
}

std::vector<FileSystemFile*> FileSystemFolder::getFiles() {
	return files;
}


void FileSystemFolder::resize() {
	std::stack<FileSystemObject*> stack;
	stack.push(this);

	size = 0;
	FileSystemFolder *fo;
	FileSystemObject* obj;
	while (!stack.empty()) {
		obj = stack.top();
		stack.pop();
		if ((fo = dynamic_cast<FileSystemFolder*>(obj)) != NULL) {

			int sum = 1;
			std::vector<FileSystemObject*>* contents = fo->getContents();
			for (std::vector<FileSystemObject*>::iterator it = contents->begin(); it != contents->end(); it++) {
				sum += (*it)->getSize();
			}
			delete contents;
			size += sum;
		}
		//this is a file, return the size of the file
		else {
			size += obj->getSize();
		}
	}
}


int FileSystemFolder::getSize() {
	if (size == 1) {
		resize();
	}

	return size;
}