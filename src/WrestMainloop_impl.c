#ifdef EXPAND_C

CONSTRUCT(WrestMainloop)
{
    if (!self->timeout_us)
        self->timeout_us = 1000;

    wrest_log("Mainloop: created");
}

FINALIZE(WrestMainloop)
{
}

M__add_istream
{
}

M__run
{
    int status;

    do {

        W_DYNAMIC_ARRAY_FOR_EACH(struct WrestCoroutine*, task, self->tasks) {
            if (task->completed)
                continue;

            switch (W_CALL_VOID(task,eval)) {
            case WrestEvalStatusYield:
                printf("@=%d\n",((struct { int value; }*) (task->context))->value);
                break;
            case WrestEvalStatusFailed:
                wrest_log("Error: task failed");
                /* Fall through */
            case WrestEvalStatusOk:
                task->completed = 1;
                break;
            default:
                wrest_log("Error: invalid status of task");
                break;
            }
        }

        W_CALL_VOID(self,wait);

    } while (status >= 0);
}

M__wait
{
}

M__add_ostream
{
}

M__add_timeout
{
    W_DYNAMIC_ARRAY_PUSH(self->timeouts, timeout);
    timeout->due_us = timeout->usec;
}

M__add_task
{
printf("Add task:%d\n", task->completed);
    W_DYNAMIC_ARRAY_PUSH(self->tasks, task);
}

#endif
