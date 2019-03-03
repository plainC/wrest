#include "wrest.h"

void timeout_cb(void* self)
{
    printf("Here\n");
}

int main()
{
    struct Mainloop* mainloop = W_NEW(MainloopSelect);
    struct ActivityRegular* cb = W_NEW(ActivityRegularCb,
        .name="regular", .cb = timeout_cb, .usec = 1000000
    );
    W_CALL(mainloop,add)(cb);
    W_CALL_VOID(mainloop,run);
}
