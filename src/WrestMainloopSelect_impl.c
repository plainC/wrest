#ifdef EXPAND_C

#ifndef WDEBUG_EXPAND
# include <sys/time.h>
# include <sys/types.h>
# include <unistd.h>
#endif

CONSTRUCT(WrestMainloopSelect)
{
    FD_ZERO(&self->readfds);
    FD_ZERO(&self->writefds);
    FD_ZERO(&self->exceptfds);

    W_CALL_ACONSTRUCT(WrestMainloop);
}

FINALIZE(WrestMainloopSelect)
{
}

M__prepare_wait
{
    self->timeout.tv_usec = self->timeout_us;
}

M__handle_events
{
    if (status >= 1) {
    }

    W_DYNAMIC_ARRAY_FOR_EACH(struct WrestTimeout*, timeout, self->timeouts) {
        if (timeout->completed)
            continue;

        if (timeout->due_us <= self->timeout_us) {
            timeout->due_us = timeout->usec;
            timeout->completed = W_CALL_VOID(timeout,eval);
        } else
            timeout->due_us -= self->timeout_us;
    }
}

METHOD(WrestMainloopSelect,private,int,wait)
{
    W_CALL_VOID(self,prepare_wait);
    int status = select(self->nfds, &self->readfds, &self->writefds,
            &self->exceptfds, &self->timeout);
    W_CALL(self,handle_events)(status);

    return 0;
}

#endif

