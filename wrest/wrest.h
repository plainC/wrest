#ifndef __WREST_H
#define __WREST_H

#define W_OBJECT_CAST_TO_VOID

#define W_HASH(a) jenkins_one_at_a_time_hash(a,strlen(a))
#define W_EQUAL(a,b) (strcmp(a,b)==0)

#ifndef WDEBUG_EXPAND
# include <stdio.h>
# include <stdlib.h>
# include <stddef.h>
# include <stdint.h>
# include <string.h>
# include <unistd.h>
# include <uv.h>
# include <wondermacros/objects/meta.h>
# include <wondermacros/array/dynamic_array.h>
# include <wondermacros/pointer/ref_void_ptr.h>

# include <wondermacros/array/hash_table.h>
#endif

#include <wondermacros/list/cslist.h>
#include <wondermacros/objects/api.h>

/* Generate struct wrest_http_req. */
#define W_XFILE            <wrest/http_request_fields.h>
#define W_NAME             wrest_http_req
#define W_NAME_INDEX       1
#define W_TYPE             const char*
#define W_PREFIX_FIELDS       \
    const char* method;       \
    const char* uri;          \
    const char* http_version; \
    const char* body;         \
    /**/
#include <wondermacros/x/struct_fields.h>

#include "classes.h"

static inline uint32_t
jenkins_one_at_a_time_hash(char *key, size_t len)
{
    uint32_t hash, i;
    for(hash = i = 0; i < len; ++i)
    {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

#endif

