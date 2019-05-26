#include "FileSystemFile.h"



FileSystemFile::FileSystemFile() : FileSystemObject(DEFAULT_FILE_NAME, TYPE_FILE_MISC){}

FileSystemFile::FileSystemFile(const std::string str, const int typ) : FileSystemObject(str, typ) {}

FileSystemFile::~FileSystemFile(){}
