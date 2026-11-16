#include <nitro/types.h>

typedef struct UnkStruct_0230ef9c {
    u8 pad0[0x28];
    s32 z;
    u8 pad2C[0x60 - 0x2c];
    u16 unk60;
} UnkStruct_0230ef9c;

void SpecialStageGimmick_Unk_ov001_023099c4(void);
void SpecialStageGimmick_Destroy(UnkStruct_0230ef9c *p);

void SpecialStageGimmick_Unk_ov001_0230ef9c(UnkStruct_0230ef9c *p) {
    p->unk60 |= 4;
    if (p->z <= 0)
        return;
    SpecialStageGimmick_Unk_ov001_023099c4();
    SpecialStageGimmick_Destroy(p);
}
