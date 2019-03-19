#define CLASS UVtcp
#define ABSTRACT
#define NO_DESTRUCT

#define UVtcp__define \
    VAR(read,struct sockaddr_in,addr) \
    VAR(read,struct UVloop*,loop) \
    VAR(read,uv_tcp_t,handle) \
    VAR(read,int,port) \
    VAR(read,char*,address) \
    SIGNAL(on_error,(const char* message)) \
    METHOD(UVtcp,public,int,parse_address) \
    /**/
