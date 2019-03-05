#ifdef EXPAND_C

CONSTRUCT(TestTimeout)
{
}

FINALIZE(TestTimeout)
{
}

METHOD(TestTimeout,private,int,eval)
{
    W_CALL(self->loop,add_task)(W_NEW(WrestFib));

    return 0;
}

#endif
