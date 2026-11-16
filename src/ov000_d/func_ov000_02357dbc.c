#include <nitro/types.h>

typedef struct {
    u8 pad0[0x30];
    void *res;
} UnkStruct_02357dbc_Src;

typedef struct {
    s32 unk0;
    UnkStruct_02357dbc_Src *a;
    UnkStruct_02357dbc_Src *b;
    u8 bufA[0x200];
    u8 bufB[0x200];
} UnkStruct_02357dbc;

typedef struct {
    u8 pad0[0x10];
    void *work;
} UnkStruct_02357dbc_Task;

UnkStruct_02357dbc_Task *func_020372d0(void (*f)(void), void (*g)(void), s32 b, s32 c, s32 d, s32 e);
void func_ov000_02357d5c(void);
u32 func_ov000_02357804(void *res);
u32 func_ov000_023577f8(void *res);
void func_02008648(void *src, void *dst);
void func_02008720(void *src, void *dst);
void func_020087f8(void *src, void *dst);
void *func_02033a54(void *res);
void func_02033a68(void *src, void *dst, u32 size);
void DC_StoreRange(void *p, u32 size);

static inline void UnkDecomp_02357dbc(void *res, void *dst) {
    switch (func_ov000_02357804(res)) {
    case 0x10:
        func_02008648(res, dst);
        break;
    case 0x20:
        func_02008720(res, dst);
        break;
    case 0x30:
        func_020087f8(res, dst);
        break;
    default:
        func_02033a68(func_02033a54(res), dst, func_ov000_023577f8(res));
        break;
    }
    DC_StoreRange(dst, func_ov000_023577f8(res));
}

#pragma thumb on

void func_ov000_02357dbc(UnkStruct_02357dbc_Src *a, UnkStruct_02357dbc_Src *b) {
    UnkStruct_02357dbc *w = func_020372d0(func_ov000_02357d5c, NULL, 0, 0x2000, 0x1000, sizeof(UnkStruct_02357dbc))->work;

    w->unk0 = 0;
    w->a = a;
    w->b = b;
    UnkDecomp_02357dbc(a->res, w->bufA);
    UnkDecomp_02357dbc(b->res, w->bufB);
}

#pragma thumb reset
