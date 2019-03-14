#include "wrest.h"

void
error_cb(struct UVtcpServer* self, void* context, const char* message)
{
    printf("ERROR: %s\n", message);
}

void
quit_cb(struct UVtcpServer* self, void* context, const char* uri,
        char** response, size_t* response_size)
{
    printf("QUIT: '%s'\n", uri);

    *response = strdup("QUIT ok");
    *response_size = strlen(*response);

    W_CALL_VOID(self->loop,stop);
}

void
get_cb(struct UVtcpServer* self, void* context, const char* uri,
        char** response, size_t* response_size)
{
    printf("GET: '%s'\n", uri);

    *response = strdup("GET ok");
    *response_size = strlen(*response);
}

void
post_cb(struct UVtcpServer* self, void* context, const char* uri, char* body,
        char** response, size_t* response_size)
{
    printf("POST: '%s'\nBody:\n%s\n", uri, body);

    *response = strdup("POST ok");
    *response_size = strlen(*response);
}

void
put_cb(struct UVtcpServer* self, void* context, const char* uri, char* body,
        char** response, size_t* response_size)
{
    printf("PUT: '%s'\nBody:\n%s\n", uri, body);

    *response = strdup("PUT ok");
    *response_size = strlen(*response);
}

int main()
{
    struct UVloop* loop = W_NEW(UVloop);
    struct RestServer* server = W_NEW(RestServer,
        .loop = loop,
        .address="127.0.0.1",
        .port=8000);

    if (W_CALL(server,bind)(0)) {
        printf("Unable to bind: %s %d\n", server->address, server->port);
        return 1;
    }
    if (W_CALL(server,listen)(128)) {
        printf("Unable to listen\n");
        return 2;
    }

    W_OBJECT_SIGNAL_TYPE* handle;

    W_CONNECT(server,on_error, error_cb, handle);
    W_CONNECT(server,on_get, get_cb, handle);
    W_CONNECT(server,on_post, post_cb, handle);
    W_CONNECT(server,on_put, put_cb, handle);
    W_CONNECT(server,on_quit, quit_cb, handle);

    W_CALL(loop,run)(UV_RUN_DEFAULT);

    W_CALL_VOID(server,free);
    W_CALL_VOID(loop,free);

    return 0;
}
