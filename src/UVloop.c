#include "forward_declare.h"
#include "UVloop.h"
#include "UVloop_class.h"
#include <wondermacros/objects/x/class_start.h>

#define M__run METHOD(UVloop,public,int,run,(uv_run_mode mode))
#define M__stop METHOD(UVloop,public,void,stop)
#define M__now METHOD(UVloop,public,uint64_t,now)
#define EXPAND_C
#include "UVloop_impl.c"

#include <wondermacros/objects/x/class_end.h>
