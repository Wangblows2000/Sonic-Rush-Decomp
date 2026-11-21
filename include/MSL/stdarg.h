#ifndef MAIN_SDK2_STDARG_H
#define MAIN_SDK2_STDARG_H

typedef char *va_list;
#define __va_align4(n) ((((unsigned long)(n)) + 3U) & ~3U)
#define va_start(ap, parm) ((ap) = ((char *)((unsigned long)(&parm) & ~3U)) + __va_align4(sizeof(parm)))
#define va_arg(ap, type) (*(type *)(((ap) += __va_align4(sizeof(type))) - __va_align4(sizeof(type))))
#define va_end(ap) ((void)0)

#endif
