#include <nitro/types.h>

typedef struct { u16 unk0; u16 unk2; u8 unk4; u8 unk5; u8 unk6; u8 unk7; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
extern UnkTask *gScreenShakeTask;
void ScreenShake_Stop(void) { UnkWork *w; if (gScreenShakeTask == NULL) return; w = gScreenShakeTask->unk10; w->unk6 = 0; w->unk7 = 0; w->unk4 = 0; w->unk0 = 0; w->unk2 = 0; }
