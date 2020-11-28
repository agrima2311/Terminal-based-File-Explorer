#include "../include/headers.h"

void initTerminalDimensions()
{

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &termSize);

    maxHeightForDirItems = termSize.ws_row - 1;
}

void initNormalModeCursorPos()
{
    xpos = 1;
    ypos = vertCursorScrollPos;
}

void initCommandModeCursorPos()
{
    xpos = maxHeightForDirItems + 1;
    ypos = 1;
}

string splitPath(string str)
{
    int positionOfParent;
    positionOfParent = str.find_last_of("/\\");
    return str.substr(0, positionOfParent);
}