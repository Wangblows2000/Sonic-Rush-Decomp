#include <nitro/types.h>

void MIi_CpuClear32(u32 data, void *dest, u32 size);
extern u32 data_02077ae4;
extern u32 data_02077ae0;
extern u32 data_02077adc;

typedef struct { u32 unk0; u32 unk4; u32 unk8; u32 unkC; u32 unk10; u32 unk14; u32 unk18; u32 unk1C; u8 unk20[0x34]; } UnkStruct_02021ef8;

void NNS_G3dRenderObjInit(UnkStruct_02021ef8 *s, u32 a) {
    vu32 zero = 0;
    MIi_CpuClear32(zero, s, sizeof(UnkStruct_02021ef8));
    s->unkC = data_02077ae4;
    s->unk14 = data_02077ae0;
    s->unk1C = data_02077adc;
    s->unk4 = a;
}
