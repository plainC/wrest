#include "forward_declare.h"
#include "ActivityRegularCb.h"
#include "ActivityRegularCb_class.h"
#include <wondermacros/objects/x/class_start.h>

CONSTRUCT(ActivityRegularCb)
{
    W_CALL_ACONSTRUCT(ActivityRegular);
}

/* Override: run */
METHOD(ActivityRegularCb,public,void,run)
{
    self->cb(NULL);
}

/* Override: is_active */
METHOD(ActivityRegularCb,public,int,is_active)
{
    return 1;
}

/* Override: is_finished */
METHOD(ActivityRegularCb,public,int,is_finished)
{
    return 0;
}

#include <wondermacros/objects/x/class_end.h>
