#ifdef EXPAND_C

CONSTRUCT(WrestFib)
{
    self->completed = 0;
}

METHOD(WrestFib,public,int,eval)
{
    W_CCR_CONTEXT_BEGIN;
        int p;
        int n;
        int i;
        int max;
    W_CCR_CONTEXT_END;
    W_CCR_BEGIN;
        CTX->max = 46;
        int fib = CTX->p = CTX->n = 0;
        W_CCR_YIELD(fib);
        fib = CTX->p = CTX->n = 1;
        W_CCR_YIELD(fib);
        for (CTX->i=2; CTX->i<CTX->max; ++CTX->i) {
            fib = CTX->p+CTX->n;
            CTX->p = CTX->n;
            CTX->n = fib;
            W_CCR_YIELD(fib);
        }
    W_CCR_END;
}

#endif
