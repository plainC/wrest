#ifdef EXPAND_C

CONSTRUCT(UVtcpServer)
{
    W_CALL_ACONSTRUCT(UVtcp);
}

M__bind
{
    int status = W_CALL_VOID(self,parse_address);

    if (status) {
        printf("Parse address failed: %d\n", status);
        return status;
    }

    self->handle.data = self;

    return uv_tcp_bind(&self->handle, (const struct sockaddr*) &self->addr, flags);
}

static uv_stream_t* client_handle;

static void
on_alloc_buffer(uv_handle_t *handle, size_t size, uv_buf_t *buf)
{
    buf->base = malloc(size);
    buf->len = size;
}

static void
on_close(uv_handle_t* handle)
{
    free(handle);
}

static void
on_write(uv_write_t *req, int status)
{
    struct UVtcpServer* self = (struct UVtcpServer*) req->data;

    if (status)
        W_EMIT(self,on_error, uv_strerror(status));
    else
        W_EMIT(self,on_connection_close, "");

    uv_close((uv_handle_t*) client_handle, on_close);
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

        struct sockaddr_storage name={0};
        int namelen=0;
        uv_tcp_getsockname((const uv_tcp_t*) client, (struct sockaddr*) &name,&namelen);

        struct sockaddr_in6* a = (struct sockaddr_in6*) &name;
        char ip[17]={0};
        uv_ip4_name((const struct sockaddr_in *) &a->sin6_addr,ip,sizeof(ip));

        W_EMIT(W_OBJECT_AS(conn,UVtcpServer),on_data, buf->base, nread, ip, &response, &response_size);

        if (!response) {
            response = strdup("Ok");
            response_size = 2;
        }

        uv_write_t *req =(uv_write_t *) malloc(sizeof(uv_write_t));
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

    if (uv_accept(server, (uv_stream_t*) client) == 0) {
        uv_read_start((uv_stream_t*) client, on_alloc_buffer, on_read);
    } else {
        uv_close((uv_handle_t*) client, NULL);
        W_EMIT(self,on_error, "uv_accept failed");
    }
}

M__listen
{
    return uv_listen((uv_stream_t*)&self->handle, backlog, on_new_connection);
}

#endif
