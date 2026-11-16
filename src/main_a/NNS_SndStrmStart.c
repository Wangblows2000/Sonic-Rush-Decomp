#include <nitro/types.h>

void SND_StartTimer(u32 a, u32 b, u32 c, u32 d);
void PM_PrependPreSleepCallback(void *p);
void PM_AppendPostSleepCallback(void *p);

typedef struct { u8 unk0[8]; u8 unk8[0xC]; u8 unk14[0x10]; u32 unk24_0 : 1; s32 unk24_1 : 1; u32 pad : 30; u8 unk28[0x18]; u32 unk40; u32 unk44; } UnkStruct_0201c488;

void NNS_SndStrmStart(UnkStruct_0201c488 *s) {
    SND_StartTimer(s->unk44, 0, 1 << s->unk40, 0);
    if (s->unk24_1) return;
    PM_PrependPreSleepCallback(s->unk8);
    PM_AppendPostSleepCallback(s->unk14);
    s->unk24_1 = 1;
}
