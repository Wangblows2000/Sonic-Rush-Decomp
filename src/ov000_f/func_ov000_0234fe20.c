#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 x;
    s32 y;
} UnkStruct_0234fe20_Obj;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[8 - 2];
    s32 unk8;
    s32 unkC;
    s32 x;
    s32 y;
    u8 pad18[0x1C - 0x18];
    s32 unk1C;
    s32 unk20;
    u8 pad24[0x34 - 0x24];
    s16 unk34;
    u8 pad36[0x58 - 0x36];
    u32 unk58;
    u8 pad5C[0x32C - 0x5C];
    u32 unk32C;
    u8 pad330[0x384 - 0x330];
    u8 unk384[0x3E8 - 0x384];
    UnkStruct_0234fe20_Obj *unk3E8;
    u8 pad3EC[0x3F0 - 0x3EC];
    s32 unk3F0;
    s32 unk3F4;
    s32 unk3F8;
} UnkStruct_0234fe20;

s32 func_02068bd8(s32 cur, s32 target, s32 c, s32 d, s32 e, s32 f);
s16 func_02068df8(s32 v, s32 a, s32 max);
void func_ov000_023470a0(UnkStruct_0234fe20 *w, s32 v);
void func_0204b7ec(void *p, s32 a, s32 b);

static inline void Arrive_0234fe20(UnkStruct_0234fe20 *w) {
    w->unk58 &= ~0x10;
    w->unk58 &= ~0x100;
    w->unk8 = 1;
    w->unk1 = w->unk3F0;
}

void func_ov000_0234fe20(UnkStruct_0234fe20 *w) {
    s32 d;

    switch (w->unk8) {
    case 0:
        if (w->unk3E8 != NULL) {
            w->unk1C = w->x;
            w->unk20 = w->y;
            if (w->unk3F0 == 0 || w->unk3F0 == 0x80) {
                w->y = func_02068bd8(w->y, w->unk3E8->y, w->unk3F8, 1, 0x400, 0x100);
                d = w->y - w->unk3E8->y;
                if (d < 0) {
                    d = -d;
                }
                if (d < 0x800) {
                    w->x = func_02068bd8(w->x, w->unk3E8->x, w->unk3F4, 1, 0x400, 0x100);
                }
            } else {
                w->x = func_02068bd8(w->x, w->unk3E8->x, w->unk3F4, 1, 0x400, 0x100);
                d = w->x - w->unk3E8->x;
                if (d < 0) {
                    d = -d;
                }
                if (d < 0x800) {
                    w->y = func_02068bd8(w->y, w->unk3E8->y, w->unk3F8, 1, 0x400, 0x100);
                }
            }
            if (w->unk3E8->x != w->x) {
                return;
            }
            if (w->unk3E8->y != w->y) {
                return;
            }
            Arrive_0234fe20(w);
        } else {
            Arrive_0234fe20(w);
        }
        return;
    case 1:
        if (!(w->unk58 & 1)) {
            w->unk1 += 0x80;
            w->unk32C ^= 1;
        }
        w->unkC++;
        if (w->unkC <= 0xc) {
            return;
        }
        if (!(w->unk58 & 1)) {
            return;
        }
        w->unk34 = 0x800;
        if (w->unk32C & 1) {
            w->unk34 = -w->unk34;
        }
        w->unk8 = 2;
        func_0204b7ec(w->unk384, 0, 0x40);
        return;
    default:
        if (w->unk32C & 1) {
            w->unk34 = func_02068df8(w->unk34, -0x10, 0xd00);
        } else {
            w->unk34 = func_02068df8(w->unk34, 0x10, 0xd00);
        }
        func_ov000_023470a0(w, w->unk34);
        return;
    }
}
