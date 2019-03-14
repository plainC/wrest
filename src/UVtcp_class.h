#define CLASS UVtcp
#define ABSTRACT
#define NO_CONSTRUCT
#define NO_DESTRUCT

#define UVtcp__define \
    VAR(read,char*,address) \
    VAR(read,uv_tcp_t,handle) \
    VAR(read,struct sockaddr_in,addr) \
    VAR(read,int,port) \
    VAR(read,struct UVloop*,loop) \
    SIGNAL(on_error,(const char* message)) \
    /**/
