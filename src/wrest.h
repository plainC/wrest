#ifndef __WREST_H
#define __WREST_H

#define W_OBJECT_CAST_TO_VOID

#ifndef WDEBUG_EXPAND
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <uv.h>
# include <wondermacros/objects/meta.h>
# include <wondermacros/array/dynamic_array.h>
#endif

#include <wondermacros/list/cslist.h>
#include <wondermacros/objects/api.h>

struct str_rope { struct str_rope* next; const char* str; size_t size; };
typedef struct str_rope str_rope_t;

#include "classes.h"

#endif

