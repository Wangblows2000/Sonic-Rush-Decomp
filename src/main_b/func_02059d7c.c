#include <nitro/types.h>

typedef struct { u8 unk0[0x18]; void (*cb)(void); } UnkStruct_02059d7c;
extern void *data_022b4574;
void func_020371c8(void *p);
void func_02059d7c(UnkStruct_02059d7c *s) { void (*cb)(void) = s->cb; func_020371c8(data_022b4574); if (cb != NULL) cb(); }
