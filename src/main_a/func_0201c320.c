#include <nitro/types.h>

void SND_StopTimer(u32 a, u32 b, u32 c, u32 d);
void PM_DeletePreSleepCallback(void *p);
void PM_DeletePostSleepCallback(void *p);
u32 SND_GetCurrentCommandTag(void);
void SND_FlushCommand(u32 a);
void SND_WaitForCommandProc(u32 a);
void func_0201c2ec(void *s);

typedef struct { u8 unk0[8]; u8 unk8[0xC]; u8 unk14[0x10]; u32 unk24_0 : 1; s32 unk24_1 : 1; u32 pad : 30; u8 unk28[0x18]; u32 unk40; u32 unk44; } UnkStruct_0201c320;

void func_0201c320(UnkStruct_0201c320 *s) {
    if (s->unk24_1) {
        u32 x;
        SND_StopTimer(s->unk44, 0, 1 << s->unk40, 0);
        PM_DeletePreSleepCallback(s->unk8);
        PM_DeletePostSleepCallback(s->unk14);
        s->unk24_1 = 0;
        x = SND_GetCurrentCommandTag();
        SND_FlushCommand(1);
        SND_WaitForCommandProc(x);
    }
    func_0201c2ec(s);
}
