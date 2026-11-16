#include <nitro/types.h>

void GX_BeginLoadTexPltt(void);
void GX_LoadTexPltt(void *src, u32 offset, u32 size);
void GX_EndLoadTexPltt(void);

typedef struct { u8 unk0[0x2C]; u32 unk2C; u16 unk30; u16 unk32; u8 unk34[4]; u32 unk38; } UnkStruct_02021a64;

void NNS_G3dPlttLoad(UnkStruct_02021a64 *s, BOOL lock) {
    if (lock) GX_BeginLoadTexPltt();
    GX_LoadTexPltt((u8 *)s + s->unk38, (s->unk2C & 0xFFFF) << 3, s->unk30 << 3);
    s->unk32 |= 1;
    if (!lock) return;
    GX_EndLoadTexPltt();
}
