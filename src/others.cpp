#include "../include/headers.h"

string findCompletePath(string token)
{
    string currentdirpath = absPathCurDir;

    if (token[0] == '.' && token[1] != '\0')
        return currentdirpath + "/" + token.substr(2, token.length() - 1);

    // else if(token[0] == '/')
    // return currentdirpath + "/" + token.substr(2, token.length()-1);

    else if (token[0] == '~' && token[1] != '\0')
        return appHome + "/" + token.substr(2, token.length() - 1);

    else if (token[0] == '~')
        return appHome + "/";

    else
    {
        return currentdirpath;
    }
}
