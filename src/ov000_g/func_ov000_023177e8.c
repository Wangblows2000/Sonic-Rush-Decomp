#include <nitro/types.h>

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0x34 - 2];
    s16 unk34;
    u8 pad36[0x58 - 0x36];
    u32 unk58;
} UnkStruct_023177e8_P;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xC - 2];
    s32 unkC;
    u8 pad10[0x34 - 0x10];
    s16 unk34;
    u8 pad36[0x54 - 0x36];
    u32 unk54;
    u8 pad58[0x204 - 0x58];
    UnkStruct_023177e8_P *unk204;
} UnkStruct_023177e8;

s16 ObjSpd_DownSet(s16 a, s32 b);
s16 ObjSpd_UpSet(s16 a, s32 b, s32 c);

void func_ov000_023177e8(UnkStruct_023177e8 *w) {
    UnkStruct_023177e8_P *p = w->unk204;
    s32 v;

    if (p != NULL) {
        if (!(p->unk58 & 0x2000)) {
            w->unkC = 8;
            if (w->unk204->unk58 & 1) {
                w->unk1 = 0;
                w->unkC = 0;
            } else {
                w->unkC = (s8)w->unk1 << 8;
            }
            w->unk34 = w->unk204->unk34;
            w->unkC -= w->unk34;
            w->unk204 = NULL;
            return;
        }
        w->unk1 = p->unk1;
        w->unk34 = w->unk204->unk34;
        return;
    }
    if (!(w->unk54 & 2))
        return;
    v = w->unkC;
    if ((v < 0 ? -v : v) < 0x100 && (w->unk34 < 0 ? -w->unk34 : w->unk34) < 0x80) {
        w->unkC = 0;
        w->unk34 = 0;
    } else if (v < 0) {
        if (w->unk34 > 0)
            w->unk34 = ObjSpd_DownSet(w->unk34, 0x20);
        w->unk34 = ObjSpd_UpSet(w->unk34, -0x20, 0x300);
    } else {
        if (w->unk34 < 0)
            w->unk34 = ObjSpd_DownSet(w->unk34, 0x20);
        w->unk34 = ObjSpd_UpSet(w->unk34, 0x20, 0x300);
    }
    w->unkC -= w->unk34;
    w->unk1 = (w->unkC & 0xffff) >> 8;
    if (w->unk34 == 0 && w->unk1 == 0)
        w->unk54 &= ~2;
}
