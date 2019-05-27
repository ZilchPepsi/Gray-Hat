#pragma once

#include <vector>
#include "Logger.h"

#define MAX_WORD_SIZE 20

class NameGenerator
{
public:
	NameGenerator();
	~NameGenerator();

	void init();
	std::string generateFileName();

private:

	Logger logger;

	bool loaded;
	std::vector<std::string> prefix, suffix, extensions;
};

