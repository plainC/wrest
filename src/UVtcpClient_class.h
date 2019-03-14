#define CLASS UVtcpClient
#define SUPER UVtcp
#define NO_DESTRUCT

#define UVtcpClient__define \
    /* Inherits: UVtcp */ \
    UVtcp__define \
    \
    VAR(private,size_t,len) \
    VAR(read,uv_connect_t*,conn) \
    VAR(private,const char*,message) \
    METHOD(UVtcpClient,public,int,connect,(const char* message, size_t len)) \
    /**/
