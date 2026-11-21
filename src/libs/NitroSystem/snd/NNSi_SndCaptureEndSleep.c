#include <nitro/types.h>

void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_FlushRange(void *start, u32 size);
void SND_StartTimer(u32 a, u32 b, u32 c, u32 d);

typedef struct { u32 unk0; u8 unk4[8]; void *bufL; void *bufR; u32 size; u8 unk18[4]; u32 unk1C; u8 unk20[4]; u32 unk24; u32 unk28; s32 unk2C; } UnkStruct_0201c8d0;
extern UnkStruct_0201c8d0 data_02083e10;

void NNSi_SndCaptureEndSleep(void) {
    UnkStruct_0201c8d0 *p = &data_02083e10;
    vu32 z0, z1;
    if (p->unk0 == 0) return;
    p->unk1C = 0;
    z0 = 0;
    MIi_CpuClear32(z0, p->bufL, p->size);
    z1 = 0;
    MIi_CpuClear32(z1, p->bufR, p->size);
    DC_FlushRange(p->bufL, p->size);
    DC_FlushRange(p->bufR, p->size);
    SND_StartTimer(p->unk24, p->unk28, p->unk2C >= 0 ? 1 << p->unk2C : 0, 0);
}
