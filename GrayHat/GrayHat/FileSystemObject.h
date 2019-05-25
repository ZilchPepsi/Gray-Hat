#pragma once

#include <string>

class FileSystemObject
{
public:
	FileSystemObject(const std::string, const int);
	virtual ~FileSystemObject();

	virtual std::string* getName();
	virtual int getType();
	virtual std::string* getDisplayName();
	virtual int getDisplayType();


	virtual void setDisplayName(std::string*);
	virtual void setDisplayType(int);

	virtual int encrypt(int);

protected:
	const std::string name;
	const int type;
	std::string displayName;
	std::string displayType;
	int encryption;
	int size;
};

