#include <nitro/types.h>

typedef struct UnkStruct_0230ef64 {
    u8 pad0[0x28];
    s32 z;
    u8 pad2C[0x60 - 0x2c];
    u16 unk60;
} UnkStruct_0230ef64;

void func_ov001_02309994(void);
void SpecialStageGimmick_Destroy(UnkStruct_0230ef64 *p);

void func_ov001_0230ef64(UnkStruct_0230ef64 *p) {
    p->unk60 |= 4;
    if (p->z <= 0)
        return;
    func_ov001_02309994();
    SpecialStageGimmick_Destroy(p);
}
