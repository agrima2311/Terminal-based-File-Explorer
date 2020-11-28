#include "../include/headers.h"

struct termios oldt;
struct termios newt;

void enterRawMode()
{
    signal(SIGWINCH, window_resize_handler);
    tcgetattr(STDIN_FILENO, &newt);
    newt = oldt;             /* copy old settings to new settings */
    newt.c_lflag &= ~ICANON; /* make one change to old settings in new settings */
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    moveCursor;
}

void dashboard()
{
    tcgetattr(STDIN_FILENO, &oldt);
    enterRawMode();
    resetCursorNormalMode();
    moveCursor;

    while (1)
    {
        fflush(stdin);
        char inp[3];
        inp[0] = cin.get();

        if (inp[0] == 75 || inp[0] == 107) //scrolling
        {
            if (xpos == 1 && curDirItemsOffset > 1)
            {
                idxCurDirItemSelected--;
                curDirItemsOffset--;
                listDir(absPathCurDir, curDirItemsOffset);
                moveCursor;
            }
            else if (xpos > 1)
            {
                xpos--;
                idxCurDirItemSelected--;
                moveCursor;
            }
        }
        if (inp[0] == 76 || inp[0] == 108) //scrolling
        {
            if (xpos == maxHeightForDirItems && idxCurDirItemSelected < curTotalDirItems)
            {
                idxCurDirItemSelected++;
                curDirItemsOffset++;
                // exitRawMode();
                listDir(absPathCurDir, curDirItemsOffset);
                moveCursor;
                // enterRawMode();
            }
            else if (xpos < maxHeightForDirItems && idxCurDirItemSelected < curTotalDirItems)
            {
                xpos++;
                idxCurDirItemSelected++;
                moveCursor;
            }
        }

        if (inp[0] == 27) // case of a special key press
        {
            inp[1] = cin.get();
            inp[2] = cin.get();
            if (inp[2] == 'A' || inp[0] == 107 || inp[0] == 75) //up key pressed
            {
                if (xpos == 1 && curDirItemsOffset > 1)
                {
                    idxCurDirItemSelected--;
                    curDirItemsOffset--;
                    listDir(absPathCurDir, curDirItemsOffset);
                    moveCursor;
                }
                else if (xpos > 1)
                {
                    xpos--;
                    idxCurDirItemSelected--;
                    moveCursor;
                }
            }
            if (inp[2] == 'B' || inp[0] == 108 || inp[0] == 76) //down key pressed
            {
                if (xpos == maxHeightForDirItems && idxCurDirItemSelected < curTotalDirItems)
                {
                    idxCurDirItemSelected++;
                    curDirItemsOffset++;
                    // exitRawMode();
                    listDir(absPathCurDir, curDirItemsOffset);
                    moveCursor;
                    // enterRawMode();
                }
                else if (xpos < maxHeightForDirItems && idxCurDirItemSelected < curTotalDirItems)
                {
                    xpos++;
                    idxCurDirItemSelected++;
                    moveCursor;
                }
            }

            if (inp[2] == 'C') // right arrow key
            {
                moveCursor;
                if (!forwardDirsAbsPaths.empty())
                {
                    backwardDirsAbsPaths.push(absPathCurDir);
                    absPathCurDir = forwardDirsAbsPaths.top();
                    forwardDirsAbsPaths.pop();
                    resetCursorNormalMode();
                    listDir(absPathCurDir, curDirItemsOffset);
                }
            }

            if (inp[2] == 'D') //left arrow key
            {
                moveCursor;
                if (!backwardDirsAbsPaths.empty())
                {
                    forwardDirsAbsPaths.push(absPathCurDir);
                    absPathCurDir = backwardDirsAbsPaths.top();
                    backwardDirsAbsPaths.pop();
                    resetCursorNormalMode();
                    listDir(absPathCurDir, curDirItemsOffset);
                }
            }
        }
        if (inp[0] == 'q')
        {
            exitRawMode();
            break;
        }
        if (inp[0] == ':')
        {
            enterCommandMode();
            listDir(absPathCurDir, curDirItemsOffset);
            resetCursorNormalMode();
        }

        if (inp[0] == 10)
        {
            //completePath is the fullpath
            string currentDirectory = curDirItemNames[xpos + curDirItemsOffset - 2];
            string completePath = absPathCurDir + "/" + currentDirectory;
            //printf("Trying to cd into : %s", completePath.c_str());
            //char *currPath=completePathc_str();

            invalidateForwardStack();
            struct stat s;
            stat(completePath.c_str(), &s);

            if ((s.st_mode & S_IFMT) == S_IFDIR)
            {
                if (currentDirectory == string("."))
                    ;

                else if (currentDirectory == string(".."))
                {
                    moveCursorXY(maxHeightForDirItems, 1);
                    cout << "path is " << absPathCurDir;
                    string name = splitPath(absPathCurDir);
                    completePath = name;
                    backwardDirsAbsPaths.push(absPathCurDir);
                    xpos = 1;
                    curDirItemsOffset = 1;
                    idxCurDirItemSelected = 1;
                    exitRawMode();
                    listDir(completePath, curDirItemsOffset);
                    enterRawMode();
                }
                else
                {
                    moveCursorXY(maxHeightForDirItems, 1);
                    //printf("Trying to cd into : %s",completePath);
                    moveCursor;
                    backwardDirsAbsPaths.push(string(absPathCurDir));
                    absPathCurDir = completePath;
                    xpos = 1;
                    curDirItemsOffset = 1;
                    idxCurDirItemSelected = 1;
                    exitRawMode();
                    listDir(absPathCurDir, curDirItemsOffset);
                    enterRawMode();
                }
            }
            else
            {
                char absPathToSelFile[1000];
                strcpy(absPathToSelFile, absPathCurDir.c_str());
                strcat(absPathToSelFile, "/");
                strcat(absPathToSelFile, curDirItemNames[xpos + curDirItemsOffset - 2].c_str());

                pid_t processid = vfork();
                if (processid < 0)
                {
                    //failed to fork
                }
                if (processid == 0)
                {
                    //child process code
                    fflush(stdin);
                    char execName[] = "vi";
                    char *exec_args[] = {execName, absPathToSelFile, NULL};
                    execv("/usr/bin/vi", exec_args);
                }
                else
                {
                    //parent process code
                    wait(NULL);
                    exitRawMode();
                    listDir(absPathCurDir, curDirItemsOffset);
                    enterRawMode();
                }
            }
        }

        if (inp[0] == 127) //backspace
        {
            absPathCurDir = backwardDirsAbsPaths.top();
            backwardDirsAbsPaths.pop();
            invalidateForwardStack();
            resetCursorNormalMode();
            listDir(absPathCurDir, curDirItemsOffset);
        }

        if (inp[0] == 'H' || inp[0] == 'h') //homekey
        {
            moveCursor;
            backwardDirsAbsPaths.push(absPathCurDir);
            while (!forwardDirsAbsPaths.empty())
                forwardDirsAbsPaths.pop();
            listDir(absPathCurDir, 1); // or should i store path of home in a global var and use that?
        }
    }
}

void exitRawMode()
{
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

void invalidateForwardStack()
{
    while (!forwardDirsAbsPaths.empty())
        forwardDirsAbsPaths.pop();
}

void resetCursorNormalMode()
{
    xpos = 1;
    ypos = vertCursorScrollPos;
    curDirItemsOffset = 1;
    idxCurDirItemSelected = 1;
}

void refreshDisplay()
{
    exitRawMode();
    listDir(absPathCurDir, curDirItemsOffset);
    enterRawMode();
}