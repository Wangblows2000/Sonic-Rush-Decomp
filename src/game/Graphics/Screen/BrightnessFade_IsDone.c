#include <nitro/types.h>

typedef struct { u8 unk0[0x10]; u16 *unk10; } UnkTask;
extern UnkTask *gBrightnessFadeTask;
BOOL BrightnessFade_IsDone(void) { if (gBrightnessFadeTask == NULL) return TRUE; return (*gBrightnessFadeTask->unk10 & 0x20) != 0; }
