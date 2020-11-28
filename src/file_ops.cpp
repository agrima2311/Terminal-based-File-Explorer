#include "../include/headers.h"

//***************************create file***************************


void create_file(vector<string> &token)
{
    int numberOfTokens = token.size();
    if (numberOfTokens == 2)
    {
        string destinationPath = absPathCurDir + "/" + token[1];
        int ret = creat(destinationPath.c_str(), O_RDONLY | O_CREAT | S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        if (ret == -1)
        {
            printf("\nUnable to create file : %s", token[1].c_str());
        }
    }
    else
    {
        for (int i = 1; i < numberOfTokens - 1; i++)
        {
            string fileName = token[i];
            string destinationPath = findCompletePath(token[numberOfTokens - 1]);
            destinationPath = destinationPath + "/" + fileName;
            int ret = creat(destinationPath.c_str(), O_RDONLY | O_CREAT | S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
            if (ret == -1)
            {
                printf("\nUnable to create file : %s", fileName.c_str());
            }
        }
    }
}

//***************************rename file***************************

void rename_file(vector<string> &token)
{
    string oldFileName = token[1];
    string newFileName = token[2];

    oldFileName = absPathCurDir + "/" + oldFileName;
    newFileName = absPathCurDir + "/" + newFileName;

    int ret = rename(oldFileName.c_str(), newFileName.c_str());
    if (ret == -1)
    {
        cout << " reeor in renaming";
    }
}


//***************************delete file***************************

void remove_file(vector<string> &token)
{
    string fileName = findCompletePath(token[1]);
    removeFileUtil(fileName);
}

void removeFileUtil(string fileName)
{
    int ret = remove(fileName.c_str());
    if (ret == -1)
    {
        cout << "error in removing file ";
    }
}