#include <nitro/types.h>

void SND_StopTimer(u32 a, u32 b, u32 c, u32 d);
u32 SND_GetCurrentCommandTag(void);
void SND_FlushCommand(u32 a);
void SND_WaitForCommandProc(u32 a);

typedef struct { u32 unk0; u8 unk4[0x20]; u32 unk24; u32 unk28; s32 unk2C; } UnkStruct_0201c970;
extern UnkStruct_0201c970 data_02083e10;

void NNSi_SndCaptureBeginSleep(void) {
    UnkStruct_0201c970 *p = &data_02083e10;
    u32 x;
    if (p->unk0 == 0) return;
    SND_StopTimer(p->unk24, p->unk28, p->unk2C >= 0 ? 1 << p->unk2C : 0, 0);
    x = SND_GetCurrentCommandTag();
    SND_FlushCommand(1);
    SND_WaitForCommandProc(x);
}
