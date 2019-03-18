#define CLASS RestServer
#define SUPER UVtcpServer
#define NO_DESTRUCT

#define RestServer__define \
    /* Inherits: UVtcpServer */ \
    UVtcpServer__define \
    \
    SIGNAL(on_connect,(struct wrest_http_req* req, char** response, size_t* response_length)) \
    SIGNAL(on_quit,(struct wrest_http_req* req, char** response, size_t* response_length)) \
    SIGNAL(on_head,(struct wrest_http_req* req, char** response, size_t* response_length)) \
    SIGNAL(on_patch,(struct wrest_http_req* req, char** response, size_t* response_length)) \
    SIGNAL(on_get,(struct wrest_http_req* req, char** response, size_t* response_length)) \
    SIGNAL(on_delete,(struct wrest_http_req* req, char** response, size_t* response_length)) \
    SIGNAL(on_put,(struct wrest_http_req* req, char** response, size_t* response_length)) \
    SIGNAL(on_trace,(struct wrest_http_req* req, char** response, size_t* response_length)) \
    SIGNAL(on_post,(struct wrest_http_req* req, char** response, size_t* response_length)) \
    SIGNAL(on_options,(struct wrest_http_req* req, char* body, char** response, size_t* response_length)) \
    /**/
