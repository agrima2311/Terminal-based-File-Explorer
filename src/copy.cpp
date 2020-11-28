#include "../include/headers.h"


//*********CHECK IF FILE OR DIRECTORY*************

bool isDir(string path)
{
    //string path = absPathCurDir + "/" + name;
    struct stat st;
    if (stat(path.c_str(), &st) == -1)
    {
        printf("wrong name\n");
        return false;
    }
    else
    {
        if ((S_ISDIR(st.st_mode)))
            return true;
        else
            return false;
    }
    return false;
}


//*************USED TO COPY FILE********************

void copyFile(string sourcePath, string destnPath)
{
    struct stat sourceInfo, destnInfo;
    int r;
    char writeBlock[1024];

    int inret = open(sourcePath.c_str(), O_RDONLY);
    int opret = open(destnPath.c_str(), O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    if (stat(sourcePath.c_str(), &sourceInfo) == -1)
    {
        printf("error in opening file\n");
    }
    if (stat(destnPath.c_str(), &destnInfo) == -1)
    {
        printf("error in opening destination dir\n");
    }
    while (r = read(inret, writeBlock, sizeof(writeBlock)) > 0)
    {
        write(opret, writeBlock, r);
    }
    chown(destnPath.c_str(), sourceInfo.st_uid, sourceInfo.st_gid);
    chmod(destnPath.c_str(), sourceInfo.st_mode);
}


//*********COPY DIRECTORY*************************************
void copyDir(string sourcePath, string destnPath)
{
    DIR *d;
    struct dirent *dir;
    int ret = mkdir(destnPath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    d = opendir(sourcePath.c_str());
    if (d == NULL)
    {
        cout << "error in opening source directory\n";
        return;
    }
    while ((dir = readdir(d)) != NULL)
    {
        if (!strcmp(dir->d_name, ".") || !strcmp(dir->d_name, ".."))
            ;
        else
        {
            // string name = dir->d_name;
            string currentfilePath = sourcePath + "/" + dir->d_name;
            // printf("current file path is %s\n", currentfilePath.c_str());
            string destPathForEachFile = destnPath + "/" + dir->d_name;
            //printf("destination for each file is %s \n", destPathForEachFile.c_str());
            //string destPathForEachFile = fullDestnPath ;
            struct stat st;
            if (stat(currentfilePath.c_str(), &st) == -1)
            {
                cout << "error 2";
                return;
            }

            if (S_ISDIR(st.st_mode))
            {
                //current = dirName + "/" + dir-> d_name;
                copyDir(currentfilePath, destPathForEachFile);
            }
            else
            {
                copyFile(currentfilePath, destPathForEachFile);
            }
        }
    }
    //close dir
    // closedir(d);
}


//****** THIS FUNCTION CHECKS ID THE GIVEN INPUT IS FILE OR DIR********

void copy(vector<string> &token)
{
    int noOftokens = token.size();
    if (noOftokens < 3)
    {
        printf("Number of tokens less than 3! Error\n");
        return;
    }

    for (int i = 1; i < noOftokens - 1; i++)
    {
        string directoryPath = token[noOftokens - 1];
        string destnPath = findCompletePath(directoryPath) + "/" + token[i];
        //  printf("destination path is %s \n", destnPath.c_str());

        string sourcePath = absPathCurDir + "/" + token[i];
        // printf("sourcepath is %s\n", sourcePath.c_str());
        // check if token[i] is a file
        if (isDir(sourcePath) == false)
        {
            // printf("it is a file \n");
            copyFile(sourcePath, destnPath);
        }
        else if (isDir(sourcePath) == true)
        {
            // printf("it is a directory \n");
            // destnPath = destnPath + "/" + token[i];
            //  printf("destination path is %s \n", destnPath.c_str());
            copyDir(sourcePath, destnPath);
        }
    }
}
// copy file.txt ~/file2.txt
// /home/foo/Desktop/file2.txt
// /home/foo/Desktop/file.txt

// copy X Y ~/Z
// destination path is /home/foo/Desktop/Z/X
// /home/foo/Desktop/X
//
