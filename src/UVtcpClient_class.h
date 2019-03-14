#define CLASS UVtcpClient
#define SUPER UVtcp

#define UVtcpClient__define \
    /* Inherits: UVtcp */ \
    UVtcp__define \
    \
    VAR(private,size_t,len) \
    VAR(private,const char*,message) \
    VAR(read,uv_connect_t*,conn) \
    METHOD(UVtcpClient,public,int,connect,(const char* message, size_t len)) \
    /**/
