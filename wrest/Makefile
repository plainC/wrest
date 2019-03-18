CFLAGS =                                   \
	-I ..                              \
	-I ../../wondermacros/wondermacros \
        -g                                 \

CLASSES = $(wildcard *_class.h)
SRCS    = $(patsubst %_class.h,%.c,$(CLASSES))
C_SRCS  = $(patsubst %.c,%_impl.c,$(SRCS))
HEADERS = $(patsubst %.c,%.h,$(SRCS))
OBJS    = $(patsubst %.c,%.o,$(SRCS))

all: build_src wrest client librest.so

build_src: classes.wobject
	../../wondermacros/tools/wobject -c classes.wobject

librest.so: rest.c
	gcc $(CFLAGS) -c -fPIC rest.c
	gcc -o librest.so -shared -FPIC rest.o

wrest: $(OBJS) server.o UVloop_class.h
	cc -o wrest server.o $(OBJS) -luv -ldl

client: $(OBJS) client.o UVloop_class.h
	cc -o client client.o UVloop.o UVtcp.o UVtcpClient.o -luv

$(OBJS): $(SRCS) $(HEADERS) $(CLASSES) $(C_SRCS) server.c
	cc -c $(CFLAGS) server.c client.c $^

install: wrest
	mkdir -p /usr/include/wrest
	cp wrest.h                     \
		classes.h              \
		forward_declare.h      \
		http_request_fields.h  \
		http_response_fields.h \
		http_status_codes.h    \
		$(HEADERS)             \
		$(CLASSES) /usr/include/wrest
	cp wrest /usr/bin

clean:
	rm -f wrest server.o client client.o $(OBJS) *.gch *.o librest.so
