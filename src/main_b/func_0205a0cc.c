#include <nitro/types.h>

typedef struct UnkWork { void (*fn)(struct UnkWork *); void (*fn2)(struct UnkWork *); u32 flags; s32 count; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
extern UnkTask *gTaskCurrent;
void func_0205a0cc(void) { UnkWork *w = gTaskCurrent->unk10; w->count++; if (w->fn != NULL) w->fn(w); if (!(w->flags & 0x80000000)) return; if (w->fn2 == NULL) return; w->fn2(w); }
