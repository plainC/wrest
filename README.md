# Wrest

Wrest implements [REST (Representational state transfer)](https://en.wikipedia.org/wiki/Representational_state_transfer)
protocol using [libuv](https://en.wikipedia.org/wiki/Libuv) in plain C. We
use [object-oriented programming](https://en.wikipedia.org/wiki/Object-oriented_programming)
to wrap libuv and the actions. The object system is provided by [Wondermacros](https://github.com/plainc/wondermacros).

Building a REST service with Wrest could not be easier in C. Just bind a callback
function to each REST command.

## To Build

* Install Boost library (Wondermacros uses Boost pre-processor library)
* Install [Wondermacros](https://github.com/plainc/wondermacros)
* `cd src && make`

## How to run

To start the server, type `./server`. It will start the server in `127.0.0.1`
on port 8000 (edit `server.c` to change). To run the client type

```
./client GET /
./client POST /inbox body.ex
```

## Limitations

This is just a demo at a moment.
* The server currently blocks for each request.
* The messages are read using single read.
* Client reads only small files for a message body (4092 bytes).
* No HTTP header support.
* No SSL etc.

## To use

Wrest provides signals for each REST protocol command. Just bind a callback
function each one of them (`on_get`, `on_post`, `on_put`, `on_patch`, etc.).
In addition, bind also `on_error`. Binding is done, for instance

```
W_OBJECT_SIGNAL_TYPE* handle;

W_CONNECT(server,on_error, error_cb, handle);

W_CONNECT(server,on_get, get_cb, handle);
W_CONNECT(server,on_post, post_cb, handle);
```

Each REST command is then an independent function. To implement `GET` just
provide a callback function.
```
void
get_cb(struct UVtcpServer* self, void* context, const char* uri,
        char** response, size_t* response_size)
{
    printf("GET: '%s'\n", uri);

    *response = strdup("Ok");
    *response_size = 2;
}
```
`PUT`, `POST` and `PATCH` get also the message body as an argument. The
callback has, thus, an extra argument.

```
void
post_cb(struct UVtcpServer* self, void* context, const char* uri, char* body,
        char** response, size_t* response_size)
{
    printf("POST: '%s'\nBody:\n%s\n", uri, body);

    *response = strdup("Got it");
    *response_size = 6;
}
```
See a full [server source code](https://github.com/plainC/wrest/blob/master/src/server.c) for more details.
