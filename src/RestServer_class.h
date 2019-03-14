#define CLASS RestServer
#define SUPER UVtcpServer
#define NO_DESTRUCT

#define RestServer__define \
    /* Inherits: UVtcpServer */ \
    UVtcpServer__define \
    \
    SIGNAL(on_post,(char* uri, char* body, char** response, size_t* response_length)) \
    SIGNAL(on_quit,(char* uri, char** response, size_t* response_length)) \
    SIGNAL(on_put,(char* uri, char* body, char** response, size_t* response_length)) \
    SIGNAL(on_get,(char* uri, char** response, size_t* response_length)) \
    SIGNAL(on_delete,(char* uri, char** response, size_t* response_length)) \
    SIGNAL(on_patch,(char* uri, char* body, char** response, size_t* response_length)) \
    /**/
