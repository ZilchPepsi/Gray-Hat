#pragma once

#include "TerminalGraphics.h"

#include <stdio.h>
#include <wchar.h>
#include <windows.h>
#include <iostream>

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

class TerminalGraphics_win : TerminalGraphics
{
	public:
		TerminalGraphics_win();
		~TerminalGraphics_win();

		std::string init();
		void clearScreen();
		void setScreenSize(int height, int width);
		std::string writeText(std::string text, int row, int col);
		std::string writeText(std::string text, int row, int col, int color);
		void setCursorVisible(bool visible);

	private:
		HANDLE outHandle;

		const wchar_t * ESC = L"\x1b";
		const wchar_t * CSI = L"\x1b[";
		const wchar_t * CSI_CLR_SCRN = L"\x1b[2J";

		//helper functions
};

