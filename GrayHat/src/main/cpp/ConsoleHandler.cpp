#pragma comment(lib, "User32.lib")

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

std::string ConsoleHandler::getNextCommand(){
	std::string ret;
	queueLock.lock();
	if(!commandQueue.empty()){
		ret = commandQueue.front();
		commandQueue.pop();
	}
	queueLock.unlock();
	return ret;
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

std::string ConsoleHandler::getBorder(bool centerLine) {
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
		if(centerLine)
			out += ESC "["+itoa(y)+";1Hx\tx\tx";
		else
			out += ESC "["+itoa(y)+";1Hx\t\tx";

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
		lineNo++;
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
		else if ((*it)->getType() == TYPE_FILE_SYM) {
			sizeStr = itoa((*it)->getSize()) + " kB";
			out += ESC "[" + itoa(lineNo) + ";5H-> " + (*it)->getName();
			out += " -> " + dynamic_cast<FileSystemFile*>(*it)->getSymlink()->getName(); 
			out += ESC "[" + itoa(mid - sizeStr.length() - 1) + "G" + sizeStr.substr(0, sizeStr.length() - 2) + ESC "[32mkB" ESC "[m";
		}
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


std::string ConsoleHandler::getKeyboardInput_GAME_STATE() {
	std::string input = kbh.getBuffer();
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
				queueLock.lock();
				commandQueue.push(inputBuffer);
				queueLock.unlock();
				inputBuffer.clear();
				break;
			case KEYBOARD_TAB[0]:
				handleTab();
				break;
			}
			x++;
		}
	}
	return ESC "[" + itoa(getYSize() - 1) + ";4H" + inputBuffer;
}
int ConsoleHandler::getKeyboardInput_MENU_STATE(bool* enter) {
	std::string input = kbh.getBuffer();
	int ret = 0;	
	for(int x =0; x< input.length(); x++){
		if(input[x] == '#') {
			switch (input[x + 1]) {
			case KEYBOARD_UP[0]:
				ret--;
				break;
			case KEYBOARD_DOWN[0]:
				ret++;
				break;
			case KEYBOARD_ENTER[0]:
				*enter = true;
				break;
			}
			x++;
		}
	}
	return ret;
}

std::string ConsoleHandler::getMenuOptions(){
	int row = getYSize()/2;
	int midX = getXSize()/2;
	bool enterPressed = false;

	menuSelection= (menuSelection+getKeyboardInput_MENU_STATE(&enterPressed))%optionsLen;
	if(menuSelection <0)
	menuSelection = optionsLen-1;

	std::string ret;
	for(int x =0; x< optionsLen; x++){
			ret+=ESC "["+itoa(row+x)+";"+itoa((midX-(sizeof(options[x])-1)/2) + 2)+"H";
		if(menuSelection == x)
			ret += "[ ";
		ret += options[x];
		if(menuSelection == x)
			ret+= " ]";
	}
	if(enterPressed){
		if(menuSelection == 0)
			state = STATE_GAME; 
		else
			state = STATE_EXIT;
	}
	return ret;
}

std::string ConsoleHandler::getMenuArt(){
	std::string col = itoa((getXSize() / 2) - 22);
	int row = (getYSize()/4)-7;
	std::string ret = ESC "["+itoa(row++)+";"+col+"H";
	ret+="           =================/------         " ESC "["+itoa(row++)+";"+col+"H"; 
	ret+="           |###############/      |         " ESC "["+itoa(row++)+";"+col+"H";
	ret+="           |##############/       |         " ESC "["+itoa(row++)+";"+col+"H";
	ret+="           |#############/        |         " ESC "["+itoa(row++)+";"+col+"H";
	ret+="           |############/         |         " ESC "["+itoa(row++)+";"+col+"H";
	ret+="           |###########/          |         " ESC "["+itoa(row++)+";"+col+"H";
	ret+="         =============/---------------      " ESC "["+itoa(row++)+";"+col+"H";
	ret+="       ___    ___    / ___                  " ESC "["+itoa(row++)+";"+col+"H";
	ret+="      |      |   |  / |   |  |   |          " ESC "["+itoa(row++)+";"+col+"H";
	ret+="      |  __  |___| /  |___|  |___|          " ESC "["+itoa(row++)+";"+col+"H";
	ret+="      |___|  |  \\ /   |   |     |          " ESC "["+itoa(row++)+";"+col+"H";
	ret+="                 /            ___   ____    " ESC "["+itoa(row++)+";"+col+"H";
	ret+="                /     |   |  |   |    |     " ESC "["+itoa(row++)+";"+col+"H";
	ret+="               /      |___|  |___|    |     " ESC "["+itoa(row++)+";"+col+"H";
	ret+="              /       |   |  |   |    |     ";
return ret;
}

bool ConsoleHandler::startsWith(std::string a, std::string b){
	if(b.size() > a.size()){
		return false;
	}

	for(int x= 0; x< b.size(); x++){
		if(a[x] != b[x]){
			return false;
		}
	}
	return true;
}

void ConsoleHandler::handleTab(){
		std::string file;

		if(inputBuffer.find(' ') != std::string::npos){
			file = inputBuffer.substr(inputBuffer.find_last_of(' ')+1);
		}else file = inputBuffer;


		std::vector<std::string> matches;
		std::vector<FileSystemObject*>* contents  = curDir->getContents();
		for(std::vector<FileSystemObject*>::iterator it = contents->begin(); it != contents->end(); it++){
				if(startsWith((*it)->getName(), file)){
					logger.log((*it)->getName() +" starts with "+file+"...adding");
					matches.push_back((*it)->getName());
				}
		}
		delete contents;

		if(!matches.empty()){
			if(matches.size()>1){
				std::string ret = file; 
				int pos = file.size();
				int nextChar;
				bool good = true;
				while(good && pos < matches[0].size()){
					nextChar = matches[0][pos];
					for(int x = 1; x< matches.size(); x++){
						if(matches[x][pos] != nextChar){
							good = false;
							break;
						}
					}
					if(good)
						ret += nextChar; 
					pos++;
				}

				if(inputBuffer.find(' ') != std::string::npos){
					inputBuffer = inputBuffer.substr( 0, inputBuffer.find_last_of(' ')+1) + ret;
				}else inputBuffer = ret;
			}else{
				if(inputBuffer.find(' ') != std::string::npos){
					inputBuffer = inputBuffer.substr( 0, inputBuffer.find_last_of(' ')+1) + matches[0];
				}else inputBuffer = matches[0];
			}
		}
}

char ConsoleHandler::getState(){
	return state;
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

	//go to alt screen
	printf(ESC "[?1049h");

	//set tab at middle of screen
	printf(ESC "[3g" ESC "[1;%dH" ESC "H", getXSize() / 2);

	std::string drawString;
	while (state != STATE_EXIT) {

		if(state == STATE_GAME){
		drawString = ESC "[?25h" ESC "[2J" + getBorder(true);
		drawString += getCurrentDir();
		drawString += getInventory();
		drawString += getKeyboardInput_GAME_STATE();

		drawString += ESC "[" + itoa(getYSize() - 1) + ";3H>";
		}
		else if(state == STATE_MENU){
			drawString = ESC "[?25l" ESC "[2J" + getBorder(false);
			drawString += getMenuArt();
			drawString += getMenuOptions();
		}

		printf(drawString.c_str());

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	printf(ESC "[?1049l");
	kbh.stop();
}
