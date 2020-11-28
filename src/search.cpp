#include "../include/headers.h"

int searchUtil(string path, string name)
{
    DIR *d;
    struct dirent *dir;
    d = opendir(path.c_str());
    if (d == NULL)
    {
        printf("\nsorry, wrong path name %s", path.c_str());
        return 0;
    }
    //printf("searching in %s\n",path.c_str());

    while ((dir = readdir(d)) != NULL)
    {
        if (!strcmp(dir->d_name, ".") || !strcmp(dir->d_name, ".."))
            ;
        else
        {
            struct stat st;
            string finalPath = path + "/" + dir->d_name;
            if (stat(finalPath.c_str(), &st) == -1)
            {
                printf("error in stat\n");
                return 0;
            }
            if (S_ISDIR(st.st_mode))
            {
                if (!strcmp(dir->d_name, name.c_str()))
                {
                    // printf("found");
                    return 1;
                }
                else
                {
                    if (searchUtil(finalPath, name) == 1)
                        return 1;
                }
            }
            else
            {
                if (!strcmp(dir->d_name, name.c_str()))
                {
                    //printf("found");
                    return 1;
                }
            }
        }
    }
    return 0;
}

int search(vector<string> &token)
{
    if (token.size() < 2)
    {
        printf("invalid number of arguments\n");
        return 0;
    }
    string path = absPathCurDir; // /home/foo/Desktop   token[1] is L
                                 // printf("\nabs path is %s\n", absPathCurDir.c_str());
    return searchUtil(path, token[1]);
}