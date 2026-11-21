#include <nitro/types.h>

typedef struct { u16 elemSize; u16 unk2; u8 data[1]; } UnkSub_020319a4;
typedef struct { u8 unk0[8]; u8 unk8[6]; u16 unkE; } UnkRes_020319a4;
UnkRes_020319a4 *NNS_G3dGetMdlSet(void *p);

typedef struct { u8 unk0[0x90]; void *unk90; u32 unk94; u32 unk98; } UnkStruct_020319a4;

void Model_SetShapeResource(UnkStruct_020319a4 *s, void *res, u32 idx, u32 a, u32 b) {
    UnkRes_020319a4 *r = NNS_G3dGetMdlSet(res);
    UnkSub_020319a4 *sub = (UnkSub_020319a4 *)(r->unk8 + r->unkE);
    s->unk90 = (u8 *)r + *(u32 *)(sub->data + sub->elemSize * idx);
    s->unk94 = a;
    s->unk98 = b;
}
