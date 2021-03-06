#include "wrest.h"
#include <dlfcn.h>

/**/
const char* http_status_phrase[1024] = {
#define XMACRO(code,phrase) [code] = # phrase,
#include <wrest/http_status_codes.h>
#undef XMACRO
};
/**/

void
error_cb(struct UVtcpServer* self, void* context, const char* message)
{
    printf("ERROR: %s\n", message);
}

void
quit_cb(struct UVtcpServer* self, void* context, struct wrest_http_req* req,
        struct wrest_http_resp* resp)
{
    printf("QUIT: '%s'\n", req->uri);

    resp->body = strdup("QUIT ok");

    W_CALL_VOID(self->loop,stop);
}

void
get_cb(struct UVtcpServer* self, void* context, struct wrest_http_req* req,
       struct wrest_http_resp* resp)
{
    printf("GET: '%s'\n", req->uri);

    resp->body = strdup("<html><body>GET ok</body></html>");
    resp->status_code = 200;
}

void
post_cb(struct UVtcpServer* self, void* context, struct wrest_http_req* req,
        struct wrest_http_resp* resp)
{
    printf("POST: '%s'\nBody:\n%s\n", req->uri, req->body);

    resp->body = strdup("POST ok");
    resp->status_code = 200;
}

void
put_cb(struct UVtcpServer* self, void* context, struct wrest_http_req* req,
       struct wrest_http_resp* resp)
{
    printf("PUT: '%s'\nBody:\n%s\n", req->uri, req->body);

    resp->body = strdup("PUT ok");
    resp->status_code = 200;
}

static void
usage(char* name)
{
    fprintf(stderr, "%s [-m PATH][-p INT] IP_ADDRESS\n", name);
    exit(1);
}

static void* dl_handle;

static void
load_module(const char* path, struct RestServer* server, char* name)
{
    dl_handle = dlopen(path, RTLD_LAZY);

    if (!dl_handle) {
        fputs(dlerror(), stderr);
        usage(name);
    }

    W_OBJECT_SIGNAL_TYPE* h;
    char* error;
    void (*rest_get)(struct UVtcpServer* self, void* context, struct wrest_http_req* req,
        struct wrest_http_resp* resp);
    void (*rest_head)(struct UVtcpServer* self, void* context, struct wrest_http_req* req, 
        struct wrest_http_resp* resp);
    void (*rest_delete)(struct UVtcpServer* self, void* context, struct wrest_http_req* req,
        struct wrest_http_resp* resp);
    void (*rest_connect)(struct UVtcpServer* self, void* context, struct wrest_http_req* req,
        struct wrest_http_resp* resp);
    void (*rest_trace)(struct UVtcpServer* self, void* context, struct wrest_http_req* req,
        struct wrest_http_resp* resp);
    void (*rest_quit)(struct UVtcpServer* self, void* context, struct wrest_http_req* req,
        struct wrest_http_resp* resp);
    void (*rest_post)(struct UVtcpServer* self, void* context, struct wrest_http_req* req,
        struct wrest_http_resp* resp);
    void (*rest_put)(struct UVtcpServer* self, void* context, struct wrest_http_req* req,
        struct wrest_http_resp* resp);
    void (*rest_patch)(struct UVtcpServer* self, void* context, struct wrest_http_req* req,
        struct wrest_http_resp* resp);
    void (*rest_options)(struct UVtcpServer* self, void* context, struct wrest_http_req* req,
        struct wrest_http_resp* resp);

    dlerror();

#define BIND(m)                                \
  do {                                         \
    rest_##m = dlsym(dl_handle, "rest_" #m);   \
    if (rest_##m) {                            \
        W_CONNECT(server,on_##m, rest_##m, h); \
        printf("* %s\n", #m);                  \
    }                                          \
  } while (0)

    printf("Loaded module: %s with\n", path);
    BIND(get);
    BIND(head);
    BIND(post);
    BIND(put);
    BIND(patch);
    BIND(delete);
    BIND(connect);
    BIND(options);
    BIND(trace);
    BIND(quit);

#undef BIND
}

int
main(int argc, char** argv)
{
    char* module = NULL;
    char* address = "0.0.0.0";
    int port = 9000;

    /* Parse arguments. */
    for (int i=1; i < argc; i++) {
        if (strcmp(argv[i],"-p")==0) {
            if (i+1 == argc)
                usage(argv[0]);
            else
                port = atoi(argv[++i]);
        } else {
            if (strcmp(argv[i],"-m")==0)
                if (i+1 == argc)
                    usage(argv[0]);
                else
                    module = argv[++i];
            else
                if (strchr(argv[i],'.')==NULL && strchr(argv[i],':')==NULL)
                    usage(argv[0]);
                else
                    address = argv[i];
        }
    }

    struct UVloop* loop = W_NEW(UVloop);
    struct RestServer* server = W_NEW(RestServer,
        .loop = loop,
        .address=address,
        .port=port);

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
    if (!module) {
        W_CONNECT(server,on_get, get_cb, handle);
        W_CONNECT(server,on_post, post_cb, handle);
        W_CONNECT(server,on_put, put_cb, handle);
        W_CONNECT(server,on_quit, quit_cb, handle);
    } else
        load_module(module, server, argv[0]);

    W_CALL(loop,run)(UV_RUN_DEFAULT);

    W_CALL_VOID(server,free);
    W_CALL_VOID(loop,free);

    if (module)
        dlclose(dl_handle);

    return 0;
}
