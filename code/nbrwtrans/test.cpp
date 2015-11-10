
#define DLL_NBRWTRANS_API __declspec(dllexport)

#include "test.h"

#include <stdio.h>

DLL_NBRWTRANS_API int testadd(int a, int b)
{
    return a + b;
}

void mypoint::printPt()
{
    printf("This is a test\n");
}