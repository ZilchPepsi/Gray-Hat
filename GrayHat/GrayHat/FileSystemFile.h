#pragma once
#include "FileSystemObject.h"
#include "Executable.h"

#define DEFAULT_FILE_NAME "config.ini"
#define DEFAULT_OBJ_NAME "secrets.txt"
#define DEFAULT_KEY_NAME "passwords.key"
#define DEFAULT_PEM_NAME "global.pem"

#define PROB_FILE_MISC 75.0f
#define PROB_FILE_SPEC 25.0f

#define PROB_FILE_OBJ 5.0f
#define PROB_FILE_EXESYM 95.0f

#define PROB_FILE_EXE 45.0f
#define PROB_FILE_SYM 55.0f

class FileSystemFile : public FileSystemObject
{

	public:
	
		FileSystemFile();
		FileSystemFile(std::string str, int typ);
		~FileSystemFile();

		FileSystemObject* getSymlink();
		int setSymLink(FileSystemObject*);

		Executable* getExecutable();
		int setExecutable(Executable*);
	
	private:
		FileSystemObject* symlink;
		Executable* script;

};

