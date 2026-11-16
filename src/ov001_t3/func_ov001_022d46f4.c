#include <nitro/types.h>

typedef struct {
    u8 pad0[0x30];
    s32 unk30;
} UnkRes_022d46f4;

typedef struct {
    s32 unk0;
    UnkRes_022d46f4 *res;
    s32 unk8;
    s32 start;
    s32 count;
    u16 src[16];
    u16 dst[16];
    u16 cur[16];
    u8 pad74[0x94 - 0x74];
} UnkWork_022d46f4;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022d46f4 *work;
} UnkTask_022d46f4;

extern s32 data_02250438;
extern s32 data_02250434;

UnkTask_022d46f4 *func_020372d0(void (*fn)(void), void (*dtor)(void), s32 b, s32 c, s32 d, s32 size);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void func_ov001_022d4538(void);
void func_020356bc(s32 a, s32 b, u16 *pal);
void func_02035754(void);

#pragma thumb on
static inline void LoadPal_022d46f4(s32 id, u16 *pal) {
    s32 s0 = data_02250438;
    s32 s1 = data_02250434;
    data_02250438 = 0;
    data_02250434 = 0;
    func_020356bc(id, 0, pal);
    func_02035754();
    data_02250438 = s0;
    data_02250434 = s1;
}

UnkTask_022d46f4 *func_ov001_022d46f4(UnkRes_022d46f4 *res, s32 a, s32 b, s32 count, s32 extra) {
    UnkTask_022d46f4 *t;
    s32 i;
    u16 r1;
    UnkWork_022d46f4 *w;
    vu16 zero;
    u16 pal[0x100];
    u16 r0;
    u16 g0;
    u16 g1;
    u16 b0;
    u16 b1;

    t = func_020372d0(func_ov001_022d4538, NULL, 0, 0x3000, 0, sizeof(UnkWork_022d46f4));
    w = t->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkWork_022d46f4));
    w->unk0 = 0;
    w->res = res;
    w->unk8 = extra;
    w->start = a;
    w->count = count;
    LoadPal_022d46f4(res->unk30, pal);
    for (i = 0; i < count; i++) {
        w->src[i] = pal[a + i];
        w->dst[i] = pal[b + i];
        r0 = w->src[i] & 0x1f;
        g0 = (w->src[i] >> 5) & 0x1f;
        b0 = (w->src[i] >> 10) & 0x1f;
        r1 = w->dst[i] & 0x1f;
        g1 = (w->dst[i] >> 5) & 0x1f;
        b1 = (w->dst[i] >> 10) & 0x1f;
        if (r0 > r1) r0 = r1;
        if (g0 > g1) g0 = g1;
        if (b0 > b1) b0 = b1;
        w->cur[i] = (b0 << 10) | (r0 | (g0 << 5));
    }
    return t;
}
#pragma thumb reset
