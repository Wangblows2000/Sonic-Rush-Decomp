#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
    u8 pad18[0x54 - 0x18];
    u32 unk54;
    u8 pad58[4];
    u16 unk5C;
    u8 pad5E[0x64 - 0x5E];
    s8 unk64;
    u8 pad65;
    s8 unk66;
    s8 unk67;
} UnkStruct_0234be10;

s32 func_02066638(s32 x, s32 y, u8 a, s32 b, s32 c, s32 d);
void func_ov000_0234c4a4(UnkStruct_0234be10 *w, s32 a);

BOOL func_ov000_0234be10(UnkStruct_0234be10 *w, BOOL set) {
    s32 l;
    s32 r;

    if (func_02066638(w->unk10 >> 8, (w->unk14 >> 8) + w->unk67, w->unk54 & 1, 2, 0, 0) > 0) {
        l = func_02066638((w->unk10 >> 8) + w->unk64 - 2, (w->unk14 >> 8) + w->unk67, w->unk54 & 1, 2, 0, 0);
        r = func_02066638((w->unk10 >> 8) + w->unk66 + 2, (w->unk14 >> 8) + w->unk67, w->unk54 & 1, 2, 0, 0);
        if (l <= 0 && r >= 0x10) {
            if (set) {
                w->unk5C &= ~0x400;
                if (w->unk5C & 1) {
                    func_ov000_0234c4a4(w, 0xe);
                } else {
                    func_ov000_0234c4a4(w, 0xd);
                }
            }
            return TRUE;
        }
        if (l >= 0x10 && r <= 0) {
            if (set) {
                w->unk5C &= ~0x400;
                if (w->unk5C & 1) {
                    func_ov000_0234c4a4(w, 0xd);
                } else {
                    func_ov000_0234c4a4(w, 0xe);
                }
            }
            return TRUE;
        }
    }
    return FALSE;
}
