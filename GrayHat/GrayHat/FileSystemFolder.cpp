#include "FileSystemFolder.h"

FileSystemFolder::FileSystemFolder() : FileSystemObject("Yo_Change_This_Name", FileSystemObject::TYPE_FOLDER)
{
	
}

FileSystemFolder::FileSystemFolder(const std::string str, const int typ) : FileSystemObject(str,typ)
{
	
}

FileSystemFolder::~FileSystemFolder()
{

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
