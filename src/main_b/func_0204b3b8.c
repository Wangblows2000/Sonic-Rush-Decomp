#include <nitro/types.h>

typedef struct { u8 unk0[0x1c]; u8 unk1C; u8 unk1D; } UnkStruct_022c4560;
extern UnkStruct_022c4560 data_022c4560;
extern void (*data_0207425c[][3])(void);
extern u32 data_ov000_02369ba0;
void func_0201b348(void);
void func_0201b330(void);
void func_ov000_0233bc3c(void);
void func_ov000_023357e0(void);
void func_0204b3b8(void) { func_0201b348(); func_0201b330(); data_0207425c[data_022c4560.unk1C][data_022c4560.unk1D](); func_ov000_0233bc3c(); func_ov000_023357e0(); data_ov000_02369ba0 = 0; }
