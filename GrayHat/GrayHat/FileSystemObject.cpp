#include "FileSystemObject.h"

FileSystemObject::FileSystemObject(std::string str, int typ):logger("FileSystemObject") {
	name = str;
	type = typ;
	displayName = str;
	displayType = typ;
	height = 1;
	size = 1;
}


FileSystemObject::~FileSystemObject() {

}

std::string FileSystemObject::getName() {
	return name;
}

int FileSystemObject::getType() {
	return type;
}

std::string FileSystemObject::getDisplayName() {
	return displayName;
}

int FileSystemObject::getDisplayType() {
	return displayType;
}

void FileSystemObject::setName(std::string str) {
	name = str;
}
void FileSystemObject::setType(int typ) {
	type = typ;
}

void FileSystemObject::setDisplayName(std::string str) {
	displayName = str;
}

void FileSystemObject::setDisplayType(int typ) {
	displayType = typ;
}

int FileSystemObject::encrypt(int val) {
	if (val <=0)
		return size;
	return size / val;
}

int FileSystemObject::getHeight() {
	return height;
}

void FileSystemObject::setHeight(int h){
	height = h;
}

int FileSystemObject::getSize() {
	return size;
}
void FileSystemObject::setSize(int s) {
	size = s;
}

FileSystemObject* FileSystemObject::getParent() {
	return parent;
}

void FileSystemObject::setParent(FileSystemObject* f) {
	parent = f;
}


