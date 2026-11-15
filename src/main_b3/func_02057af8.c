#include <nitro/types.h>

typedef struct { u8 pad0[8]; s32 chr; s32 scr; s32 pal; u32 flags; } UnkRes_02057af8;
typedef struct UnkNode_02057af8 UnkNode_02057af8;
extern UnkNode_02057af8 *data_02250438;
extern UnkNode_02057af8 *data_02250434;
extern UnkNode_02057af8 *data_02250440;
extern UnkNode_02057af8 *data_0225043c;
extern u8 data_0207c5f0[];
extern void *data_0208fe80;

UnkRes_02057af8 *func_0203879c(void *a, s32 b, s32 c);
s32 func_02057c5c(void *p);
u32 func_02057c50(void *p);
void func_02008648(void *src, void *dst);
void func_02008720(void *src, void *dst);
void func_020087f8(void *src, void *dst);
void *func_02033a54(void *p);
void func_02033a68(void *src, void *dst, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_02035704(void *p, s32 a, s32 b, u32 c);
void func_02035754(void);
void func_02035d04(void *p, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i, s32 j, s32 k);
void func_020363f0(void);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void func_020349b8(void *heap, void *p);

#pragma thumb on
static inline void LoadScr_02057af8(void *scr, UnkRes_02057af8 *res) {
    s32 flag = (res->flags & 0x1000) ? 1 : 0;
    UnkNode_02057af8 *s0 = data_02250440;
    UnkNode_02057af8 *s1 = data_0225043c;
    data_02250440 = NULL;
    data_0225043c = NULL;
    func_02035d04(scr, 0, 0, 0x20, flag, 0xc, 0, 4, 0, 0, 0x20, 0x18);
    func_020363f0();
    data_02250440 = s0;
    data_0225043c = s1;
}

static inline void LoadPal_02057af8(void *pal) {
    UnkNode_02057af8 *s0 = data_02250438;
    UnkNode_02057af8 *s1 = data_02250434;
    data_02250438 = NULL;
    data_02250434 = NULL;
    func_02035704(pal, 0x100, 0, 0x05000400);
    func_02035754();
    data_02250438 = s0;
    data_02250434 = s1;
}

static inline void Free_02057af8(u8 *blk) {
    if ((u8 *)func_02006abc(0) != blk) {
        u32 size;
        blk -= 0x20;
        size = *(u32 *)blk;
        if (*(u32 *)(blk + size - 4) == 0x02467531) {
            vu32 fill = 0xcccccccc;
            MIi_CpuClear32(fill, blk, size);
            DC_StoreRange(blk, size);
        }
    }
    func_020349b8(data_0208fe80, blk);
}

void func_02057af8(void) {
    UnkRes_02057af8 *res = func_0203879c(data_0207c5f0, 1, -1);
    u8 *p = (u8 *)res + res->chr;

    switch (func_02057c5c(p)) {
    case 0x10:
        func_02008648(p, (void *)0x06200000);
        break;
    case 0x20:
        func_02008720(p, (void *)0x06200000);
        break;
    case 0x30:
        func_020087f8(p, (void *)0x06200000);
        break;
    default:
        func_02033a68(func_02033a54(p), (void *)0x06200000, func_02057c50(p));
        break;
    }
    DC_StoreRange((void *)0x06200000, func_02057c50(p));
    LoadScr_02057af8((u8 *)res + res->scr, res);
    LoadPal_02057af8((u8 *)res + res->pal + 4);
    Free_02057af8((u8 *)res);
}
#pragma thumb reset
