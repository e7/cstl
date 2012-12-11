#include "error_info.h"


static int build_cstl(void)
{
    return 0;
}

static void destroy_cstl(void)
{
    return;
}

int main(int argc, char *argv[])
{
    int rslt = 0;
    
    rslt = build_cstl();
    if (E_OK == rslt) {
        rslt = cstl_main(argc, argv);
    }
    destroy_cstl();

    return rslt;
}
