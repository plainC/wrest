#ifdef EXPAND_C

CONSTRUCT(UVtcpServer)
{
    if (!self->loop)
        printf("ERROR: Loop not set\n");
    else
        uv_tcp_init((uv_loop_t*) self->loop->loop, &self->handle);
}

M__bind
{
    if (!self->address)
        return 1;
    if (!self->port)
        return 2;

    if (uv_ip4_addr(self->address, self->port, &self->addr))
        return 3;

    self->handle.data = self;

    return uv_tcp_bind(&self->handle, (const struct sockaddr*) &self->addr, flags);
}

uv_stream_t* client_handle;

static void
on_alloc_buffer(uv_handle_t *handle, size_t size, uv_buf_t *buf)
{
    buf->base = malloc(size);
    buf->len = size;
}

static void
on_write(uv_write_t *req, int status)
{
    struct UVtcpServer* self = (struct UVtcpServer*) req->data;

    if (status)
        W_EMIT(self,on_error, uv_strerror(status));
    else
        W_EMIT(self,on_connection_close, "");

    uv_close((uv_handle_t*) client_handle, NULL);
    free(req);
}

static void
on_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf)
{
    struct UVtcpServer* conn = (struct UVtcpServer*) client->data;

    if (nread < 0) {
        if (nread != UV_EOF) {
            uv_close((uv_handle_t*) client, NULL);
            W_EMIT(W_OBJECT_AS(conn,UVtcpServer),on_error, uv_err_name(nread));
        }
    } else if (nread > 0) {
        char* response = NULL;
        size_t response_size;

        W_EMIT(W_OBJECT_AS(conn,UVtcpServer),on_data, buf->base, nread, &response, &response_size);

        if (!response) {
            response = strdup("Ok");
            response_size = 2;
        }

        uv_write_t *req = (uv_write_t *) malloc(sizeof(uv_write_t));
        uv_buf_t write_buf = uv_buf_init(response, response_size);
        req->data = W_OBJECT_AS(conn,UVtcpServer);
        client_handle = client;
        uv_write(req, client, &write_buf, 1, on_write);
        free(response);
    }

    if (buf->base)
        free(buf->base);
}

static void
on_new_connection(uv_stream_t *server, int status)
{
    struct UVtcpServer* self = (struct UVtcpServer*) server->data;

    if (status < 0) {
        W_EMIT(self,on_error, uv_strerror(status));
        return;
    } else
        W_EMIT(self,on_connection_open, "connection");

    uv_tcp_t *client = (uv_tcp_t*) malloc(sizeof(uv_tcp_t));
    if (!client) {
        W_EMIT(self,on_error, "Unable to allocate");
        return;
    }
    uv_tcp_init((uv_loop_t*) self->loop->loop, client);
    client->data = self;

    if (uv_accept(server, (uv_stream_t*) client) == 0)
        uv_read_start((uv_stream_t*) client, on_alloc_buffer, on_read);
    else {
        uv_close((uv_handle_t*) client, NULL);
        W_EMIT(self,on_error, "uv_accept failed");
    }
}

M__listen
{
    return uv_listen((uv_stream_t*)&self->handle, backlog, on_new_connection);
}

#endif
