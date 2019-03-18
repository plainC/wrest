#include "wrest.h"

void
rest_get(struct UVtcpServer* self, void* context, struct wrest_http_req* req,
      struct wrest_http_resp* resp)
{
    printf("MODULE GET: '%s'\n\n", req->uri);

    resp->body = strdup("GET ok");
    resp->status_code = 200;
}

void
rest_head(struct UVtcpServer* self, void* context, struct wrest_http_req* req,
      struct wrest_http_resp* resp)
{
    printf("MODULE HEAD: '%s'\n\n", req->uri);

    resp->body = strdup("HEAD ok");
    resp->status_code = 200;
}

void
rest_delete(struct UVtcpServer* self, void* context, struct wrest_http_req* req,
      struct wrest_http_resp* resp)
{
    printf("MODULE DELETE: '%s'\n\n", req->uri);

    resp->body = strdup("DELETE ok");
    resp->status_code = 200;
}

void
rest_trace(struct UVtcpServer* self, void* context, struct wrest_http_req* req,
      struct wrest_http_resp* resp)
{
    printf("MODULE TRACE: '%s'\n\n", req->uri);

    resp->body = strdup("TRACE ok");
    resp->status_code = 200;
}

void
rest_connect(struct UVtcpServer* self, void* context, struct wrest_http_req* req,
      struct wrest_http_resp* resp)
{
    printf("MODULE TRACE: '%s'\n\n", req->uri);

    resp->body = strdup("TRACE ok");
    resp->status_code = 200;
}

void
rest_quit(struct UVtcpServer* self, void* context, struct wrest_http_req* req,
      struct wrest_http_resp* resp)
{
    printf("MODULE QUIT: '%s'\n\n", req->uri);

    resp->body = strdup("QUIT ok");
    resp->status_code = 200;

    W_CALL_VOID(self->loop,stop);
}

void
rest_post(struct UVtcpServer* self, void* context, struct wrest_http_req* req,
      struct wrest_http_resp* resp)
{
    printf("MODULE POST: '%s'\n\n", req->uri);

    resp->body = strdup("POST ok");
    resp->status_code = 200;
}

void
rest_put(struct UVtcpServer* self, void* context, struct wrest_http_req* req,
      struct wrest_http_resp* resp)
{
    printf("MODULE PUT: '%s'\n\n", req->uri);

    resp->body = strdup("PUT ok");
    resp->status_code = 200;
}

void
rest_patch(struct UVtcpServer* self, void* context, struct wrest_http_req* req,
      struct wrest_http_resp* resp)
{
    printf("MODULE PATCH: '%s'\n\n", req->uri);

    resp->body = strdup("PATCH ok");
    resp->status_code = 200;
}

void
rest_options(struct UVtcpServer* self, void* context, struct wrest_http_req* req,
      struct wrest_http_resp* resp)
{
    printf("MODULE OPTIONS: '%s'\n\n", req->uri);

    resp->body = strdup("OPTIONS ok");
    resp->status_code = 200;
}

