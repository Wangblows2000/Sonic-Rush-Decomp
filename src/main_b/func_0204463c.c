#include <nitro/types.h>

typedef struct { u16 unk0; u16 unk2; u8 unk4; u8 unk5; u8 unk6; u8 unk7; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
extern UnkTask *data_022c4478;
void func_0204463c(void) { UnkWork *w; if (data_022c4478 == NULL) return; w = data_022c4478->unk10; w->unk6 = 0; w->unk7 = 0; w->unk4 = 0; w->unk0 = 0; w->unk2 = 0; }
