#include <nitro/types.h>

typedef struct { u16 unk0; u16 unk2; } UnkStruct_022c5220;
extern UnkStruct_022c5220 data_022c5220[16];
void func_0206d484(void) { u8 i; for (i = 0; i < 16; i++) { data_022c5220[i].unk0 = 0; data_022c5220[i].unk2 = 0; } }
