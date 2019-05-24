#pragma once

#include <string>

class TerminalGraphics
{
	public:
		TerminalGraphics();
		~TerminalGraphics();

		std::string init();
		void clearScreen();
		void setScreenSize(int height, int width);
		std::string writeText(std::string, int row, int col);
		std::string writeText(std::string, int row, int col, int color);


		const static int CC_DEFAULT =	0;
		const static int CC_FORE_BLK =	30;
		const static int CC_FORE_RED =	31;
		const static int CC_FORE_GRN =	32;
		const static int CC_FORE_YEL =	33;
		const static int CC_FORE_BLU =	34;
		const static int CC_FORE_MAG =	35;
		const static int CC_FORE_CYN =	36;
		const static int CC_FORE_WHT =	37;
		const static int CC_FORE_DEF =	38;
};

