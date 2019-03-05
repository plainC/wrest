#ifndef __WREST_COROUTINE_H
#define __WREST_COROUTINE_H

#ifndef W_CCR_T
# define W_CCR_T int
#endif

#define W_CCR_CONTEXT_BEGIN struct _ccrContext { W_CCR_T _ret; int _line
#define W_CCR_CONTEXT_END   }
#define W_CCR_BEGIN      if (!self->context) {self->context = malloc(sizeof(struct _ccrContext));} \
                         if (self->context) switch(CTX->_line) { case 0:;
#define W_CCR_END        } free(self->context); self->context = NULL; return WrestEvalStatusOk

#define W_CCR_YIELD(z)   \
        do {\
            CTX->_line=__LINE__;\
            CTX->_ret=(z);\
            return WrestEvalStatusYield; case __LINE__:;\
        } while (0)

#define W_CCR_RETURN(z)       do { W_CCR_YIELD(z); free(self->context); self->context=NULL; return WrestEvalStatusOk; } while (0)
#define CTX ((struct _ccrContext*) (self->context))

#endif

