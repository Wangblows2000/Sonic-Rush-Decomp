#include <nitro/types.h>

typedef struct { u16 unk0; u16 unk2; u8 unk4; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
extern UnkTask *gScreenShakeTask;
void ScreenShake_Stop(void);
void ScreenShake_Start(u8 v) { UnkWork *w; if (gScreenShakeTask == NULL) return; w = gScreenShakeTask->unk10; if (v == 0) { ScreenShake_Stop(); return; } if (w->unk4 <= v) { w->unk4 = v; w->unk0 = 1; w->unk2 = 0; } }
