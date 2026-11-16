#include <nitro/types.h>

typedef struct { u16 elemSize; u16 unk2; u8 data[1]; } UnkSub_02031bd0;
typedef struct { u8 unk0[8]; u8 unk8[6]; u16 unkE; } UnkRes_02031bd0;
UnkRes_02031bd0 *NNS_G3dGetMdlSet(void *p);
void NNS_G3dRenderObjInit(void *a, void *b);

void Model_SetResource(u8 *s, void *res, u32 idx) {
    UnkRes_02031bd0 *r = NNS_G3dGetMdlSet(res);
    UnkSub_02031bd0 *sub = (UnkSub_02031bd0 *)(r->unk8 + r->unkE);
    NNS_G3dRenderObjInit(s + 0x90, (u8 *)r + *(u32 *)(sub->data + sub->elemSize * idx));
}
