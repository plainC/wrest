#ifdef EXPAND_C

#ifndef WDEBUG_EXPAND
# include <unistd.h>
# include <fcntl.h>
#endif

CONSTRUCT(WrestOutStream)
{
    int flag = fcntl(self->fd, F_GETFL, 0);
    flag |= O_NONBLOCK;
    fcntl(self->fd, F_SETFL, flag);
}

M__write
{
   W_CALL(self->loop,add_task)(W_NEW(WrestWrite,
       .ostream=W_OBJECT_AS(self, WrestOutStream), .buffer=buffer, .size=size));
}

#endif

