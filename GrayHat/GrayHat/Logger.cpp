#define _CRT_SECURE_NO_WARNINGS

#include "Logger.h"

#include <ctime>
#include <chrono>
#include <time.h>


std::vector<struct Logger::FILE*> Logger::files = std::vector<struct Logger::FILE*>();
std::map<struct Logger::FILE*, int> Logger::instances = std::map<struct Logger::FILE*, int>();

Logger::Logger(const char* fileName)
{
	lock.lock();
	if (file = contains(fileName)) {
		instances[file]++;
	}
	else {
		file = new struct FILE(new std::ofstream(std::string(fileName)+".log", std::ofstream::out), std::string(fileName));
		instances[file] = 1;
		files.push_back(file);
	}
	lock.unlock();
}
Logger::~Logger()
{
	lock.lock();
	instances[file]--;
	if (!instances[file])
	{
		file->file->close();
		for (std::vector<struct FILE*>::iterator it = files.begin(); it != files.end(); it++) {
			if (*it == file) {
				files.erase(it);
				break;
			}
		}
		instances.erase(file);
		delete file;
	}
	lock.unlock();
}

struct Logger::FILE* Logger::contains(const char* fileName) {
	for (std::vector<struct FILE*>::iterator it = files.begin(); it != files.end(); it++) {
		if (!(*it)->name.compare(std::string(fileName))) {
			return *it;
		}
	}
	return NULL;
}

void Logger::log(std::string str) {
	log(str.c_str());
}

void Logger::log(const char* str) {

	file->lock->lock();
	std::time_t endTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	char* time = std::ctime(&endTime);
	char* temp = time;
	while (*temp != '\0')
		temp++;
	temp--;
	*temp = '\0';

	(*file->file) << time << ": "<< str << std::endl;
	file->lock->unlock();
}