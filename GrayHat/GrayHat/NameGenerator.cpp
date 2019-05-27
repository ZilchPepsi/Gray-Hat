#include "NameGenerator.h"

#include <fstream>
#include <cstdlib>
#include <ctime>

NameGenerator::NameGenerator():logger("nameGenerator")
{
	loaded = false;
}


NameGenerator::~NameGenerator()
{
}

void NameGenerator::init() {

	logger.log("starting up initialization");

	srand((unsigned int)time(0));

	char word[MAX_WORD_SIZE];
	std::ifstream inputFile;

	inputFile.open("Prefix.txt", std::ifstream::in);
	while (inputFile.peek() != EOF) {
		inputFile.getline(word, MAX_WORD_SIZE);
		prefix.push_back(word);
	}
	inputFile.close();

	inputFile.open("Suffix.txt", std::ifstream::in);
	while (inputFile.peek() != EOF) {
		inputFile.getline(word, MAX_WORD_SIZE);
		suffix.push_back(word);
	}
	inputFile.close();

	inputFile.open("Extensions.txt", std::ifstream::in);
	while (inputFile.peek() != EOF) {
		inputFile.getline(word, MAX_WORD_SIZE);
		extensions.push_back(word);
	}
	inputFile.close();

	loaded = true;

	logger.log("finished initialization");
}

std::string NameGenerator::generateFileName() {

	logger.log("returning a name");
	return prefix[rand() % prefix.size()] + 
		"_" + 
		suffix[rand() % suffix.size()] + 
		extensions[rand() % extensions.size()];
}

