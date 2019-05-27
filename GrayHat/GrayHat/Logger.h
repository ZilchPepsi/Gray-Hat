#pragma once

#include <fstream>
#include <vector>
#include <map>


class Logger
{
public:
	Logger(const char* fileName);
	~Logger();

	void log(const char*);
	void log(std::string);

private:

	struct FILE {
		std::ofstream* file;
		std::string name;
		FILE(std::ofstream* f, std::string n):file(f),name(n){}
		~FILE() {delete file;}
	};


	static std::vector<struct FILE*> files;
	static std::map<struct FILE*, int> instances;

	struct FILE* file;
	struct FILE* contains(const char*);
};

