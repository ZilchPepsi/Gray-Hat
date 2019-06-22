


//int main()
//{
//	//GameEngine ge;
//
//	//ge.mainLoop();
//
//
//	return 0;
//}

#include "GameEngine.h"
#include "ConsoleHandler.h"


#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

#define ESC "\x1b"

#define WIDTH  1600
#define HEIGHT 900

//void PrintVerticalBorder()
//{
//	printf(ESC "(0");       // Enter Line drawing mode
//	printf(CSI "104;93m");   // bright yellow on bright blue
//	printf("x");            // in line drawing mode, \x78 -> \u2502 "Vertical Bar"
//	printf(CSI "0m");       // restore color
//	printf(ESC "(B");       // exit line drawing mode
//}
//
//void PrintHorizontalBorder(int xSize, int ySize, bool fIsTop)
//{
//	printf(ESC "(0");       // Enter Line drawing mode
//	printf(CSI "104;93m");  // Make the border bright yellow on bright blue
//	printf(fIsTop ? "l" : "m"); // print left corner 
//
//	for (int i = 1; i < xSize - 1; i++)
//		printf("q"); // in line drawing mode, \x71 -> \u2500 "HORIZONTAL SCAN LINE-5"
//
//	printf(fIsTop ? "k" : "j"); // print right corner
//	printf(CSI "0m");
//	printf(ESC "(B");       // exit line drawing mode
//}
//
//void PrintStatusLine(const char* pszMessage, int xSize, int ySize)
//{
//	printf(CSI "%d;1H", ySize);
//	printf(CSI "K"); // clear the line
//	printf(pszMessage);
//}



int __cdecl wmain(int argc, WCHAR* argv[])
{
	HWND console = GetConsoleWindow();
	MoveWindow(console, 0, 0, WIDTH, HEIGHT, TRUE);

	ConsoleHandler ch;

	GameEngine ge(&ch);
	ge.start();

	ch.start(console);



	//// Enter the alternate buffer
	//printf(CSI "?1049h");

	//// Clear screen, tab stops, set, stop at columns 16, 32
	//printf(CSI "1;1H");
	//printf(CSI "2J"); // Clear screen

	//int iNumTabStops = 4; // (0, 20, 40, width)
	//printf(CSI "3g"); // clear all tab stops
	//printf(CSI "1;20H"); // Move to column 20
	//printf(ESC "H"); // set a tab stop

	//printf(CSI "1;40H"); // Move to column 40
	//printf(ESC "H"); // set a tab stop

	//// Set scrolling margins to 3, h-2
	//printf(CSI "3;%dr", ch.getYSize() - 2);
	//int iNumLines = ch.getYSize() - 4;

	//printf(CSI "1;1H");
	//printf(CSI "102;30m");
	//printf("Windows 10 Anniversary Update - VT Example");
	//printf(CSI "0m");

	//// Print a top border - Yellow
	//printf(CSI "2;1H");
	//PrintHorizontalBorder(ch.getXSize(), ch.getYSize(), true);

	//// // Print a bottom border
	//printf(CSI "%d;1H", ch.getYSize() - 1);
	//PrintHorizontalBorder(ch.getXSize(), ch.getYSize(), false);

	//wchar_t wch;

	//// draw columns
	//printf(CSI "3;1H");
	//int line = 0;
	//for (line = 0; line < iNumLines * iNumTabStops; line++)
	//{
	//	PrintVerticalBorder();
	//	if (line + 1 != iNumLines * iNumTabStops) // don't advance to next line if this is the last line
	//		printf("\t"); // advance to next tab stop

	//}

	//PrintStatusLine("Press any key to see text printed between tab stops.", ch.getXSize(), ch.getYSize());
	//wch = _getwch();

	//// Fill columns with output
	//printf(CSI "3;1H");
	//for (line = 0; line < iNumLines; line++)
	//{
	//	int tab = 0;
	//	for (tab = 0; tab < iNumTabStops - 1; tab++)
	//	{
	//		PrintVerticalBorder();
	//		printf("line=%d", line);
	//		printf("\t"); // advance to next tab stop
	//	}
	//	PrintVerticalBorder();// print border at right side
	//	if (line + 1 != iNumLines)
	//		printf("\t"); // advance to next tab stop, (on the next line)
	//}

	//PrintStatusLine("Press any key to demonstrate scroll margins", ch.getXSize(), ch.getYSize());
	//wch = _getwch();

	//printf(CSI "3;1H");
	//for (line = 0; line < iNumLines * 2; line++)
	//{
	//	printf(CSI "K"); // clear the line
	//	int tab = 0;
	//	for (tab = 0; tab < iNumTabStops - 1; tab++)
	//	{
	//		PrintVerticalBorder();
	//		printf("line=%d", line);
	//		printf("\t"); // advance to next tab stop
	//	}
	//	PrintVerticalBorder(); // print border at right side
	//	if (line + 1 != iNumLines * 2)
	//	{
	//		printf("\n"); //Advance to next line. If we're at the bottom of the margins, the text will scroll.
	//		printf("\r"); //return to first col in buffer
	//	}
	//}

	//PrintStatusLine("Press any key to exit", ch.getXSize(), ch.getYSize());
	//wch = _getwch();

	//// Exit the alternate buffer
	//printf(CSI "?1049l");

}