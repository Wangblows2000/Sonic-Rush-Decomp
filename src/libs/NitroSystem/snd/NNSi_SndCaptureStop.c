#include <nitro/types.h>

void SND_StopTimer(u32 a, u32 b, u32 c, u32 d);
u32 SND_GetCurrentCommandTag(void);
void SND_FlushCommand(u32 a);
void SND_WaitForCommandProc(u32 a);
BOOL OS_ReceiveMessage(void *queue, void *msg, s32 flags);
void NNS_SndUnlockCapture(u32 a);
void NNS_SndUnlockChannel(void *p);
void NNS_SndFreeAlarm(s32 a);
void SND_SetOutputSelector(u32 a, u32 b, u32 c, u32 d);

typedef struct { u32 unk0; u32 unk4; u8 unk8[0x18]; void *unk20; u32 unk24; u32 unk28; s32 unk2C; } UnkStruct_0201c9d0;
extern UnkStruct_0201c9d0 data_02083e10;
extern u8 data_02083dfc[];

void NNSi_SndCaptureStop(void) {
    UnkStruct_0201c9d0 *p = &data_02083e10;
    BOOL has;
    if (p->unk0 == 0) return;
    has = p->unk2C >= 0;
    SND_StopTimer(p->unk24, p->unk28, has ? 1 << p->unk2C : 0, 0);
    if (has) {
        u32 x = SND_GetCurrentCommandTag();
        SND_FlushCommand(1);
        void *q;
        SND_WaitForCommandProc(x);
        q = data_02083dfc;
        while (OS_ReceiveMessage(q, NULL, 0)) {
        }
    }
    if (p->unk28) NNS_SndUnlockCapture(p->unk28);
    if (p->unk20) NNS_SndUnlockChannel(p->unk20);
    if (has) NNS_SndFreeAlarm(p->unk2C);
    if (p->unk4 == 1) SND_SetOutputSelector(0, 0, 0, 0);
    p->unk0 = 0;
}
