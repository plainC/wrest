# Wrest

Wrest implements [REST (Representational state transfer)](https://en.wikipedia.org/wiki/Representational_state_transfer)
protocol using [libuv](https://en.wikipedia.org/wiki/Libuv) in plain C. We
use [object-oriented programming](https://en.wikipedia.org/wiki/Object-oriented_programming)
to wrap libuv and the actions. The object system is provided by [Wondermacros](https://github.com/plainc/wondermacros).

Building a REST service with Wrest could not be easier in C. Just write a
dynamically loaded [library of functions](https://github.com/plainC/wrest/blob/master/wrest/rest.c)
that implement the REST methods.
When the server is started, use `-m PATH` option to load a specific REST
service. Each REST method is implemented as a separate callback which gets
a parsed HTTP request as an argument.

## To Build

* Install Boost library (Wondermacros uses Boost pre-processor library)
* Install [Wondermacros](https://github.com/plainc/wondermacros)
* `cd src && make`

To build your own REST service library type

```
gcc -c -fPIC mylib.c
gcc -o libmylib.so -shared -FPIC mylib.o
```

## How to run

To start the server, type e.g. `./wrest -m ./librest.so`. It will start the
server in address `0.0.0.0` on port 9000. To change the IP port, use
`-p INT` option. To change the IP address, type the address as last argument.

To run the client type

```
./client GET /
./client POST /inbox body.ex
```

## Limitations

This is just a demo at a moment.
* The server currently blocks for each request.
* The messages are read using single read.
* Client reads only small files for a message body (4092 bytes).
* No SSL etc.
