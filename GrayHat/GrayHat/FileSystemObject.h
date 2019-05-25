#pragma once

#include <string>

class FileSystemObject
{
public:
	FileSystemObject(const std::string str, int typ);
	virtual ~FileSystemObject();

	virtual std::string getName();
	virtual int getType();
	virtual std::string getDisplayName();
	virtual int getDisplayType();


	virtual void setDisplayName(std::string str);
	virtual void setDisplayType(int typ);

	virtual int encrypt(int val);

	// Types
	static const int TYPE_ERR = -1;
	static const int TYPE_DEVICE = 0;
	static const int TYPE_FOLDER = 1;
	static const int TYPE_FILE_DEFAULT = 2;
	static const int TYEP_FILE_EXE = 3;
	static const int TYPE_FILE_SYM = 4;
	static const int TYPE_FILE_KEY = 5;
	static const int TYPE_FILE_MISC = 6;


protected:
	const std::string name;
	const int type;
	std::string displayName;
	int displayType;
	int encryption;
	int size;
};

