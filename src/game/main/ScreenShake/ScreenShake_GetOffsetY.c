#include <nitro/types.h>

typedef struct { u8 unk0[6]; s8 unk6; s8 unk7; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
extern UnkTask *gScreenShakeTask;
s8 ScreenShake_GetOffsetY(void) { if (gScreenShakeTask == NULL) return 0; return gScreenShakeTask->unk10->unk7; }
