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

/* This is called just before entering select. */
M__prepare_wait
{
    /* Reset select timeout. */
    self->timeout.tv_usec = self->timeout_us;

    W_DYNAMIC_ARRAY_FOR_EACH(struct WrestCoroutine*, task, self->blocked_tasks) {
        FD_SET(task->data /* has the blocking fd */, &self->writefds);
    }
}

/* This is called when select returns. */
M__handle_events
{
    if (status >= 1) {
        W_DYNAMIC_ARRAY_FOR_EACH(struct WrestCoroutine*, task, self->blocked_tasks) {
            if (FD_ISSET(task->data /* has the blocking fd */, &self->writefds))
                W_CALL(self,add_task)(task);
        }
    }

    /* Reset due times in timeouts. */
    struct WrestCoroutine** ready = NULL;
    struct WrestCoroutine** blocked = NULL;
    W_DYNAMIC_ARRAY_FOR_EACH(struct WrestTimeout*, timeout, self->timeouts) {
        if (timeout->due_us <= self->timeout_us) {
            timeout->due_us = timeout->usec;
            W_CALL(self,eval_task)(W_OBJECT_AS(timeout,WrestCoroutine),&ready,&blocked);
        } else {
            timeout->due_us -= self->timeout_us;
            W_DYNAMIC_ARRAY_PUSH(ready, W_OBJECT_AS(timeout,WrestCoroutine));
        }
    }

    W_DYNAMIC_ARRAY_FREE(self->timeouts);
    self->timeouts = (void*) ready;
    /* FIXME: Add blocking timeouts */
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
