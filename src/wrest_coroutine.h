/*
 * (C) Copyright 2019 by J.P. Iivonen
 * original coroutine.h is copyright 1995,2000 Simon Tatham.
 * https://www.chiark.greenend.org.uk/~sgtatham/coroutines.html
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL SIMON TATHAM BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#ifndef __WREST_COROUTINE_H
#define __WREST_COROUTINE_H

#ifndef W_CCR_T
# define W_CCR_T int
#endif

#define CTX                              \
    ((struct _ccrContext*) (self->context))

#define W_CCR_CONTEXT_BEGIN              \
    struct _ccrContext {                 \
        W_CCR_T _ret;                    \
        int _line

#define W_CCR_CONTEXT_END                \
    }

#define W_CCR_BEGIN                      \
    if (!self->context) {                \
        self->context = malloc(sizeof(struct _ccrContext));\
        CTX->_line = 0;                  \
    }                                    \
    if (self->context)                   \
        switch(CTX->_line) {             \
        case 0:;

#define W_CCR_END                        \
    }                                    \
    free(self->context);                 \
    self->context = NULL;                \
    return WrestEvalStatusOk

#define W_CCR_YIELD(z)                   \
        do {                             \
            CTX->_line=__LINE__;         \
            CTX->_ret=(z);               \
            return WrestEvalStatusYield; \
            case __LINE__:;              \
        } while (0)

#define W_CCR_RETURN(z)                  \
    do {                                 \
        W_CCR_YIELD(z);                  \
        free(self->context);             \
        self->context=NULL;              \
        return WrestEvalStatusOk;        \
    } while (0)

#endif
