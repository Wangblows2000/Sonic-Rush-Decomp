#include <nitro/types.h>

typedef struct UnkStruct_02314758 UnkStruct_02314758;
struct UnkStruct_02314758 {
    u8 pad[0xc];
    s32 unkC;
    u8 pad1[0x54 - 0x10];
    u32 unk54;
    u8 pad2[0x204 - 0x58];
    UnkStruct_02314758 *unk204;
    u8 pad3[0x3e8 - 0x208];
    UnkStruct_02314758 *unk3E8;
};

void func_ov000_02314758(UnkStruct_02314758 *s) {
    UnkStruct_02314758 *p = s->unk204;
    if (p == NULL)
        return;
    s->unkC = p->unkC;
    if (p->unk3E8 != s) {
        s->unk54 &= ~2;
        s->unk204 = NULL;
    }
}
