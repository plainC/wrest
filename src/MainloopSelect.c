#include "forward_declare.h"
#include "MainloopSelect.h"
#include "MainloopSelect_class.h"
#include <wondermacros/objects/x/class_start.h>

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

CONSTRUCT(MainloopSelect)
{
    W_CALL_ACONSTRUCT(Mainloop);
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
        self->timeout.tv_usec = self->timeout_usec;

        status = select(self->nfds, self->readfds, self->writefds,
            self->exceptfds, &self->timeout);

        W_DYNAMIC_ARRAY_FOR_EACH(struct ActivityRegular*, activity, self->activities) {
            if (activity->due <= self->timeout_usec) {
                W_CALL_VOID(activity,run);
                activity->due = activity->usec;
            } else
                activity->due -= self->timeout_usec;
        }
    } while (status >= 0);

}

#include <wondermacros/objects/x/class_end.h>
