#include "forward_declare.h"
#include "ActivityRegular.h"
#include "ActivityRegular_class.h"
#include <wondermacros/objects/x/class_start.h>

CONSTRUCT(ActivityRegular)
{
    if (!self->usec)
        self->usec = 3000000;
}

#include <wondermacros/objects/x/class_end.h>
