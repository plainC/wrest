#include "wrest.h"

void
rest_get(struct UVtcpServer* self, void* context, const char* uri,
      char** response, size_t* response_size)
{
    printf("MODULE GET: '%s'\n\n", uri);

    *response = strdup("GET ok");
    *response_size = strlen(*response);
}

void
rest_head(struct UVtcpServer* self, void* context, const char* uri,
      char** response, size_t* response_size)
{
    printf("MODULE HEAD: '%s'\n\n", uri);

    *response = strdup("HEAD ok");
    *response_size = strlen(*response);
}

void
rest_delete(struct UVtcpServer* self, void* context, const char* uri,
      char** response, size_t* response_size)
{
    printf("MODULE DELETE: '%s'\n\n", uri);

    *response = strdup("DELETE ok");
    *response_size = strlen(*response);
}

void
rest_trace(struct UVtcpServer* self, void* context, const char* uri,
      char** response, size_t* response_size)
{
    printf("MODULE TRACE: '%s'\n\n", uri);

    *response = strdup("TRACE ok");
    *response_size = strlen(*response);
}

void
rest_connect(struct UVtcpServer* self, void* context, const char* uri,
      char** response, size_t* response_size)
{
    printf("MODULE TRACE: '%s'\n\n", uri);

    *response = strdup("TRACE ok");
    *response_size = strlen(*response);
}

void
rest_quit(struct UVtcpServer* self, void* context, const char* uri,
      char** response, size_t* response_size)
{
    printf("MODULE QUIT: '%s'\n\n", uri);

    *response = strdup("QUIT ok");
    *response_size = strlen(*response);

    W_CALL_VOID(self->loop,stop);
}

void
rest_post(struct UVtcpServer* self, void* context, const char* uri, char* body,
      char** response, size_t* response_size)
{
    printf("MODULE POST: '%s'\n\n", uri);

    *response = strdup("POST ok");
    *response_size = strlen(*response);
}

void
rest_put(struct UVtcpServer* self, void* context, const char* uri, char* body,
      char** response, size_t* response_size)
{
    printf("MODULE PUT: '%s'\n\n", uri);

    *response = strdup("PUT ok");
    *response_size = strlen(*response);
}

void
rest_patch(struct UVtcpServer* self, void* context, const char* uri, char* body,
      char** response, size_t* response_size)
{
    printf("MODULE PATCH: '%s'\n\n", uri);

    *response = strdup("PATCH ok");
    *response_size = strlen(*response);
}

void
rest_options(struct UVtcpServer* self, void* context, const char* uri, char* body,
      char** response, size_t* response_size)
{
    printf("MODULE OPTIONS: '%s'\n\n", uri);

    *response = strdup("OPTIONS ok");
    *response_size = strlen(*response);
}
