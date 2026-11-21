#include <nitro/types.h>

typedef struct { u16 unk0; u16 unk2; u16 unk4; } UnkStruct_0204045c;
extern void (*sTraceFunc)(void *);
extern u8 data_02078ad8;
void WFS_Start(void);
void WH_CallbackForWFS(UnkStruct_0204045c *s) { if (s->unk0 != 14) return; if (s->unk4 != 10) return; if (sTraceFunc != NULL) sTraceFunc(&data_02078ad8); WFS_Start(); }
