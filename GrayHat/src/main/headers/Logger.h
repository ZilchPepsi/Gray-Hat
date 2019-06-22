#pragma once

#include <fstream>
#include <vector>
#include <map>
#include <mutex>


class Logger
{
public:
	Logger(const char* fileName);
	~Logger();

	void log(const char*);
	void log(std::string);
	void log();

	std::string itoa(int);

private:

	struct FILE {
		std::ofstream* file;
		std::string name;
		std::mutex* lock;
		FILE(std::ofstream* f, std::string n):file(f),name(n){
			lock = new std::mutex();
		}
		~FILE() { delete file; delete lock; }
	};


	static std::vector<struct FILE*> files;
	static std::map<struct FILE*, int> instances;

	static std::mutex lock;

	struct FILE* file;
	struct FILE* contains(const char*);
};

