#include <stdio.h>

#include "logsys.h"

int main(int argc, char** argv)
{
    int i = 0;
    char szInfo[256] = {0};

    printf("log sys test\n");

    loginit("log.dat", LOG_WARNING);

    for (i = LOG_EMERGENCY; i <= LOG_DEBUG; i++) { 
        sprintf(szInfo, "log level%d", i);
        logout(i, szInfo, __FILE__, __LINE__);
    }

    return 0;
}