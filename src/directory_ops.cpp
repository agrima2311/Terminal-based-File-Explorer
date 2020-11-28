#include "../include/headers.h"

//********************* create dir****************************

void create_dir(vector<string> &token)
{
    int numberOfTokens = token.size();
    if (numberOfTokens == 2)
    {
        string folderDestn = absPathCurDir + "/" + token[1];
        int ret = mkdir(folderDestn.c_str(), S_IRWXU | S_IRWXG);
        if (ret == -1)
        {
            printf("\nUnable to create file : %s", token[1].c_str());
        }
    }
    else
    {
        for (int i = 1; i < numberOfTokens - 1; i++)
        {
            string folderDestn = findCompletePath(token[numberOfTokens - 1]) + "/" + token[i];
            int ret = mkdir(folderDestn.c_str(), S_IRWXU | S_IRWXG);
            if (ret == -1)
            {
                printf("\nUnable to create directory : %s", token[i].c_str());
            }
        }
    }
}


//********************* delete dir****************************

void remove_dir(vector<string> &token)
{
    string dirPath = findCompletePath(token[1]);
    removeDirUtil(dirPath);
}

void removeDirUtil(string dirPath)
{
    // printf("directory to be deleted is %s\n", dirPath.c_str());
    DIR *d = opendir(dirPath.c_str());
    if (d)
    {
        struct dirent *dir;
        while ((dir = readdir(d)) != NULL)
        {
            // Skip the names "." and ".." as we don't have to recurse on them.
            if (!strcmp(dir->d_name, ".") || !strcmp(dir->d_name, ".."))
                ;
            else
            {
                string path = dirPath + "/" + dir->d_name;
               // printf("file or folder currently to be deleted is %s\n", path.c_str());
                struct stat st;

                if (stat(path.c_str(), &st) == -1)
                {
                    cout << "error can't open directory";
                    return;
                }
                if (S_ISDIR(st.st_mode))
                {
                   // printf("directory currently to be deleted is %s\n", path.c_str());

                    removeDirUtil(path);
                }
                else
                {
                   // printf("file currently to be deleted is %s\n", path.c_str());

                    removeFileUtil(path);
                }
            }
        }
        closedir(d);
        int ret = rmdir(dirPath.c_str());
        if (ret == -1)
        {
            cout << "error, cant remove directory name";
        }
    }
    else
    {
        cout << "error,cant open dir";
    }
}
