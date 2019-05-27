#include <stdio.h>
#include <libgen.h>
#include <string.h>

#define MAX_DIR_DEPTH 5

typedef struct {
    char* dirArray[MAX_DIR_DEPTH];
    int dirDepth; // if root dir, dir depth = 0
    char* base;
} FilePath;

//----------------------------------------------------------------------
// pathParser
//    Extract the filePath into dirname and base name.
//    The retuen value is using the call by reference.
// 
//    filePath: "/foo/bar/baz.txt"
//    dir: ["foo", "bar"]
//    base: "baz.txt"
//----------------------------------------------------------------------

FilePath
pathParser(char* path)
{
    if (path[0] == '/')
        path = &path[1]; // Don't count the first '/'

    char* ts1 = strdup(path);
    char* ts2 = strdup(path);

    FilePath filepath;

    // The return of basename() will be the copy pointer of input!!
    char* currentDir = dirname(ts1);
    filepath.base = strdup(basename(ts2)); 

    // See how depth the path is
    int depth;
    for (depth = 0; path[depth]; path[depth] == '/' ? depth++ : *path++);
    filepath.dirDepth = depth;

    // Not in current directory. Travel to the directory
    while (strcmp(currentDir, ".")) { // while currentDir is not "."
        filepath.dirArray[--depth] = strdup(basename(currentDir));
        currentDir = dirname(currentDir);
    }

    return filepath;
}

void pathParserDebuger(FilePath filepath)
{
    for (int i = 0; i <  filepath.dirDepth; i++) {
        printf("%s/", filepath.dirArray[i]);
    }
    printf("%s\n", filepath.base);
}

int main()
{
    char* path1 = "/foo/bar/baz.txt";
    char* path2 = "test/haha/hehe";
    char* path3 = "/testhaha";
    char* path4 = "WhoCopyMyHomeworkPleaseGiveMeAStar!";

    pathParserDebuger(pathParser(path1));
    pathParserDebuger(pathParser(path2));
    pathParserDebuger(pathParser(path3));
    pathParserDebuger(pathParser(path4));

    return 0;
}
