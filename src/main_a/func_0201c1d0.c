#include <nitro/types.h>

void SND_StopTimer(u32 a, u32 b, u32 c, u32 d);
u32 SND_GetCurrentCommandTag(void);
void SND_FlushCommand(u32 a);
void SND_WaitForCommandProc(u32 a);

typedef struct { u8 unk0[0x24]; u32 unk24_0 : 1; s32 unk24_1 : 1; u32 pad : 30; u8 unk28[0x18]; u32 unk40; u32 unk44; } UnkStruct_0201c1d0;

void func_0201c1d0(UnkStruct_0201c1d0 *s) {
    u32 x;
    if (!s->unk24_1) return;
    SND_StopTimer(s->unk44, 0, 1 << s->unk40, 0);
    x = SND_GetCurrentCommandTag();
    SND_FlushCommand(1);
    SND_WaitForCommandProc(x);
}
