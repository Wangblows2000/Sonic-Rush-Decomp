#include <nitro/types.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
void StrmCallback_stream(void *s, u32 a);
void SND_StartTimer(u32 a, u32 b, u32 c, u32 d);

typedef struct { u8 unk0[0x24]; u32 unk24_0 : 1; s32 unk24_1 : 1; u32 pad : 30; u8 unk28[0x10]; u32 unk38; u8 unk3C[4]; u32 unk40; u32 unk44; } UnkStruct_0201c158;

void EndSleep_stream(UnkStruct_0201c158 *s) {
    if (!s->unk24_1) return;
    while (s->unk38 != 0) {
        u32 e = OS_DisableInterrupts();
        StrmCallback_stream(s, 1);
        OS_RestoreInterrupts(e);
    }
    SND_StartTimer(s->unk44, 0, 1 << s->unk40, 0);
}
