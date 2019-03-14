#ifdef EXPAND_C

#include <boost/preprocessor/control/expr_if.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>

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
    const char* body = mark_body ? strndup(data + mark_body, length-mark_body) : NULL;

    size_t len_command = mark_command;
    size_t len_uri = mark_uri - mark_command;

#define COMMAND(uc,lc,has_body) \
    if (len_command == strlen(#uc) && strncmp(command, #uc, strlen(#uc))==0) \
        W_EMIT(self,on_##lc, uri BOOST_PP_COMMA_IF(has_body) BOOST_PP_EXPR_IF(has_body,body), response, response_size);

    COMMAND(GET,get,0)
    else COMMAND(POST,post,1)
    else COMMAND(PUT,put,1)
    else COMMAND(PATCH,patch,1)
    else COMMAND(DELETE,delete,0)
    else COMMAND(HEAD,head,0)
    else COMMAND(CONNECT,connect,0)
    else COMMAND(OPTIONS,options,1)
    else COMMAND(TRACE,trace,0)
    else COMMAND(QUIT,quit,0)
    else
        W_EMIT(self,on_error, "Invalid command");

#undef COMMAND

    free(uri);
    free((void*) body);
}

#endif

