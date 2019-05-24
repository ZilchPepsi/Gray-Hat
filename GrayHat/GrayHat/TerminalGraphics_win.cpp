#include "TerminalGraphics_win.h"


TerminalGraphics_win::TerminalGraphics_win(){}
TerminalGraphics_win::~TerminalGraphics_win(){}

std::string TerminalGraphics_win::init()
{
	outHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	if (outHandle == INVALID_HANDLE_VALUE)
		return "[ERROR] invalid handle\n";

	DWORD handleModeOrig = 0;
	if (!GetConsoleMode(outHandle, &handleModeOrig))
	{
		return "[ERROR] invalid orig handle mode\n";
	}

	DWORD handleModeRequested = ENABLE_VIRTUAL_TERMINAL_INPUT | DISABLE_NEWLINE_AUTO_RETURN;
	if (!SetConsoleMode(outHandle, handleModeOrig | handleModeRequested))
	{
		return "[ERROR] failed to set console mode\n";
	}

	return "init\n";
}


void TerminalGraphics_win::clearScreen()
{
	std::cout.flush();
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (!GetConsoleScreenBufferInfo(outHandle, &csbi))
	{
		// csbi method won't work, last resort
		wprintf(CSI_CLR_SCRN);
	}
	DWORD length = csbi.dwSize.X * csbi.dwSize.Y;
	DWORD written;
	COORD topLeft = { 0,0 };
	FillConsoleOutputCharacter(outHandle, TEXT(' '), length, topLeft, &written);
	FillConsoleOutputAttribute(outHandle, csbi.wAttributes, length, topLeft, &written);
	SetConsoleCursorPosition(outHandle, topLeft);
}


std::string TerminalGraphics_win::writeText(std::string text, int row, int col)
{
	return writeText(text, row, col, CC_DEFAULT);
}

std::string TerminalGraphics_win::writeText(std::string text, int row, int col, int color)
{
	std::wstring cmd = CSI + std::to_wstring(color) + L"m"; // make color command
	wprintf(cmd.c_str());	//apply color command

	cmd = CSI + std::to_wstring(row) + L";" + std::to_wstring(col) + L"H"; // maake cursor pos set command
	wprintf(cmd.c_str()); // move cursor

	printf(text.c_str()); // write text

	return "wrote\n";
}

void TerminalGraphics_win::setScreenSize(int height, int width)
{
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, width, height, TRUE);
}

void TerminalGraphics_win::setCursorVisible(bool visible)
{
	CONSOLE_CURSOR_INFO cursor;
	GetConsoleCursorInfo(outHandle, &cursor);
	cursor.bVisible = false; // set to visible to actually have func
	if (visible)
		cursor.dwSize = 10;
	else
		cursor.dwSize = 0;
	SetConsoleCursorInfo(outHandle, &cursor);
}