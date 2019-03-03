#include "forward_declare.h"
#include "ActivityRegular.h"
#include "ActivityRegular_class.h"
#include <wondermacros/objects/x/class_start.h>

CONSTRUCT(ActivityRegular)
{
    if (!self->sec && !self->usec)
        self->sec = 3;
}

#include <wondermacros/objects/x/class_end.h>
