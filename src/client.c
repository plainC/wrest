#include "wrest.h"

void
error_cb(struct UVtcpServer* self, void* context, const char* message)
{
    printf("ERROR: %s\n", message);
}

char*
read_file(const char* filename)
{
    static char buffer[4096] = "\r\n\r\n";
;

    FILE* in = fopen(filename, "r");
    if (!in)
        return "\r\n";
    fread(buffer+4, 4096-4, 1, in);
    fclose(in);

    return buffer;
}

int main(int argc, char** argv)
{
    struct UVloop* loop = W_NEW(UVloop);
    struct UVtcpClient* client = W_NEW(UVtcpClient,
        .loop = loop,
        .address="0.0.0.0",
        .port=9000);

    char* command = "GET";
    char* uri = "/";
    char* body = "\r\n";

    if (argc > 1)
        command = argv[1];
    if (argc > 2)
        uri = argv[2];
    if (argc > 3)
        body = read_file(argv[3]);

    W_OBJECT_SIGNAL_TYPE* handle;

    W_CONNECT(client,on_error, error_cb, handle);

    char buffer[strlen(command)+strlen(uri)+32];
    sprintf(buffer, "%s %s HTTP/1.1%s", command, uri, body);
    W_CALL(client,connect)(buffer,strlen(buffer));
    W_CALL(loop,run)(UV_RUN_DEFAULT);

    W_CALL_VOID(client,free);
    W_CALL_VOID(loop,free);

    return 0;
}
