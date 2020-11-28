#include "../include/headers.h"

void moveFile(vector<string> &token)
{
    for (int i = 1; i < token.size() - 1; i++)
    {
        string destnPath = findCompletePath(token[token.size() - 1]) + "/" + token[i];
        // destnpath = destnpath + "/" + token[i];
        string sourcePath = absPathCurDir + "/" + token[i];

        if (isDir(sourcePath) == true) //cehck parameters
        {
            //printf("it is a directory \n");
            copyDir(sourcePath, destnPath);
            removeDirUtil(sourcePath);
        }
        else
        {
            //printf("it is a file \n");
            copyFile(sourcePath, destnPath);
            removeFileUtil(sourcePath);
        }
    }
}