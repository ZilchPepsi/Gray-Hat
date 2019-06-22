#include "FileSystemFile.h"



FileSystemFile::FileSystemFile() : FileSystemObject(DEFAULT_FILE_NAME, TYPE_FILE_MISC){}

FileSystemFile::FileSystemFile(const std::string str, const int typ) : FileSystemObject(str, typ) {}

FileSystemFile::~FileSystemFile(){}


FileSystemObject* FileSystemFile::getSymlink() {
	if (type == TYPE_FILE_SYM) {
		return symlink;
	}
	else {
		return NULL;
	}
}

int FileSystemFile::setSymLink(FileSystemObject* obj) {
	if (type == TYPE_FILE_SYM) {
		symlink = obj;
		return 1;
	}
	else {
		return 0;
	}
}

Executable* FileSystemFile::getExecutable() {
	if (type == TYPE_FILE_EXE) {
		return script;
	}
	else {
		return NULL;
	}
}

int FileSystemFile::setExecutable(Executable* e) {
	if (type == TYPE_FILE_EXE){
		script = e;
		return 1;
	}
	else {
		return 0;
	}
}

void FileSystemFile::changeExtension(std::string ex) {
	std::string subname = name.substr(0, name.find_last_of("."));
	name = subname + ex;
	displayName = subname + ex;
}