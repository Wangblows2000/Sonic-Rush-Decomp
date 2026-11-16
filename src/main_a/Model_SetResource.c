#include <nitro/types.h>

typedef struct { u16 elemSize; u16 unk2; u8 data[1]; } UnkSub_02031bd0;
typedef struct { u8 unk0[8]; u8 unk8[6]; u16 unkE; } UnkRes_02031bd0;
UnkRes_02031bd0 *func_02025ecc(void *p);
void func_02021ef8(void *a, void *b);

void Model_SetResource(u8 *s, void *res, u32 idx) {
    UnkRes_02031bd0 *r = func_02025ecc(res);
    UnkSub_02031bd0 *sub = (UnkSub_02031bd0 *)(r->unk8 + r->unkE);
    func_02021ef8(s + 0x90, (u8 *)r + *(u32 *)(sub->data + sub->elemSize * idx));
}
