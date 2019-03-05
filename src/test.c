#include "wrest.h"

int main()
{
    struct WrestMainloop* loop = W_NEW(WrestMainloopSelect);
    struct WrestCoroutine* cr = W_NEW(WrestFib);

    W_CALL(loop,add_task)(cr);

    W_CALL_VOID(loop,run);

    return 0;
}

