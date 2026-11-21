#include <nitro/types.h>

typedef struct { u32 unk0; u32 unk4; u32 unk8; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
extern u32 gScreenFlashTask;
void ScreenFlash_Destructor(UnkTask *t) { UnkWork *w = t->unk10; *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | (w->unk4 << 8); *(vu32 *)0x4001000 = (*(vu32 *)0x4001000 & ~0x1f00) | (w->unk8 << 8); gScreenFlashTask = 0; }
