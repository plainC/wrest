#include "forward_declare.h"
#include "UVtcpClient.h"
#include "UVtcpClient_class.h"
#include <wondermacros/objects/x/class_start.h>

#define M__connect METHOD(UVtcpClient,public,int,connect,(const char* message, size_t len))
#define EXPAND_C
#include "UVtcpClient_impl.c"

#include <wondermacros/objects/x/class_end.h>
