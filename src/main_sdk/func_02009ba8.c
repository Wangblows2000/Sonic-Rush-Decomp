#include <nitro/types.h>

void func_02008ee8(void);
void func_02008ed4(void);
void DC_StoreRange(void *p, u32 size);
typedef struct { u8 pad[0x3c]; u32 arr[1]; } UnkStruct_02009ba8;
void func_02009ba8(UnkStruct_02009ba8 *p, s32 i, u32 v) { func_02008ee8(); p->arr[i] = v; DC_StoreRange(&p->arr[i], 4); func_02008ed4(); }
