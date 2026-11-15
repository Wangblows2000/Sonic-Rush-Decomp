#include <nitro/types.h>

typedef struct { u32 unk0; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
void func_0204bb78(void);
UnkTask *func_020372d0(void (*fn)(void), s32 a, s32 b, s32 c, s32 d, s32 e);
void func_0204d5f8(u32 v) { func_020372d0(func_0204bb78, 0, 0, 0, 0, 8)->unk10->unk0 = v; }
