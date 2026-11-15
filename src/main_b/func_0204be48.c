#include <nitro/types.h>

typedef struct { u32 flags; } UnkWork;
typedef struct { u32 unk0; u32 unk4; void (*fn)(void); u32 unkC; UnkWork *unk10; } UnkTask;
extern UnkTask *data_022b4574;
void func_0204b718(s32 a);
void func_0204be14(void);
void func_0204bdf8(void);
void func_0204be48(void) { UnkWork *w = data_022b4574->unk10; func_0204b718(0); if (w->flags & 2) data_022b4574->fn = func_0204be14; else data_022b4574->fn = func_0204bdf8; }
