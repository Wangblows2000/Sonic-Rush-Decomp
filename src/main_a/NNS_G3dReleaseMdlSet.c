#include <nitro/types.h>

typedef struct { u16 elemSize; u16 unk2; u8 data[1]; } UnkSub_020214f0;
typedef struct { u8 unk0; u8 count; u8 unk2[4]; u16 subOffset; } UnkDict_020214f0;
typedef struct { u8 unk0[8]; UnkDict_020214f0 dict; } UnkStruct_020214f0;
void NNS_G3dReleaseMdlTex(void *p);
void NNS_G3dReleaseMdlPltt(void *p);

void NNS_G3dReleaseMdlSet(UnkStruct_020214f0 *s) {
    u32 i;
    for (i = 0; i < s->dict.count; i++) {
        UnkSub_020214f0 *sub = (UnkSub_020214f0 *)((u8 *)&s->dict + s->dict.subOffset);
        u8 *p = (u8 *)s + *(u32 *)(sub->data + sub->elemSize * i);
        NNS_G3dReleaseMdlTex(p);
        NNS_G3dReleaseMdlPltt(p);
    }
}
