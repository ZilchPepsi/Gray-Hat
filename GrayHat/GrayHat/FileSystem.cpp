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
	int maxDirs = std::max( rand() % maxContents , MIN_DIRS);					//max directories in a directory

	logger.log("max height: " + logger.itoa(maxHeight));
	logger.log("max contents: " + logger.itoa(maxContents));
	logger.log("max dirs: " + logger.itoa(maxDirs));


	//populate root directory
	for (int x = 0; x < maxDirs; x++) {
		root.addSubFolder(generator.generateDirectoryName(), TYPE_DIR);
	}
	for (int x = 0; x < maxContents - maxDirs; x++) {
		root.addFile(generator.generateFileName(), TYPE_FILE_MISC);
	}

	/*
	************************************************	
	//creates system, and assigns files/directories
	************************************************
	*/
	int objBranch = rand() % maxDirs + 1;					//the branch in root that will contain the obj file
	int keyBranch = rand() % maxDirs + 1;					//the branch in root that will contain the key file
	float spcDif_level = PROB_FILE_MISC/maxHeight;			//increase of special file probability per level
	float objDif_level = PROB_FILE_EXESYM / maxHeight;		//increase of objective file probability per level
	//the above 2 levels ensure that at max depth the probability of placing an objective file is 100%

	bool keyPlaced = false;									//has the key been placed?
	bool objectivePlaced = false;							//has the objective file been placed?
	int dirCount = 0;										//number of root directories parsed, used for finding
																//branch for key and objective files
	FileSystemObject* obj;
	FileSystemFolder* fo;
	FileSystemFile* fi;

	std::stack<FileSystemObject*> stack;
	std::vector<FileSystemObject*>* contents = root.getContents();
	for (std::vector<FileSystemObject*>::iterator it = contents->begin(); it != contents->end(); it++) {
		
		//this is a directory
		if ((*it)->getType() == TYPE_DIR) {
			dirCount++;
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
							objectivePlaced = true;
							placed = true;
						}
						else if(dirCount == keyBranch && !keyPlaced) {
							fi->setType(TYPE_FILE_KEY);
							fi->setName(DEFAULT_KEY_NAME);
							fi->setDisplayName(DEFAULT_KEY_NAME);
							keyPlaced = true;
							placed = true;
						}
					}
					//create special file
					if (!placed) {
						if ((rand() % 100) < PROB_FILE_EXE) {
							fi->setType(TYPE_FILE_EXE);
							fi->changeExtension(".exe");
							executables.push_back(fi);
						}
						else {
							fi->setType(TYPE_FILE_SYM);
							fi->changeExtension(".ln");
							symlinks.push_back(fi);
						}
					}
				}
				else {
					fi->setType(TYPE_FILE_MISC);
				}
			}
		}
	}
	delete contents;

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
	return 0;
}

FileSystemFolder* FileSystem::getRoot()
{
	return &root;
}

int FileSystem::treeSize(FileSystemObject* obj)
{
	FileSystemFolder *fo;

	if ((fo = dynamic_cast<FileSystemFolder*>(obj)) != NULL) {

		int sum = fo->getSize();
		std::vector<FileSystemObject*>* contents = fo->getContents();
		for (std::vector<FileSystemObject*>::iterator it = contents->begin(); it != contents->end(); it++) {
			if (dynamic_cast<FileSystemFolder*>(*it) != NULL) {
				sum += treeSize(*it);
			}
			else {
				sum += (*it)->getSize();
			}
		}
		delete contents;
		return sum;
	}
	//this is a file, return the size of the file
	else {
		return obj->getSize();
	}
}