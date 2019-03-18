#include "forward_declare.h"
#include "UVtcpServer.h"
#include "UVtcpServer_class.h"
#include <wondermacros/objects/x/class_start.h>

#define M__listen METHOD(UVtcpServer,public,int,listen,(int backlog))
#define M__bind METHOD(UVtcpServer,public,int,bind,(int flags))
#define EXPAND_C
#include "UVtcpServer_impl.c"

#include <wondermacros/objects/x/class_end.h>
