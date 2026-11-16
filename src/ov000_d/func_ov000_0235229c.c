#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_0235229c_3E8;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    u8 pad20[0x30 - 0x20];
    s16 unk30;
    s16 unk32;
    u8 pad34[0x58 - 0x34];
    u32 unk58;
    u8 pad5C[0x32C - 0x5C];
    u32 unk32C;
    u8 pad330[0x3E8 - 0x330];
    UnkStruct_0235229c_3E8 *unk3E8;
} UnkStruct_0235229c;

s32 ObjShift_Set(s32 a, s32 b, s32 c, s32 d, s32 e);
s16 ObjSpd_DownSet(s16 v, s32 a);
s16 ObjSpd_UpSet(s16 v, s32 a, s32 b);
void func_ov000_023521f0(UnkStruct_0235229c *w);

void func_ov000_0235229c(UnkStruct_0235229c *w) {
    if (w->unk3E8 != NULL) {
        w->unk1C = w->unk10;
        w->unk10 = ObjShift_Set(w->unk10, w->unk3E8->unk10, 4, 0, 0x100);
        if (!(w->unk32C & 1)) {
            s32 y = w->unk3E8->unk14 + 0x1200;
            if (w->unk14 >= y) {
                w->unk14 = y;
                w->unk30 = 0;
                w->unk32C |= 1;
                w->unk58 &= ~0x80;
            }
        }
        if (w->unk32C & 1) {
            if (w->unk18 >= 0x2800) {
                w->unk58 |= 0x80;
                w->unk32 = ObjSpd_DownSet(w->unk32, 0x50);
            } else {
                w->unk30 = ObjSpd_UpSet(w->unk30, 8, 0x600);
                w->unk32 = ObjSpd_UpSet(w->unk32, 0x36, 0x600);
            }
        }
    }
    if ((w->unk32C & 1) && w->unk32 == 0) {
        w->unk58 &= ~0x100;
    }
    if (w->unk58 & 1) {
        func_ov000_023521f0(w);
    }
}
