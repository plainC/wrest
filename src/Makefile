CFLAGS =            \
	-I .        \
	-I ../../.. \
        -g          \

CLASSES = $(wildcard *_class.h)
SRCS    = $(patsubst %_class.h,%.c,$(CLASSES))
C_SRCS  = $(patsubst %.c,%_impl.c,$(SRCS))
HEADERS = $(patsubst %.c,%.h,$(SRCS))
OBJS    = $(patsubst %.c,%.o,$(SRCS))

all: build_src

build_src: classes.wobject
	../../wondermacros/tools/wobject -c classes.wobject
	echo "\nTo build type 'make test'"

test: $(OBJS) test.o WrestMainloop_class.h
	cc -o test test.o $(OBJS)

$(OBJS): $(SRCS) $(HEADERS) $(CLASSES) $(C_SRCS) test.c
	cc -c $(CFLAGS) test.c $^

clean:
	rm -f test test.o $(OBJS) *.gch $(CLASSES) $(HEADERS) $(SRCS) classes.h forward_declare.h *.o
