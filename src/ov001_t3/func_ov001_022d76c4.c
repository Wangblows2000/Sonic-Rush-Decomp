#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2c];
} UnkAnim_022d76c4;

typedef struct {
    u16 x0;
    u16 y0;
    u16 unk4;
    u16 unk6;
    u16 unk8;
    u16 unka;
    s32 unkc;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1c;
    s32 unk20;
    UnkAnim_022d76c4 anims[6];
    struct UnkStruct_022d76c4 *parent;
} UnkWork_022d76c4;

typedef struct UnkStruct_022d76c4 {
    u8 pad0[0x258];
    s32 unk258;
    u8 pad25c[0x824 - 0x25c];
    s32 unk824;
} UnkStruct_022d76c4;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022d76c4 *work;
} UnkTask_022d76c4;

extern u16 data_ov001_0231616c[];
extern u16 data_ov001_0231616e[];
extern u16 data_ov001_02316170[];
extern u16 data_ov001_02316172[];

void func_02037038(s32 a, s32 b);
UnkTask_022d76c4 *func_020372d0(void (*fn)(void), void (*dtor)(void), s32 b, s32 c, s32 d, s32 size);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void func_ov001_022d83fc(void);
void func_02040ef0(UnkAnim_022d76c4 *a, s32 b, s32 c);

#pragma thumb on
void func_ov001_022d76c4(UnkStruct_022d76c4 *p, s32 a, s32 b) {
    UnkWork_022d76c4 *w;
    vu16 zero;
    s32 i;

    func_02037038(0x3000, 0x3000);
    w = func_020372d0(func_ov001_022d83fc, NULL, 0, 0x3000, 0x1000, sizeof(UnkWork_022d76c4))->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkWork_022d76c4));
    w->parent = p;
    for (i = 0; i < 6; i++) {
        func_02040ef0(&w->anims[i], p->unk824, i);
    }
    if (a >= 7) a = 0;
    if (b >= 7) b = 0;
    w->x0 = data_ov001_0231616e[a * 4];
    w->y0 = data_ov001_0231616c[a * 4];
    w->unk4 = data_ov001_02316172[b * 4];
    w->unk6 = data_ov001_02316170[b * 4];
    w->unk8 = data_ov001_0231616e[b * 4];
    w->unka = data_ov001_0231616c[b * 4];
    w->unkc = 0;
    w->unk10 = 10;
    w->unk14 = 20;
    w->unk18 = 30;
    w->unk1c = 40;
    w->unk20 = 50;
    p->unk258 = 0;
}
#pragma thumb reset
