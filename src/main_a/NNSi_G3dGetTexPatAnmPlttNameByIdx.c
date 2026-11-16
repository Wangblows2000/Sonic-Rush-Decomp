#include <nitro/types.h>

typedef struct { u8 unk0[0xA]; u16 unkA; } UnkStruct_02026254;

void *NNSi_G3dGetTexPatAnmPlttNameByIdx(UnkStruct_02026254 *p, u32 idx) {
    return (u8 *)p + p->unkA + idx * 16;
}
