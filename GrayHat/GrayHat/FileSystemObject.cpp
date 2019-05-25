#include "FileSystemObject.h"



FileSystemObject::FileSystemObject(const std::string str, int typ):name(str),type(typ)
{
	displayName = str;
	displayType = typ;
}


FileSystemObject::~FileSystemObject()
{

}

std::string FileSystemObject::getName()
{
	return name;
}

int FileSystemObject::getType()
{
	return type;
}

std::string FileSystemObject::getDisplayName()
{
	return name;
}

int FileSystemObject::getDisplayType()
{
	return displayType;
}

void FileSystemObject::setDisplayName(std::string str)
{
	displayName = str;
}

void FileSystemObject::setDisplayType(int typ)
{
	displayType = typ;
}

int FileSystemObject::encrypt(int val)
{
	if (val == 0)
		return size;
	return size / val;
}
