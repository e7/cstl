#include "mem.h"
#include "list.h"


int main(int argc, char *argv[])
{
    int rslt = 0;

    if (0 != mempool_init()) {
        rslt = -1;
        goto FINAL;
    }
    fprintf(stderr, "init success!\n");
    mempool_exit();

FINAL:
    return rslt;
}
