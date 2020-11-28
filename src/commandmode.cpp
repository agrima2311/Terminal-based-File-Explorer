#include "../include/headers.h"

void processIntoTokens(string inpCommand, vector<string> &token)
{
    // Used to split string around spaces.
    string word = "";
    for (auto x : inpCommand)
    {
        if (x == ' ')
        {
            token.push_back(word);
            word = "";
        }
        else
        {
            word = word + x;
        }
    }
    token.push_back(word);
}

void enterCommandMode()
{
    char ch;

    initCommandModeCursorPos();
    moveCursor;
    printf("\033[J");

    while (1)
    {
        //waiting for user to finish writing command
        ypos = 1;
        printf(":");
        ypos++;

        string inpCommand;
        while (1)
        {
            fflush(stdin);
            ch = getchar();
            if (ch == 10 || ch == 27)
                break;
            if (ch == 127)
            {
                if (inpCommand.length() > 0)
                {
                    ypos--;
                    inpCommand = inpCommand.substr(0, inpCommand.length() - 1);
                    xpos = inpCommand.length() - 1;
                    printf("\b");
                    printf("\033[J");
                }
            }
            else
            {
                inpCommand = inpCommand + ch;
                printf("%c", ch);
            }
        }
        if (ch == 10)
        {
            //process
            vector<string> params;
            processIntoTokens(inpCommand, params); //split string into tokens
            if (params[0] == "create_dir")
            {
                create_dir(params);
            }
            else if (params[0] == "delete_dir")
            {
                remove_dir(params);
            }
            else if (params[0] == "create_file")
            {
                create_file(params);
            }
            else if (params[0] == "rename")
            {
                rename_file(params);
            }
            else if (params[0] == "delete_file")
            {
                remove_file(params);
            }
            else if (params[0] == "copy")
            {
                copy(params);
            }
            else if (params[0] == "move")
            {
                moveFile(params);
            }
            else if (params[0] == "search")
            {
                if (search(params) == 1)
                    printf("\ntrue\n");
                else
                    printf("\nfalse\n");
            }
            else if (params[0] == "goto")
            {
                string location = goto_(params);
                backwardDirsAbsPaths.push(absPathCurDir);
                absPathCurDir = appHome + "/" + location.substr(2, location.length() - 1);
                invalidateForwardStack();
                resetCursorNormalMode();
                refreshDisplay();
            }

            else
            {
                printf("\nWrong command, Try again!");
            }
        }
        else if (ch == 27)
        {
            //back to normal mode
            return;
        }

        printf("\n");
    }
    //return 0;
}