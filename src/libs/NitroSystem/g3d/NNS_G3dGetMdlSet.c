#include <nitro/types.h>

typedef struct { u8 unk0[0xC]; u16 unkC; } UnkStruct_02025ecc;

void *NNS_G3dGetMdlSet(UnkStruct_02025ecc *p) {
    return (u8 *)p + *(u32 *)((u8 *)p + p->unkC);
}
