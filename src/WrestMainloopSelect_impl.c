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

#endif

