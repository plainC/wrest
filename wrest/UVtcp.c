#include "forward_declare.h"
#include "UVtcp.h"
#include "UVtcp_class.h"
#include <wondermacros/objects/x/class_start.h>

#define M__parse_address METHOD(UVtcp,public,int,parse_address)
#define EXPAND_C
#include "UVtcp_impl.c"

#include <wondermacros/objects/x/class_end.h>
