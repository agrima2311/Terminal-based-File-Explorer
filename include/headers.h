#include <bits/stdc++.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include<sys/wait.h> 
#include <sys/types.h>
#include <fcntl.h>


#define moveCursor printf("%c[%d;%dH", 27, xpos, ypos)
#define moveCursorNormMode printf("%c[%d;%dH", 27, xpos, vertCursorScrollPos)
#define moveCursorXY(x, y) printf("%c[%d;%dH", 27, x, y)


using namespace std;

extern int xpos; //Current X coordinate of cursor (Absolute)
extern int ypos; //Current Y coordinate of cursor (Absolute)

extern string absPathCurDir; // Absolute path of current directory that is opened at any instance
extern int idxCurDirItemSelected; //Index of directory item currently selected by the cursor
extern struct winsize termSize;
extern int maxHeightForDirItems; //Max height to populate directory items
extern int curDirItemsOffset;
extern int curTotalDirItems;
extern const int vertCursorScrollPos;
extern string appHome;

extern stack<string> backwardDirsAbsPaths;
extern stack<string> forwardDirsAbsPaths;
extern vector<string> curDirItemNames;




void initTerminalDimensions();
void initNormalModeCursorPos();
void initCommandModeCursorPos();
void displayDirEntry(string absPathToCurDir, string dirEntryName);
int listDir(string absPathToCurDir, int itemOffset);
void enterRawMode();
void exitRawMode();
void window_resize_handler(int signal);
void dashboard();
void printNormalModeBanner();
string splitPath(string str);
void invalidateForwardStack();
void refreshDisplay();
void resetCursorNormalMode();
void enterCommandMode();
void create_dir(vector <string> &token);
void create_file(vector <string> &token);
void rename_file(vector<string> &token);
void remove_file(vector<string> &token);
void removeFileUtil(string fileName);
void remove_dir(vector<string> &token);
void removeDirUtil(string dirPath);
string findCompletePath(string token);
void copy(vector <string> &token);
string goto_(vector <string> &token);
int search( vector <string> &token);
bool isDir(string path);
void copyFile(string sourcePath, string destnPath);
void copyDir(string sourcePath, string destnPath);
void moveFile(vector <string> &token);
