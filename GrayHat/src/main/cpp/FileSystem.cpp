#include "FileSystem.h"

#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <stack>
#include "NameGenerator.h"


FileSystem::FileSystem() : root(std::string("root"), TYPE_DIR), logger("FileSystem"){}


FileSystem::~FileSystem()
{
}

int FileSystem::generateSystem()
{
	logger.log("generating system");

	srand((unsigned int)time(0));
	NameGenerator generator;
	generator.init();
	
	std::vector<FileSystemFile*> symlinks;
	std::vector<FileSystemFile*> executables;


	//global maxes for this generated map
	int maxHeight = std::max(rand() % MAX_TREE_HEIGHT + 1, MIN_TREE_HEIGHT);	//max height of tree
	int maxContents = std::max(rand() % MAX_CONTENTS + 1, MIN_CONTENTS);		//max number of items in a directory
	int maxDirs = std::max( rand() % maxContents, MIN_DIRS);					//max directories in a directory

	logger.log("max height: " + logger.itoa(maxHeight));
	logger.log("max contents: " + logger.itoa(maxContents));
	logger.log("max dirs: " + logger.itoa(maxDirs));


	//populate root directory
	for (int x = 0; x < std::max(maxDirs, MIN_ROOT_DIRS); x++) {
		root.addSubFolder(generator.generateDirectoryName(), TYPE_DIR);
	}
	for (int x = 0; x < maxContents - std::max(maxDirs, MIN_ROOT_DIRS); x++) {
		root.addFile(generator.generateFileName(), TYPE_FILE_MISC);
	}

	/*
	************************************************	
	//creates system, and assigns files/directories
	************************************************
	*/
	int objBranch = rand() % maxDirs + 1;					//the branch in root that will contain the obj file
	int keyBranch = rand() % maxDirs + 1;					//the branch in root that will contain the key file
	logger.log("set branch");
	float spcDif_level = PROB_FILE_MISC / maxHeight;		//increase of special file probability per level
	float objDif_level = PROB_FILE_EXESYM / maxHeight;		//increase of objective file probability per level
	//the above 2 levels ensure that at max depth the probability of placing an objective file is 100%

	logger.log("objective going on branch " + logger.itoa(objBranch));
	logger.log("key going on branch " + logger.itoa(keyBranch));

	bool keyPlaced = false;									//has the key been placed?
	bool objectivePlaced = false;							//has the objective file been placed?
	int dirCount = 0;										//number of root directories parsed, used for finding
															//branch for key and objective files
	FileSystemObject* obj;
	FileSystemFolder* fo;
	FileSystemFile* fi;

	std::stack<FileSystemObject*> stack;
	std::vector<FileSystemObject*>* contents = root.getContents();
	logger.log("starting file iteration in generation");
	for (std::vector<FileSystemObject*>::iterator it = contents->begin(); it != contents->end(); it++) {
		
		//this is a directory
		if ((*it)->getType() == TYPE_DIR) {
			dirCount++;
			logger.log("going into branch " + logger.itoa(dirCount));
		}
		stack.push(*it);
		//this is the DFS approach to populating the map
		while (!stack.empty()) {

			obj = stack.top();
			stack.pop();

			//got a directory
			if ((fo = dynamic_cast<FileSystemFolder*>(obj)) != NULL) {
				if (fo->getHeight() == maxHeight)
					continue;

				int maxC = std::max(rand() % maxContents + 1, MIN_CONTENTS);					//contents
				int maxD = std::max((rand() % (std::min(maxContents-1,maxDirs)))+1, MIN_DIRS);	//directories
					
				//populate directory
				for (int x = 0; x < maxD; x++) {
					fo->addSubFolder(generator.generateDirectoryName(), TYPE_DIR);
				}
				for (int x = 0; x < maxC - maxD; x++) {
					fo->addFile(generator.generateFileName(), TYPE_FILE_MISC);
				}

				//set height of all contents in this directory and add to stack
				std::vector<FileSystemObject*>* subCont = fo->getContents();
				for (std::vector<FileSystemObject*>::iterator it = subCont->begin(); it != subCont->end(); it++) {
					(*it)->setHeight(fo->getHeight() + 1);
					stack.push(*it);
				}
				delete subCont;

			}
			//got a file
			else {
				fi = dynamic_cast<FileSystemFile*>(obj);

				//if (should create special file)
				if ((rand() % 100) < PROB_FILE_SPEC + (((float)fi->getHeight())*spcDif_level)) {

					bool placed = false;

					//if(should create objective file
					if ((rand() % 100) < PROB_FILE_OBJ + (((float)fi->getHeight())*objDif_level)) {
						//this is the branch that is supposed to contain the obj file
						if (dirCount == objBranch && !objectivePlaced) {
							fi->setType(TYPE_FILE_OBJ);
							fi->setName(DEFAULT_OBJ_NAME);
							fi->setDisplayName(DEFAULT_OBJ_NAME);
							fi->setSize(DEFAULT_OBJ_SIZE);
							objectivePlaced = true;
							placed = true;
						}
						else if(dirCount == keyBranch && !keyPlaced) {
							fi->setType(TYPE_FILE_KEY);
							fi->setName(DEFAULT_KEY_NAME);
							fi->setDisplayName(DEFAULT_KEY_NAME);
							fi->setSize(DEFAULT_KEY_SIZE);
							keyPlaced = true;
							placed = true;
						}
					}
					//create special file
					if (!placed) {
						if ((rand() % 100) < PROB_FILE_EXE) {
							fi->setType(TYPE_FILE_EXE);
							fi->setSize((rand() % MAX_EXE_SIZE) + 1);
							fi->changeExtension(DEFAULT_EXE_EXT);
							executables.push_back(fi);
						}
						else {
							fi->setType(TYPE_FILE_SYM);
							fi->setSize(DEFAULT_SYM_SIZE);
							fi->changeExtension(DEFAULT_SYM_EXT);
							symlinks.push_back(fi);
						}
					}
				}
				else {
					fi->setType(TYPE_FILE_MISC);
					fi->setSize((rand() % MAX_MISC_SIZE) + 1);
				}
			}
		}
	}
	delete contents;

	logger.log("linking symLinks");
	//links all symbolic links
	for (std::vector<FileSystemFile*>::iterator it = symlinks.begin(); it != symlinks.end(); it++) {
		int height = rand() % maxHeight;
		FileSystemFolder* currentSubDir = &root;
		for (int x = 0; x < height; x++) {
			if (currentSubDir->getFolders().size() > 0) {
				int pos = rand() % currentSubDir->getFolders().size();
				currentSubDir = currentSubDir->getFolders()[pos];
			}
			else {
				break;
			}
		}
		std::vector<FileSystemObject*>* contents = currentSubDir->getContents();
		int pos = rand() % contents->size();
		(*it)->setSymLink((*contents)[pos]);
		delete contents;
	}

	//TODO set executables
	logger.log("System generated");
	return 0;
}

std::string FileSystem::getFilePath(const char* fileName) {


	std::stack<FileSystemObject*> stack;
	stack.push(&root);

	FileSystemFolder* fo;
	FileSystemObject* obj;
	while (!stack.empty()) {
		obj = stack.top();
		stack.pop();
		
		if ((fo = dynamic_cast<FileSystemFolder*>(obj)) != NULL) {
			std::vector<FileSystemObject*>* contents = fo->getContents();
			for (std::vector<FileSystemObject*>::iterator it = contents->begin(); it != contents->end(); it++) {
				stack.push(*it);
			}
			delete contents;
		}
		else if(!obj->getName().compare(fileName)) {
			return obj->getAbsolutePath();
		}
	}
	return std::string("could not find ") + fileName;
}

std::string FileSystem::getFilePath(std::string str) {
	return getFilePath(str.c_str());
}

std::string FileSystem::getKeyPath() {
	return getFilePath(DEFAULT_KEY_NAME);
}
std::string FileSystem::getObjPath() {
	return getFilePath(DEFAULT_OBJ_NAME);
}
std::string FileSystem::getPEMPath() {
	return getFilePath(DEFAULT_PEM_NAME);
}


FileSystemFolder* FileSystem::getRoot()
{
	return &root;
}
