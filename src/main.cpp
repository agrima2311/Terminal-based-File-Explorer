#include "../include/headers.h"

const int vertCursorScrollPos = 72;

int idxCurDirItemSelected = 1;
int curDirItemsOffset = 1;
int curTotalDirItems;
int maxHeightForDirItems;
string appHome = ".";

struct winsize termSize;

stack<string> backwardDirsAbsPaths;
stack<string> forwardDirsAbsPaths;
vector<string> curDirItemNames;

string absPathCurDir;

int xpos = 1;
int ypos = 72;

void window_resize_handler(int signal)
{
	initTerminalDimensions();
	exitRawMode();
	listDir(absPathCurDir, curDirItemsOffset);
	enterRawMode();
}

int main(int argc, char *argv[])
{
	// Save cursor position & switch to alternate screen
	printf("\0337\033[?47h");
	initTerminalDimensions();

	absPathCurDir = ".";

	listDir(absPathCurDir, curDirItemsOffset);

	dashboard();

	// Clear alternate screen, switch back to primary, restore cursor
	printf("\033[2J\033[?47l\0338");
}
