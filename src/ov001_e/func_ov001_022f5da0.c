#include <nitro/types.h>

typedef struct UnkStruct_022f5da0_o {
    u8 pad0[0xe4];
    s32 *unkE4;
    u8 padE8[0x114 - 0xe8];
    s32 unk114;
} UnkStruct_022f5da0_o;

typedef struct UnkStruct_022f5da0_w {
    u8 pad0[0x134];
    s32 unk134;
    s32 unk138;
} UnkStruct_022f5da0_w;

typedef struct UnkStruct_022f5da0 {
    u8 pad0[0x74];
    void (*state)(struct UnkStruct_022f5da0 *p);
    u8 pad78[0x84 - 0x78];
    UnkStruct_022f5da0_o *unk84;
    u8 pad88[0x1c8 - 0x88];
    s16 unk1C8;
    u8 pad1CA[0x1d0 - 0x1ca];
    s32 unk1D0;
    s16 unk1D4;
    u8 pad1D6[0x1dc - 0x1d6];
    UnkStruct_022f5da0_w *work;
} UnkStruct_022f5da0;

extern s32 data_ov001_0231f520;
extern s32 data_ov001_0231f524;

void func_ov001_022f5a24(UnkStruct_022f5da0 *p);
void func_ov001_022fc09c(UnkStruct_022f5da0 *p, s32 x, s32 y, s32 z, u16 ang);

void func_ov001_022f5da0(UnkStruct_022f5da0 *p) {
    UnkStruct_022f5da0_o *o = p->unk84;
    UnkStruct_022f5da0_w *w;

    if (o->unk114 >= 0) {
        if (p->unk1D0 > *o->unkE4 + o->unk114)
            return;
    } else {
        if (p->unk1D0 < *o->unkE4 + o->unk114)
            return;
    }
    w = p->work;
    p->state = func_ov001_022f5a24;
    *o->unkE4 = p->unk1D0;
    o->unk114 = 0;
    func_ov001_022fc09c(p, w->unk134 - data_ov001_0231f524, -(w->unk138 + data_ov001_0231f520), 0,
                        ((p->unk1D0 >> 13) << 16) / 360 - 0xa38f);
    p->unk1C8 = 0;
    p->unk1D4 = 0;
}
