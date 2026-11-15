#include <nitro/types.h>

typedef struct { u8 unk0[0x18]; u32 unk18; } UnkStruct_022c4560;
extern UnkStruct_022c4560 data_022c4560;
void func_0204d768(void);
void func_0204d6d0(s32 a);
void func_0204d7ac(void) { func_0204d768(); data_022c4560.unk18 |= 0x20; func_0204d6d0(3); }
