#include "mem.h"
#include "list_frame.h"


int main(int argc, char *argv[])
{
    int rslt = 0;
    mempool_t mempool_for_test;

    if (0 != mempool_build(&mempool_for_test)) {
        rslt = -1;
        goto FINAL;
    }
    fprintf(stderr, "init success!\n");
    mempool_destroy(&mempool_for_test);

FINAL:
    return rslt;
}
