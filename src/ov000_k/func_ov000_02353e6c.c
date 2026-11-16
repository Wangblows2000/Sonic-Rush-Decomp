#include <nitro/types.h>

typedef struct {
    u8 pad0[0x5C];
    u16 unk5C;
    u8 pad5E[0x32C - 0x5E];
    u32 unk32C;
    u32 unk330;
    u8 pad334[0x3E8 - 0x334];
    void *unk3E8;
} UnkStruct_02353e6c;

void func_ov000_02353d6c(UnkStruct_02353e6c *w, void *p, s32 a, s32 b);

void func_ov000_02353e6c(UnkStruct_02353e6c *w, void *p, s32 a, s32 b) {
    if (w->unk3E8 == p) {
        return;
    }
    func_ov000_02353d6c(w, p, a, b);
    w->unk32C |= 6;
    w->unk5C &= ~0x200;
    w->unk330 |= 0x20000;
}
