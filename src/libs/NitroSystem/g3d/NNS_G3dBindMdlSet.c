#include <nitro/types.h>

typedef struct { u16 elemSize; u16 unk2; u8 data[1]; } UnkSub_0202155c;
typedef struct { u8 unk0; u8 count; u8 unk2[4]; u16 subOffset; } UnkDict_0202155c;
typedef struct { u8 unk0[8]; UnkDict_0202155c dict; } UnkStruct_0202155c;
BOOL NNS_G3dBindMdlTex(void *p, void *a);
BOOL NNS_G3dBindMdlPltt(void *p, void *a);

BOOL NNS_G3dBindMdlSet(UnkStruct_0202155c *s, void *a) {
    u32 i;
    BOOL r = TRUE;
    for (i = 0; i < s->dict.count; i++) {
        UnkSub_0202155c *sub = (UnkSub_0202155c *)((u8 *)&s->dict + s->dict.subOffset);
        u8 *p = (u8 *)s + *(u32 *)(sub->data + sub->elemSize * i);
        r &= NNS_G3dBindMdlTex(p, a);
        r &= NNS_G3dBindMdlPltt(p, a);
    }
    return r;
}
