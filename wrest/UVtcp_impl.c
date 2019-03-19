#ifdef EXPAND_C

CONSTRUCT(UVtcp)
{
    if (self->address)
        self->address = strdup(self->address);

    if (!self->loop)
        printf("ERROR: Loop not set\n");
    else {
        if (uv_tcp_init((uv_loop_t*) self->loop->loop, &self->handle))
            printf("ERROR: uv_tcp_init\n");
        else
            printf("tcp_init ok.\n");
    }
}

M__parse_address
{
    /**/

    if (!self->address)
        return 1;

    char* colon = strchr(self->address, ':');
    char* dot = strchr(self->address, '.');
    char* bracket = strchr(self->address, '[');
    char* close_bracket;

    if (dot)
        goto ipv4;

    if (bracket && colon)
        goto ipv6;

    if (colon && !dot)
        goto ipv6;

    return 3;

    /**/

  ipv4:
    if (colon) {
        self->port = atoi(colon+1);
        *colon = '\0';
    }

    if (!self->port)
        return 2;

    printf("Bind to IPv4: %s:%d\n", self->address, self->port);
    if (uv_ip4_addr(self->address, self->port, &self->addr))
        return 3;

    return 0;

    /**/

  ipv6:
    close_bracket = strchr(self->address, ']');
    if ((bracket && !close_bracket) || (!bracket && close_bracket))
        return 4;
    if (close_bracket) {
        colon = strchr(close_bracket+1,':');
        if (colon) {
            self->port = atoi(colon+1);
            self->address++;
            *close_bracket = '\0';
        }
    }

    if (!self->port)
        return 2;

    printf("Bind to IPv6: [%s]:%d\n", self->address, self->port);
    if (uv_ip6_addr(self->address, self->port, (struct sockaddr_in6*) &self->addr))
        return 3;

    /**/

    return 0;
}


#endif

