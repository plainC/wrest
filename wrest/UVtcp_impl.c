#ifdef EXPAND_C

CONSTRUCT(UVtcp)
{
printf("Initialize handle\n");
    uv_tcp_init((uv_loop_t*) self->loop->loop, &self->handle);
}

#endif

