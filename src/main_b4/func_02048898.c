#include <nitro/types.h>

typedef struct {
    u8 pad0[0x50];
    u32 flags;
    u8 pad54[0x28];
} UnkSpr_02048898;

typedef struct {
    u32 timer;
    u16 idx;
    u16 flags;
    u8 pad8[4];
    UnkSpr_02048898 spr0;
    UnkSpr_02048898 spr1;
} UnkWork_02048898;

typedef struct {
    u8 pad0[0x10];
    UnkWork_02048898 *work;
} UnkTask_02048898;

typedef struct {
    u8 pad0[0x1c];
    u8 unk1c;
    u8 unk1d;
    u8 pad1e[2];
    s32 unk20;
} UnkStruct_022c4560;

typedef struct {
    u8 pad0[0x10];
    u32 unk10;
} UnkStruct_022b65b4;

typedef struct {
    u16 limit;
    u8 pad2[0x20];
} UnkEntry_02073bd8;

extern UnkTask_02048898 *data_022b4574;
extern UnkStruct_022c4560 data_022c4560;
extern u16 data_022c4ee4;
extern UnkStruct_022b65b4 data_022b65b4;
extern UnkEntry_02073bd8 data_02073bd8[];

void func_02043c7c(s32 a, s32 b);
BOOL func_02043a58(void);
void func_02053bec(s32 a);
void func_0204d684(void (*f)(void), s32 a);
void func_02054e68(void);
void func_02054e4c(void);
BOOL func_ov000_0233b660(void);
void func_02032c8c(UnkSpr_02048898 *s);

void func_02048898(void) {
    UnkWork_02048898 *w = data_022b4574->work;

    if ((w->flags & 4) && data_022c4560.unk20 != 0) {
        func_02043c7c(3, 0x40);
        w->flags &= ~4;
    }
    if (w->flags & 1) {
        if (func_02043a58()) {
            data_022c4ee4 = 5;
            if (data_022c4560.unk1c >= 9 || data_022c4560.unk1d >= 2)
                func_02053bec(1);
            if (w->flags & 2)
                func_0204d684(func_02054e68, 3);
            else
                func_0204d684(func_02054e4c, 3);
            return;
        }
    } else {
        if (((u16)(((*(vu16 *)0x4000130 | *(vu16 *)0x27fffa8) ^ 0x2fff) & 0x2fff) & 0xc0b) || (data_022b65b4.unk10 & 0x40)) {
            if (data_022c4560.unk20 != 0) {
                func_02043c7c(5, 0x400);
                w->flags |= 1;
                w->flags |= 2;
            }
        }
        if (w->timer > data_02073bd8[w->idx].limit) {
            func_02043c7c(5, 0x100);
            w->flags |= 1;
        }
    }
    if (w->timer & 0x40) {
        if (func_ov000_0233b660()) {
            w->spr0.flags &= ~2;
            w->spr0.flags |= 1;
            w->spr1.flags |= 2;
            w->spr1.flags &= ~1;
        } else {
            w->spr0.flags &= ~1;
            w->spr0.flags |= 2;
            w->spr1.flags |= 1;
            w->spr1.flags &= ~2;
        }
        func_02032c8c(&w->spr0);
        func_02032c8c(&w->spr1);
    }
    w->timer++;
}
