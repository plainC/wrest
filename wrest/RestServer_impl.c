#ifdef EXPAND_C


#include <wondermacros/pp_char/charseq.h>
#include <wondermacros/string/for_string_as_uint.h>


static void
data_cb(struct RestServer* self, void* context, char* data, size_t length,
        char** response, size_t* response_size);

CONSTRUCT(RestServer)
{
    W_CALL_CONSTRUCT(UVtcpServer);

    W_OBJECT_SIGNAL_TYPE* handle;
    W_CONNECT(self,on_data, data_cb, handle);
}

static inline char*
parse_method(char* data, size_t* size, struct wrest_http_req* req)
{
#define PARSE_AND_SET(field,ch,...) \
    char* start = data;             \
    if (*size < 2)                  \
        return NULL;                \
    for (int i=0; i < *size; i++)   \
        if (data[i] == ch) {        \
            if (i==0)               \
                return NULL;        \
            data[i] = '\0';         \
            req->field = start;     \
            __VA_ARGS__             \
            *size -= i;             \
            return data+i;          \
        }                           \
                                    \
    return NULL                     \
/**/

    PARSE_AND_SET(method, ' ', ++i;);
}

static inline char*
parse_uri(char* data, size_t* size, struct wrest_http_req* req)
{
    PARSE_AND_SET(uri, ' ', ++i;);
}

static inline char*
parse_http_version(char* data, size_t* size, struct wrest_http_req* req)
{
    PARSE_AND_SET(http_version, '\r', if (data[i+1] != '\n') return NULL; else i+=2;);
}

static inline char*
parse_fields(char* data, size_t* size, struct wrest_http_req* req)
{
    struct hash_map {
        char* key;
        size_t value;
    };
    static struct hash_map* hash = NULL;
    if (! hash) {
#define XMACRO(name,field,...) \
    W_HASH_TABLE_PUSH(struct hash_map, hash, # name, offsetof(struct wrest_http_req, field));
#include "http_request_fields.h"
#undef XMACRO
    }

#define NEXT(ptr)     \
    if ((*size -= 1)) \
        ++ptr;        \
    else              \
        return NULL

    char* colon = memchr(data, ':', *size);
    while (colon) {
        *colon = '\0';

        W_HASH_TABLE_FOR_EACH_MATCH(struct hash_map, match, hash, data) {
            char* start = colon+1;
            *size -= start-data;

            while (*start == ' ')
                NEXT(start);
            char* end = start;
            while (*end != '\r')
                NEXT(end);
            *end = '\0';
            NEXT(end);
            if (*end != '\n')
                return NULL;
            NEXT(end);
            *((char**) W_REF_VOID_PTR(req, match->value)) = start;
            data = end;
        }
        colon = strchr(data, ':');
    }

#undef NEXT

    return data;
}

static inline char*
parse_body(char* data, size_t* size, struct wrest_http_req* req)
{
    if (*size < 3)
        return data;

    if (*size >= 2 && strncmp(data, "\r\n", 2)==0) {
        req->body = strndup(data+2, *size - 2);
        return data;
    } else
        return NULL;
}

static int
parse_http(char* data, size_t* size, struct wrest_http_req* req)
{
#define PARSE(name)                         \
    data = parse_## name(data, size, req);  \
    if (!data)                              \
        return 1

    PARSE(method);
    PARSE(uri);
    PARSE(http_version);
    PARSE(fields);
    PARSE(body);

#undef PARSE

    return 0;
}


static void
data_cb(struct RestServer* self, void* context, char* data, size_t length,
        char** response, size_t* response_size)
{
    int mark_command;
    int mark_uri;
    int mark_body;

    struct wrest_http_req req;
    bzero(&req, sizeof(struct wrest_http_req));

    struct wrest_http_resp resp;
    bzero(&resp, sizeof(struct wrest_http_resp));

    if (parse_http(data, &length, &req)) {
        W_EMIT(self,on_error, "Invalid HTTP request");
        return;
    }

    if (strlen(req.method) > 7) {
        W_EMIT(self,on_error, "Invalid HTTP method");
        return;
    }

    W_FOR_STRING_AS_UINT(uint64_t, method, req.method) {
        switch (method) {

#define COMMAND(lc,...)                                                            \
        case W_PP_CHARSEQ_TO_UINT(uint64_t,BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)): \
            W_EMIT(self,on_##lc, &req, &resp);                                     \
            break

            COMMAND(get,G,E,T);
            COMMAND(post,P,O,S,T);
            COMMAND(put,P,U,T);
            COMMAND(patch,P,A,T,C,H);
            COMMAND(delete,D,E,L,E,T,E);
            COMMAND(head,H,E,A,D);
            COMMAND(connect,C,O,N,N,E,C,T);
            COMMAND(options,O,P,T,I,O,N,S);
            COMMAND(trace,T,R,A,C,E);
            COMMAND(quit,Q,U,I,T);

#undef COMMAND

        default:
            W_EMIT(self,on_error, "Invalid HTTP method");
            return;
        }
    }

    int body_length = resp.body ? strlen(resp.body) : 0;
    char buffer[4096 + body_length];
    char* p = buffer;

    if (!resp.content_length) {
        char l[64];
        sprintf(l, "%d", body_length);
        resp.content_length = strdup(l);
    }

#define WRITE(...) p += sprintf(p, __VA_ARGS__)

    /**/

    if (resp.status_code) {
        const char* phrase = "Unknown";
        const char* version = "HTTP/1.1";

        if (resp.status_code < 1024 && http_status_phrase[resp.status_code])
            phrase = http_status_phrase[resp.status_code];

        WRITE("%s %d %s\r\n", version, resp.status_code, phrase);
    }

    /**/

#define XMACRO(name,field,...)                  \
    if (resp.field) {                           \
        WRITE("%s: %s\r\n", #name, resp.field); \
        free((void*) resp.field);               \
    }
#include <wrest/http_response_fields.h>
#undef XMACRO

    /**/

    if (resp.body) {
        WRITE("\r\n%s", resp.body);
        free((void*) resp.body);
    }
    *response = strdup(buffer);
    *response_size = p - buffer;
}

#endif

