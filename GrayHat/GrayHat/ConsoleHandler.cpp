#include "ConsoleHandler.h"
#include <thread>


ConsoleHandler::ConsoleHandler() :logger("ConsoleHandler")
{

}


ConsoleHandler::~ConsoleHandler()
{
}

int ConsoleHandler::getYSize(){
	return ScreenBufferInfo.srWindow.Bottom - ScreenBufferInfo.srWindow.Top + 1;
}

int ConsoleHandler::getXSize() {
	return ScreenBufferInfo.srWindow.Right - ScreenBufferInfo.srWindow.Left + 1;
}


bool ConsoleHandler::enableVTMode()
{
	// Set output mode to handle virtual terminal sequences
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOut == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	DWORD dwMode = 0;
	if (!GetConsoleMode(hOut, &dwMode))
	{
		return false;
	}

	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	if (!SetConsoleMode(hOut, dwMode))
	{
		return false;
	}
	return true;
}


std::string ConsoleHandler::itoa(int x) {
	static char buff[10];
	_itoa_s(x, buff, 10);

	return std::string(buff);
}

std::string ConsoleHandler::getBorder() {
	//save cursor pos, go to upper left corner, set color, enter line draw mode
	std::string out(ESC "7" ESC "[1;1H" ESC "[48;2;142;142;142m" ESC "(0");

	int verPos = (getXSize() / 2) - 1;

	out += "l";
	for (int x = 1; x < getXSize()-1; x++) {
		if (x == verPos)
			out+="w";
		else
			out+="q";
	}
	out+="k";

	for (int y = 2; y < getYSize(); y++)
		out += ESC "["+itoa(y)+";1Hx\tx\tx";

	out += ESC "["+itoa(getYSize())+";1H";
	out += "m";
	for (int x = 1; x < getXSize() - 1; x++) {
		if (x == verPos)
			out += "v";
		else
			out += "q";
	}
	out += "j";

	//return cursor, reset color, exit line draw mode
	out+= ESC "8" ESC "[m" ESC "(B";

	return out;
}

void ConsoleHandler::setCurrentDirectory(FileSystemFolder* folder) {
	curDir = folder;
}
void ConsoleHandler::setPlayerInventory(std::vector<FileSystemFile*>* inv) {
	playerInventory = inv;
}

std::string ConsoleHandler::getCurrentDir() {

	int mid = (getXSize() / 2);

	std::string out(ESC "[1;34m" ESC "[2;2HCurrent Directory:" ESC "[m");

	if (!curDir) {
		logger.log("current directory is empty, returning");
		return out;
	}

	out += ESC "[1;35m" ESC "[3;2H-> " + curDir->getName()+ ESC "[m";
	std::string sizeStr = itoa(curDir->getSize()) + " kB";
	out += ESC "[" + itoa(mid - sizeStr.length() - 1) + "G" + sizeStr.substr(0, sizeStr.length() - 2) + ESC "[32mkB" ESC "[m";



	int lineNo = 3;

	if (curDir->getParent() != NULL) {
		logger.log("parent is not null");
		out += ESC "[" + itoa(lineNo) + ";5H-> ..";
		out += ESC "[" + itoa(mid - sizeStr.length() - 1) + "G" + sizeStr.substr(0, sizeStr.length() - 2) + ESC "[32mkB" ESC "[m";
	}

	std::vector<FileSystemObject*>* contents = curDir->getContents();
	for (std::vector<FileSystemObject*>::iterator it = contents->begin(); it != contents->end(); it++) {
		lineNo++;
		if ((*it)->getType() == TYPE_DIR) {
			sizeStr = itoa((*it)->getSize()) + " kB";
			out += ESC "[33m" ESC "[" + itoa(lineNo) + ";5H-> " + (*it)->getName();
			out += ESC "[" + itoa(mid - sizeStr.length() - 1) + "G" + sizeStr.substr(0, sizeStr.length() - 2) + ESC "[32mkB" ESC "[m";
		}
		//else if ((*it)->getType() == TYPE_FILE_SYM) {

		//}
		else {//regular file or executable
			sizeStr = itoa((*it)->getSize()) + " kB";
			out += ESC "[" + itoa(lineNo) + ";5H-> " + (*it)->getName();
			out += ESC "[" + itoa(mid - sizeStr.length() - 1) + "G" + sizeStr.substr(0, sizeStr.length() - 2) + ESC "[32mkB" ESC "[m";
		}
	}


	delete contents;

	return out;
}


std::string ConsoleHandler::getInventory() {

	int halfPos = getYSize() / 2;

	std::string out(ESC "[1;34m" ESC "["+itoa(halfPos) + ";2HInventory:" ESC "[m");

	for (std::vector<FileSystemFile*>::iterator it = playerInventory->begin(); it != playerInventory->end(); it++) {
		halfPos++;
		out += ESC "[" + itoa(halfPos) + ";5H" + (*it)->getName();
	}

	return out;
}


std::string ConsoleHandler::getKeyboardInput() {
	std::string input = kbh.getBuffer();
	//std::string ret;

	std::string yPos = itoa(getYSize() - 1);

	for (int x = 0; x < input.length(); x++) {
		if (input[x] != '#') {
			inputBuffer += input[x];
		}
		else {
			switch (input[x + 1]) {
			case KEYBOARD_BACKSPACE[0]:
				if (inputBuffer.length() > 0) {
					inputBuffer.pop_back();
				}

				break;
			case KEYBOARD_ENTER[0]:
				logger.log("have command: " + inputBuffer);
				inputBuffer.clear();
				break;
			}
			x++;
		}
	}
	return ESC "[" + itoa(getYSize() - 1) + ";4H" + inputBuffer;
}

void ConsoleHandler::start(HWND windowHandle) {

	kbh.start();

	this->windowHandle = windowHandle;
	GetWindowRect(windowHandle, &windowSize);

	bool fSuccess = enableVTMode();
	if (!fSuccess)
	{
		printf("Unable to enter VT processing mode.\n");
	}
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOut == INVALID_HANDLE_VALUE)
	{
		printf("Couldn't get the console handle.\n");
	}

	GetConsoleScreenBufferInfo(hOut, &ScreenBufferInfo);

	windowStatus = WINDOW_UP;

	//go to alt screen
	printf(ESC "[?1049h");

	//set tab at middle of screen
	printf(ESC "[3g" ESC "[1;%dH" ESC "H", getXSize() / 2);

	std::string drawString;
	while (windowStatus == WINDOW_UP) {

		drawString = ESC "[2J" + getBorder();
		drawString += getCurrentDir();
		drawString += getInventory();
		drawString += getKeyboardInput();

		drawString += ESC "[" + itoa(getYSize() - 1) + ";3H>";

		printf(drawString.c_str());

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	printf(ESC "[?1049l");
	kbh.stop();
}
