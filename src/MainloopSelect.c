#include "forward_declare.h"
#include "MainloopSelect.h"
#include "MainloopSelect_class.h"
#include <wondermacros/objects/x/class_start.h>

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

CONSTRUCT(MainloopSelect)
{
}

FINALIZE(MainloopSelect)
{
}

/* Override: run */
METHOD(MainloopSelect,public,void,run)
{
    W_CALL_STATIC_VOID(Mainloop,self,run);
    int status;
    do {
        self->timeout.tv_sec = 1;

        status = select(self->nfds, self->readfds, self->writefds,
            self->exceptfds, &self->timeout);

        W_DYNAMIC_ARRAY_FOR_EACH(struct ActivityRegular*, activity, self->activities) {
            activity->due -= 1;
            if (activity->due <= 0) {
                W_CALL_VOID(activity,run);
                activity->due = activity->sec;
            }
        }
    } while (status >= 0);

}

#include <wondermacros/objects/x/class_end.h>
