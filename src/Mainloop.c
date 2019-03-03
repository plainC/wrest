#include "forward_declare.h"
#include "Mainloop.h"
#include "Mainloop_class.h"
#include <wondermacros/objects/x/class_start.h>

CONSTRUCT(Mainloop)
{
}

FINALIZE(Mainloop)
{
}

METHOD(Mainloop,public,void,add,(struct ActivityRegular* a))
{
    W_DYNAMIC_ARRAY_PUSH(self->activities, a);
    a->due = a->sec;
    printf("Added activity: %s\n", a->name);
}

METHOD(Mainloop,public,void,run)
{
    printf("Mainloop: run\n");
}

#include <wondermacros/objects/x/class_end.h>