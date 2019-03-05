#include "wrest.h"

int main()
{
    struct WrestMainloop* loop = W_NEW(WrestMainloopSelect);
    struct TestTimeout* t = W_NEW(TestTimeout, .loop = loop, .usec = 1000000);

    W_CALL(loop,add_timeout)(W_OBJECT_AS(t,WrestTimeout));

    W_CALL_VOID(loop,run);

    return 0;
}

