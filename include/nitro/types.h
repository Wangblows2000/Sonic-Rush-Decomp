#ifndef NITRO_TYPES_H
#define NITRO_TYPES_H

/* Basic NitroSDK-compatible types. Names follow the public NitroSDK
 * conventions so decompiled SDK code reads like the original API. */

typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned long       u32;
typedef unsigned long long  u64;
typedef signed char         s8;
typedef signed short        s16;
typedef signed long         s32;
typedef signed long long    s64;

typedef volatile u8         vu8;
typedef volatile u16        vu16;
typedef volatile u32        vu32;
typedef volatile s8         vs8;
typedef volatile s16        vs16;
typedef volatile s32        vs32;

typedef s32                 fx32;   /* 20.12 fixed point */
typedef s16                 fx16;   /* 4.12 fixed point  */

typedef int                 BOOL;
#define TRUE  1
#define FALSE 0

#ifndef NULL
#define NULL ((void *)0)
#endif

#endif /* NITRO_TYPES_H */
