#define CLASS UVtcpServer
#define SUPER UVtcp
#define NO_DESTRUCT

#define UVtcpServer__define \
    /* Inherits: UVtcp */ \
    UVtcp__define \
    \
    SIGNAL(on_connection_open,(const char* message)) \
    SIGNAL(on_data,(char* data, size_t size, char** response, size_t* response_size)) \
    SIGNAL(on_connection_close,(const char* message)) \
    METHOD(UVtcpServer,public,int,listen,(int backlog)) \
    METHOD(UVtcpServer,public,int,bind,(int flags)) \
    /**/
