#include <nitro/types.h>

typedef struct { u8 unk0[8]; u32 flags; u8 unkC[0xc]; void (*cb)(void); } UnkStruct_02059f24;
extern void *data_022b4574;
void func_020371c8(void *p);
void func_0205a148(s32 a, s32 b);
void func_02059f24(UnkStruct_02059f24 *s) { void (*cb)(void); if (s->flags & 1) { func_020371c8(data_022b4574); func_0205a148(2, 0); return; } cb = s->cb; func_020371c8(data_022b4574); if (cb != NULL) cb(); }
