#ifdef EXPAND_C

#ifndef WDEBUG_EXPAND
# include <unistd.h>
# include <errno.h>
#endif

METHOD(WrestWrite,public,int,eval)
{
    W_CCR_CONTEXT_BEGIN;
        int pos;
    W_CCR_CONTEXT_END;
    W_CCR_BEGIN;
        CTX->pos = 0;

        int status;
        do {
            errno = 0;
            status = write(self->ostream->fd, self->buffer + CTX->pos,
                W_MIN(self->size,self->ostream->loop->buffer_size));
            if (status < 0) {
                if (errno == EAGAIN) {
                    W_CCR_BLOCK(self->ostream->fd);
                    status = -1;
                } else {
                    W_CCR_ERROR(errno);
                }
            } else
                CTX->pos += status;
        } while (CTX->pos < self->size);

    W_CCR_END;
}

#endif

