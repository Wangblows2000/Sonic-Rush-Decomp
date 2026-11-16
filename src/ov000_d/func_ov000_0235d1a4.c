#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    s32 unk8;
} UnkStruct_0235d1a4_Res;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    u8 pad14[0xC1C - 0x14];
    u8 tiles[2][7][0x20];
    void *unkDDC[2][7];
    u8 padE14[0xE30 - 0xE14];
    UnkStruct_0235d1a4_Res *unkE30;
} UnkStruct_0235d1a4;

typedef struct {
    u8 pad0[0x18];
    u16 unk18;
} UnkStruct_02072cb8;

extern void *data_0208fe78;
extern UnkStruct_02072cb8 data_02072cb8;
extern u16 data_02072cbc[];
extern u32 data_02250438;
extern u32 data_02250434;
extern void *data_0208f7e0;
extern void *data_0208f7dc;

void func_ov000_0235d068(u32 *out, u16 *src, s32 n);
s32 func_02051300(u16 *buf, s32 n);
void *func_02034a40(void *heap, u32 size, s32 align);
void *func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void MIi_CpuCopy32(const void *src, void *dst, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_020349b8(void *heap, void *p);
u32 func_ov000_0235d43c(void *res);
u32 func_ov000_0235d430(void *res);
void func_02008648(void *src, void *dst);
void func_02008720(void *src, void *dst);
void func_020087f8(void *src, void *dst);
void *func_02033a54(void *res);
void func_02033a68(void *src, void *dst, u32 size);
void *func_02039c18(s32 a, s32 b);
void func_02035704(void *src, s32 a, s32 b, u32 dst);
void func_02035754(void);
void func_020337ec(void *src, s32 size, s32 a, void *dst);
void func_0203383c(void);

static inline void *UnkAlloc_0235d1a4(u32 size, s32 align) {
    u8 *p;
    size = (size + 0x43) & ~0x1f;
    p = func_02034a40(data_0208fe78, size, align);
    if (func_02006abc(0) != p) {
        vu32 fill = 0xcccccccc;
        MIi_CpuClear32(fill, p, size);
        *(u32 *)p = size;
        *(u32 *)(p + size - 4) = 0x2467531;
        DC_StoreRange(p, size);
        p += 0x20;
    }
    return p;
}

static inline void UnkFree_0235d1a4(void *p) {
    u32 size;
    if (func_02006abc(0) != p) {
        p = (u8 *)p - 0x20;
        size = *(u32 *)p;
        if (*(u32 *)((u8 *)p + size - 4) == 0x2467531) {
            vu32 fill = 0xcccccccc;
            MIi_CpuClear32(fill, p, size);
            DC_StoreRange(p, size);
        }
    }
    func_020349b8(data_0208fe78, p);
}

static inline void UnkLoad_0235d1a4(void *src, void *dst) {
    void *pa = data_0208f7e0;
    void *pb = data_0208f7dc;
    data_0208f7e0 = NULL;
    data_0208f7dc = NULL;
    func_020337ec(src, 0x20, 0, dst);
    func_0203383c();
    data_0208f7e0 = pa;
    data_0208f7dc = pb;
}

#pragma thumb on

void func_ov000_0235d1a4(UnkStruct_0235d1a4 *w) {
    void *dst;
    s32 j;
    s32 i;
    u32 sa;
    u32 sb;
    vu16 zero;
    u16 name[8];
    u32 idx[2][7];
    void *src;

    if (w->unk10) {
        func_ov000_0235d068(idx[0], data_02072cbc, data_02072cb8.unk18);
        func_ov000_0235d068(idx[1], name, (u16)func_02051300(name, 7));
    } else {
        func_ov000_0235d068(idx[1], data_02072cbc, data_02072cb8.unk18);
        func_ov000_0235d068(idx[0], name, (u16)func_02051300(name, 7));
    }
    src = (u8 *)w->unkE30 + w->unkE30->unk8;
    {
        u32 size = ((*(u32 *)src >> 8) + 0x43) & ~0x1f;
        dst = func_02034a40(data_0208fe78, size, -0x20);
        if (func_02006abc(0) != dst) {
            vu32 fill = 0xcccccccc;
            MIi_CpuClear32(fill, dst, size);
            *(u32 *)dst = size;
            *(u32 *)((u8 *)dst + size - 4) = 0x2467531;
            DC_StoreRange(dst, size);
            dst = (u8 *)dst + 0x20;
        }
    }
    switch (func_ov000_0235d43c(src)) {
    case 0x10:
        func_02008648(src, dst);
        break;
    case 0x20:
        func_02008720(src, dst);
        break;
    case 0x30:
        func_020087f8(src, dst);
        break;
    default:
        func_02033a68(func_02033a54(src), dst, func_ov000_0235d430(src));
        break;
    }
    DC_StoreRange(dst, func_ov000_0235d430(src));
    for (j = 0; j < 2; j++) {
        for (i = 0; i < 7; i++) {
            MIi_CpuCopy32((u8 *)dst + (idx[j][i] << 5), w->tiles[j][i], 0x20);
        }
    }
    for (j = 0; j < 2; j++) {
        for (i = 0; i < 7; i++) {
            w->unkDDC[j][i] = func_02039c18(0, 1);
        }
    }
    zero = 0;
    sa = data_02250438;
    sb = data_02250434;
    data_02250438 = 0;
    data_02250434 = 0;
    func_02035704((void *)&zero, 1, 0, 0x500039e);
    func_02035754();
    data_02250438 = sa;
    data_02250434 = sb;
    for (j = 0; j < 2; j++) {
        for (i = 0; i < 7; i++) {
            UnkLoad_0235d1a4(w->tiles[j][i], w->unkDDC[j][i]);
        }
    }
    UnkFree_0235d1a4(dst);
}

#pragma thumb reset
