#include <iostream>
// #include <malloc/malloc.h>
#include "fileOperator.h"
#include "maketree.h"
// #include <unistd.h>

int main()
{
    char *freader;
    std::string path = "/Users/malt/Desktop/expressions.txt";
    std::string path_O = "/Users/malt/Desktop/results.txt";
    freader = foRead((char *)path.data());
    printf("%s \n\n", freader);
    PSTLOGIC plogicRoot = makeTreeByExpr(freader);
    int countAndShortcuts = 0, countOrShortcuts = 0;
    char reChar = AcculateTree(plogicRoot, countAndShortcuts, countOrShortcuts);
    printf("%c \n%d, %d\n", reChar, countAndShortcuts, countOrShortcuts);
    std::string chars = std::string(&reChar).append("\n");   
    std::string CAnd = std::to_string(countAndShortcuts).append(", ").append(std::to_string(countOrShortcuts));
    // printf("%s",(char*)chars.append(CAnd).data());
    foWrite((char *)path_O.data(), (char*)chars.append(CAnd).data());
    // printTree(plogicRoot,0);
    freeTree(plogicRoot);
    foClose();
}