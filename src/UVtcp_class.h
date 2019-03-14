#define CLASS UVtcp
#define ABSTRACT
#define NO_CONSTRUCT
#define NO_DESTRUCT

#define UVtcp__define \
    VAR(read,struct UVloop*,loop) \
    VAR(read,char*,address) \
    VAR(read,struct sockaddr_in,addr) \
    VAR(read,uv_tcp_t,handle) \
    VAR(read,int,port) \
    SIGNAL(on_error,(const char* message)) \
    /**/
