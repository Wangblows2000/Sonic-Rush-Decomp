#include <nitro/types.h>

void MI_CpuCopy8(const void *src, void *dst, u32 size);
typedef struct { u16 unk0; u16 unk2; u8 unk4[6]; } UnkOut_02042a20;
void func_02042a20(UnkOut_02042a20 *o, s32 n) { vu16 *q = (vu16 *)0x27ffc40; u8 *p = (*q == 2) ? (u8 *)(q + 1) : NULL; o->unk0 = *(u16 *)(p + 0x36); o->unk2 = n + 1; MI_CpuCopy8(p + 4, o->unk4, 6); }
