#include <nitro/types.h>

typedef struct { u16 unk0; u16 unk2; u16 unk4; } UnkStruct_0204045c;
extern void (*data_022c197c)(void *);
extern u8 data_02078ad8;
void func_0203c470(void);
void func_0204045c(UnkStruct_0204045c *s) { if (s->unk0 != 14) return; if (s->unk4 != 10) return; if (data_022c197c != NULL) data_022c197c(&data_02078ad8); func_0203c470(); }
