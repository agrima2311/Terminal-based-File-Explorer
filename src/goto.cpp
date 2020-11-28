#include "../include/headers.h"

string goto_(vector<string> &token)
{
    if (token.size() < 2)
    {
        printf("less arguments in Goto\n");
    }
    return token[1];
}