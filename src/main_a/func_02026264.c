#include <nitro/types.h>

typedef struct { u8 unk0[0x8]; u16 unk8; } UnkStruct_02026264;

void *func_02026264(UnkStruct_02026264 *p, u32 idx) {
    return (u8 *)p + p->unk8 + idx * 16;
}
