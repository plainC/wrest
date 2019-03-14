CFLAGS =            \
	-I .        \
	-I ../../wondermacros/wondermacros \
        -g          \

CLASSES = $(wildcard *_class.h)
SRCS    = $(patsubst %_class.h,%.c,$(CLASSES))
C_SRCS  = $(patsubst %.c,%_impl.c,$(SRCS))
HEADERS = $(patsubst %.c,%.h,$(SRCS))
OBJS    = $(patsubst %.c,%.o,$(SRCS))

all: build_src server client librest.so

build_src: classes.wobject
	../../wondermacros/tools/wobject -c classes.wobject

librest.so: rest.c
	gcc -c -fPIC rest.c
	gcc -o librest.so -shared -FPIC rest.o

server: $(OBJS) server.o UVloop_class.h
	cc -o server server.o $(OBJS) -luv -ldl

client: $(OBJS) client.o UVloop_class.h
	cc -o client client.o $(OBJS) -luv

$(OBJS): $(SRCS) $(HEADERS) $(CLASSES) $(C_SRCS) server.c
	cc -c $(CFLAGS) server.c client.c $^

clean:
	rm -f server server.o client client.o $(OBJS) *.gch *.o librest.so
