#ifdef EXPAND_C

CONSTRUCT(UVloop)
{
    self->loop = uv_default_loop();
}

M__run
{
    return uv_run(self->loop, mode);
}

M__stop
{
    uv_stop(self->loop);
}

M__now
{
    return uv_now(self->loop);
}

#endif

