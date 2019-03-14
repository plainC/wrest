CFLAGS =            \
	-I .        \
	-I ../../wondermacros/wondermacros \
        -g          \

CLASSES = $(wildcard *_class.h)
SRCS    = $(patsubst %_class.h,%.c,$(CLASSES))
C_SRCS  = $(patsubst %.c,%_impl.c,$(SRCS))
HEADERS = $(patsubst %.c,%.h,$(SRCS))
OBJS    = $(patsubst %.c,%.o,$(SRCS))

all: build_src server client

build_src: classes.wobject
	../../wondermacros/tools/wobject -c classes.wobject

server: $(OBJS) server.o UVloop_class.h
	cc -o server server.o $(OBJS) -luv

client: $(OBJS) client.o UVloop_class.h
	cc -o client client.o $(OBJS) -luv

$(OBJS): $(SRCS) $(HEADERS) $(CLASSES) $(C_SRCS) server.c
	cc -c $(CFLAGS) server.c client.c $^

clean:
	rm -f server server.o client client.o $(OBJS) *.gch $(CLASSES) $(HEADERS) $(SRCS) classes.h forward_declare.h *.o
