#include <nitro/types.h>

typedef struct {
    u8 pad0[0x30];
    s16 unk30;
    u8 pad32[0x44 - 0x32];
    s16 unk44;
    s16 unk46;
    u8 pad48[0x210 - 0x48];
    s32 unk210;
} UnkStruct_02302398;

void func_ov000_02302694(UnkStruct_02302398 *w);

void func_ov000_02302398(UnkStruct_02302398 *w) {
    w->unk210++;
    w->unk44 = (w->unk210 << 8) / 32;
    w->unk46 = (w->unk210 << 8) / 32;
    if (w->unk210 < 0x20)
        return;
    w->unk210 = 0;
    w->unk44 = 0x100;
    w->unk46 = 0x100;
    w->unk30 = 0;
    func_ov000_02302694(w);
}
