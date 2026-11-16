#include <nitro/types.h>

typedef struct UnkStruct_02327ef4_2 {
    u8 pad[0x2a0];
    u32 unk2A0;
    u8 pad1[4];
    void *unk2A8;
} UnkStruct_02327ef4_2;

typedef struct UnkStruct_02327ef4 {
    u8 pad[0x10];
    UnkStruct_02327ef4_2 *unk10;
} UnkStruct_02327ef4;

void func_0202566c(void *p);
void StageObj_Destructor(UnkStruct_02327ef4 *s);

void BossEffect_Destructor(UnkStruct_02327ef4 *s) {
    UnkStruct_02327ef4_2 *p = s->unk10;
    if (!(p->unk2A0 & 1))
        func_0202566c(p->unk2A8);
    StageObj_Destructor(s);
}
