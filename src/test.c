#include "wrest.h"

int main()
{
    struct WrestMainloop* loop = W_NEW(WrestMainloopSelect, .buffer_size=1);
    struct WrestOutStream* ostream = W_NEW(WrestOutStream, .fd = 1, .loop = loop);
    W_CALL(ostream,write)("foobar\n",7);
    struct TestTimeout* t = W_NEW(TestTimeout, .loop = loop, .usec = 1000000);
    W_CALL(loop,add_ostream)(ostream);
    W_CALL(loop,add_timeout)(W_OBJECT_AS(t,WrestTimeout));

    W_CALL_VOID(loop,run);

    return 0;
}

