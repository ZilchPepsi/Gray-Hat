#include "FileSystemFile.h"



FileSystemFile::FileSystemFile() : FileSystemObject("default_filename",FileSystemObject::TYPE_FILE_DEFAULT){}

FileSystemFile::FileSystemFile(const std::string str, const int typ) : FileSystemObject(str, typ) {}

FileSystemFile::~FileSystemFile(){}
