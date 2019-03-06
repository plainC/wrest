#ifdef EXPAND_C

CONSTRUCT(WrestMainloop)
{
    if (!self->timeout_us)
        self->timeout_us = 1000;

    if (!self->buffer_size)
        self->buffer_size = 256;

    wrest_log("Mainloop: created");
}

FINALIZE(WrestMainloop)
{
}

M__add_istream
{
}

M__eval_task
{
            switch ((task->status = W_CALL_VOID(task,eval))) {
            case WrestEvalStatusYield:
                W_DYNAMIC_ARRAY_PUSH(*ready, task);
                printf("@ %d\n",((struct { int value; }*) (task->context))->value);
                break;
            case WrestEvalStatusBlocked:
                W_DYNAMIC_ARRAY_PUSH(*blocked, task);
                break;
            case WrestEvalStatusError:
                wrest_log("Error: %s",strerror(task->data));
                W_CALL_VOID(task,free);
                break;
            case WrestEvalStatusReady:
                W_DYNAMIC_ARRAY_PUSH(*ready, task);
                break;
            case WrestEvalStatusCompleted:
                /* Fixme: chain the results. */
                W_CALL_VOID(task,free);
                break;
            default:
                wrest_log("Error: invalid status of task");
                break;
            }
}

M__run
{
    int status;

    do {

        /* Execute all tasks ready. */
        struct WrestCoroutine** ready = NULL;
        struct WrestCoroutine** blocked = NULL;

        W_DYNAMIC_ARRAY_FOR_EACH(struct WrestCoroutine*, task, self->ready_tasks) {
            W_CALL(self,eval_task)(task, &ready, &blocked);
        }

        W_DYNAMIC_ARRAY_FREE(self->ready_tasks);
        W_DYNAMIC_ARRAY_FREE(self->blocked_tasks);
        self->ready_tasks = ready;
        self->blocked_tasks = blocked;

        W_CALL_VOID(self,wait);

    } while (status >= 0);
}

M__wait
{
}

M__add_ostream
{
    W_DYNAMIC_ARRAY_PUSH(self->ostreams, ostream);
}

M__add_timeout
{
    W_DYNAMIC_ARRAY_PUSH(self->timeouts, timeout);
    timeout->due_us = timeout->usec;
}

M__add_task
{
    W_DYNAMIC_ARRAY_PUSH(self->ready_tasks, task);
}

#endif
