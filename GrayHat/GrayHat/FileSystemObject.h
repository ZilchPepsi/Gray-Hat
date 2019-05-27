#pragma once

#include <string>
#include "Logger.h"

#define TYPE_ERR -1
#define TYPE_DEVICE 0
#define TYPE_DIR 1
#define TYPE_FILE_MISC 2
#define TYPE_FILE_EXE 3
#define TYPE_FILE_SYM 4
#define TYPE_FILE_KEY 5
#define TYPE_FILE_OBJ 6


class FileSystemObject
{
public:
	FileSystemObject(std::string str, int typ);
	virtual ~FileSystemObject();

	virtual std::string getName();
	virtual int getType();
	virtual std::string getDisplayName();
	virtual int getDisplayType();
	virtual int getHeight();
	virtual int getSize();


	virtual void setName(std::string);
	virtual void setType(int);
	virtual void setDisplayName(std::string);
	virtual void setDisplayType(int);
	virtual void setHeight(int);
	virtual void setSize(int);

	virtual int encrypt(int val);

protected:
	Logger logger;
	std::string name;
	int type;
	std::string displayName;
	int displayType;
	int encryption;
	int size;
	int height;
};

