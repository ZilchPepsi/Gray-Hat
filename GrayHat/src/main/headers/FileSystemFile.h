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

#define DEFAULT_SYM_EXT ".ln"
#define DEFAULT_EXE_EXT ".exe"

#define MAX_EXE_SIZE 50
#define MAX_MISC_SIZE 10
#define DEFAULT_PEM_SIZE 75
#define DEFAULT_KEY_SIZE 75
#define DEFAULT_OBJ_SIZE 100
#define DEFAULT_SYM_SIZE 1

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
	
		void changeExtension(std::string);

	private:
		FileSystemObject* symlink;
		Executable* script;

};

