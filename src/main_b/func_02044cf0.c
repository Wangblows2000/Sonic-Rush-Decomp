#include <nitro/types.h>

typedef struct { u32 unk0; void *tasks[2]; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
extern UnkTask *data_022c447c;
void func_020371c8(void *p);
void func_02044cf0(void) { s32 i; UnkWork *w = data_022c447c->unk10; for (i = 0; i < 2; i++) { if (w->tasks[i] != NULL) func_020371c8(w->tasks[i]); } func_020371c8(data_022c447c); }
