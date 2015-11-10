#include <stdio.h>
#include <stdlib.h>

#include "nbDataTransfer.h"

int main(int argc, char** argv)
{
    CNbDataTransfer testInst;
    testInst.SetParameter("192.168.1.245", 80,
        "admin", "redwood", "192.168.1.161", 2222, 5, 0);

    testInst.oneOp();

    return 0;
}