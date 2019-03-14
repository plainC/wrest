#ifdef EXPAND_C

static void
data_cb(struct RestServer* self, void* context, const char* data, size_t length,
        char** response, size_t* response_size);

CONSTRUCT(RestServer)
{
    W_CALL_CONSTRUCT(UVtcpServer);

    W_OBJECT_SIGNAL_TYPE* handle;
    W_CONNECT(self,on_data, data_cb, handle);
}

static int
parse_http(const char* data, size_t size, int* mark_command, int* mark_uri, int* mark_body)
{
    enum { COMMAND, URI, BODY, DONE };
    int state = COMMAND;

    for (int i=0; i < size && state != DONE; i++) {
        switch (state) {
        case COMMAND:
            if (data[i] == ' ') {
                state = URI;
                *mark_command = i;
            }
            break;
        case URI:
            if (data[i] == ' ') {
                state = BODY;
                *mark_uri = i;
            }
            break;
        case BODY:
            if (data[i] == '\r' && i+4 < size && data[i+1] == '\n' &&
                data[i+2] == '\r' && data[i+3] == '\n') {
                state = DONE;
                *mark_body = i+4;
            }
            break;
        }
    }
    if (state == BODY) {
        *mark_body = 0;
        state = DONE;
    }

    if (state != DONE || *mark_uri-*mark_command == 1)
        return 1;

    return 0;
}

static void
data_cb(struct RestServer* self, void* context, const char* data, size_t length,
        char** response, size_t* response_size)
{
    int mark_command;
    int mark_uri;
    int mark_body;

    if (parse_http(data, length, &mark_command, &mark_uri, &mark_body)) {
        W_EMIT(self,on_error, "Invalid HTTP request");
        return;
    }
    const char* command = data;
    char* uri = strndup(data + mark_command + 1, mark_uri-mark_command-1);
    const char* body = mark_body ? data + mark_body : NULL;

    size_t len_command = mark_command;
    size_t len_uri = mark_uri - mark_command;

    if (len_command == 3 && strncmp(command, "GET", 3)==0)
        W_EMIT(self,on_get, uri, response, response_size);
    else if (len_command == 4 && strncmp(command, "POST", 4)==0)
        W_EMIT(self,on_post, uri, body, response, response_size);
    else if (len_command == 3 && strncmp(command, "PUT", 3)==0)
        W_EMIT(self,on_put, uri, body, response, response_size);
    else if (len_command == 5 && strncmp(command, "PATCH", 5)==0)
        W_EMIT(self,on_patch, uri, body, response, response_size);
    else if (len_command == 6 && strncmp(command, "DELETE", 6)==0)
        W_EMIT(self,on_delete, uri, response, response_size);
    else if (len_command == 4 && strncmp(command, "QUIT", 4)==0)
        W_EMIT(self,on_quit, uri, response, response_size);
    else
        W_EMIT(self,on_error, "Invalid command");

    free(uri);
}

#endif

