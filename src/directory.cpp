#include "../include/headers.h"

int totFilesInDirectory;


/* Helper to convert size into human readable format */
char *humanReadableSize(uint64_t bytes)
{
    char suffix[][3] = {"B", "KB", "MB", "GB", "TB"};
    char length = sizeof(suffix) / sizeof(suffix[0]);

    int i = 0;
    double dblBytes = bytes;

    if (bytes > 1024)
    {
        for (i = 0; (bytes / 1024) > 0 && i < length - 1; i++, bytes /= 1024)
            dblBytes = bytes / 1024.0;
    }

    static char output[200];
    sprintf(output, "%.1lf%s", dblBytes, suffix[i]);
    return output;
}

int listDir(string absPathToCurDir, int itemOffset)
{
    printf("\033[H\033[J"); //clear screen
    curDirItemNames.clear();

    DIR *dirStream = opendir(absPathToCurDir.c_str());
    if (!dirStream)
    {
        return -1;
    }
    else
    {
        struct dirent *d;
        d = readdir(dirStream);
        while (d != NULL)
        {
            curDirItemNames.push_back(d->d_name);
            d = readdir(dirStream);
        }
        sort(curDirItemNames.begin(), curDirItemNames.end());

        int totItemsInDir = curDirItemNames.size();
        curTotalDirItems = totItemsInDir;

        // for(int i=0;i<totItemsInDir; i++) {
        //     printf("%s\n", curDirItemNames[i].c_str());
        // }

        for (int lineNo = 0, offsetCtr = itemOffset - 1; lineNo < maxHeightForDirItems && offsetCtr < totItemsInDir; offsetCtr++, lineNo++)
        {
            displayDirEntry(absPathToCurDir, curDirItemNames[offsetCtr]);
            printf("\n");
        }
    }

    printNormalModeBanner();

    return 1;
}

void displayDirEntry(string absPathToCurDir, string dirEntryName)
{
    struct stat thestat;

    string absPathToDirItem = absPathToCurDir + "/" + dirEntryName;
    // printf("%s\n", absPathToDirItem.c_str());
    // return ;

    //stat function retrieves info about the file,
    //returns file attributes on the inode
    stat(absPathToDirItem.c_str(), &thestat);

    //file type : S_IFMT flag
    switch (thestat.st_mode & S_IFMT)
    {
    case S_IFBLK:
        printf("b");
        break;
    case S_IFCHR:
        printf("c");
        break;
    case S_IFDIR:
        printf("d");
        break;
    default:
        printf("-");
        break;
    }
    //permissions
    if ((thestat.st_mode & S_IRUSR))
        printf("r");
    else
        printf("-");
    if ((thestat.st_mode & S_IWUSR))
        printf("w");
    else
        printf("-");
    if ((thestat.st_mode & S_IXUSR))
        printf("x");
    else
        printf("-");
    if ((thestat.st_mode & S_IRGRP))
        printf("r");
    else
        printf("-");
    if ((thestat.st_mode & S_IWGRP))
        printf("w");
    else
        printf("-");
    if ((thestat.st_mode & S_IXGRP))
        printf("x");
    else
        printf("-");
    if ((thestat.st_mode & S_IROTH))
        printf("r");
    else
        printf("-");
    if ((thestat.st_mode & S_IWOTH))
        printf("w");
    else
        printf("-");
    if ((thestat.st_mode & S_IXOTH))
        printf("x");
    else
        printf("-");

    //user owner
    string uname;
    if (getpwuid(thestat.st_uid) != NULL)
        uname = getpwuid(thestat.st_uid)->pw_name;
    printf(" %10s", uname.c_str());

    //group
    string gname;
    if (getgrgid(thestat.st_gid) != NULL)
        gname = getgrgid(thestat.st_gid)->gr_name;
    printf(" %10s", gname.c_str());

    //file size
    printf(" %10s", humanReadableSize(thestat.st_size));

    //file modification time
    char *modTime = ctime(&thestat.st_mtime);
    modTime[strlen(modTime) - 1] = '\0';
    printf(" %26s", modTime);

    //filename
    printf("  %s", dirEntryName.c_str());
}

void printNormalModeBanner()
{
    moveCursorXY(maxHeightForDirItems + 1, 1);
    printf(":: NORMAL MODE ::");
    moveCursor;
}