#include <nitro/types.h>

typedef struct { u8 unk0[0xA]; u16 unkA; } UnkStruct_02026254;

void *func_02026254(UnkStruct_02026254 *p, u32 idx) {
    return (u8 *)p + p->unkA + idx * 16;
}
