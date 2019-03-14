#ifdef EXPAND_C

CONSTRUCT(UVtcpClient)
{
    if (!self->loop)
        printf("ERROR: Loop not set\n");
    else
        uv_tcp_init((uv_loop_t*) self->loop->loop, &self->handle);
}

static void
on_read(uv_stream_t *server, ssize_t nread, const uv_buf_t* buf)
{
    struct UVtcpClient__private* self = (struct UVtcpClient__private*) server->data;

    if (nread == -1) {
        W_EMIT(self,on_error, "Read error");
        return;
    }

    write(1, buf->base, nread);
    write(1, "\n", 1);
    free(buf->base);
    uv_stop((uv_loop_t*) self->loop->loop);
}

static void
on_close(uv_handle_t* handle)
{
}

static void
on_alloc_buffer(uv_handle_t* handle, size_t size, uv_buf_t* buf)
{
    buf->base = malloc(size);
    buf->len = size;
}

static void
on_write_end(uv_write_t *req, int status)
{
    struct UVtcpClient__private* self = (struct UVtcpClient__private*) req->data;

    if (status == -1) {
        W_EMIT(self,on_error, uv_strerror(status));
        return;
    }
    req->handle->data = self;
    uv_read_start(req->handle, on_alloc_buffer, on_read);
    free(req);
}

static void
on_connection(uv_connect_t* req, int status)
{
    struct UVtcpClient__private* self = (struct UVtcpClient__private*) req->data;

    if (status < 0) {
        W_EMIT(self,on_error, uv_strerror(status));
        return;
    }

    char buffer[strlen(self->message)+1+1000];
    uv_buf_t buf = uv_buf_init(buffer, sizeof(buffer));

    buf.len = strlen(self->message);
    buf.base = (char*) self->message;
    uv_write_t* write_req = malloc(sizeof(uv_write_t));
    write_req->data = self;
    uv_write(write_req, req->handle, &buf, 1, on_write_end);
}

M__connect
{
    self->conn = malloc(sizeof(uv_connect_t));

    if (!self->address)
        return 1;
    if (!self->port)
        return 2;

    if (uv_ip4_addr(self->address, self->port, &self->addr))
        return 3;

    self->conn->data = self;
    self->message = message;
    self->len = len;

    if (uv_tcp_connect(self->conn, &self->handle,
        (const struct sockaddr*) &self->addr, on_connection))
        return 4;

    return 0;
}

#endif
