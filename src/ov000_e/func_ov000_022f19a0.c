#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    u8 pad14[4];
    s32 unk18;
    u8 pad1C[0x2E - 0x1C];
    s16 unk2E;
    u8 pad30[2];
    s16 unk32;
    u8 pad34[0x45E - 0x34];
    u16 unk45E;
} UnkStruct_022f19a0;

void func_ov000_022f898c(void);

void func_ov000_022f19a0(UnkStruct_022f19a0 *s) {
    s->unk2E -= 0x80;
    if (s->unk2E < -0x1000)
        s->unk2E = -0x1000;
    if (s->unk10 <= 0x1a000) {
        s->unk2E = 0;
        s->unk10 = 0x1a000;
    }
    s->unk32 -= 0x80;
    if (s->unk32 < -0x1000)
        s->unk32 = -0x1000;
    if (s->unk18 <= -0x1f400) {
        s->unk32 = 0;
        s->unk18 = -0x1f400;
    }
    if (s->unk32 != 0) {
        if (s->unk45E > 0xa000)
            s->unk45E -= 0x200;
    } else {
        if (s->unk45E != 0)
            s->unk45E += 0x200;
    }
    if (s->unk45E != 0)
        return;
    if (s->unk10 != 0x1a000)
        return;
    if (s->unk18 != -0x1f400)
        return;
    func_ov000_022f898c();
}
