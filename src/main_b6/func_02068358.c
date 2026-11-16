#include <nitro/types.h>

typedef struct {
    u32 vramA;
    u16 refA;
    u16 pad6;
    u32 vramB;
    u16 refB;
} UnkRef_02068358;

typedef struct {
    u8 pad0[0xc4];
    u32 vramA;
    u8 padc8[8];
    u32 vramB;
    u8 padd4[0x2c];
    UnkRef_02068358 *ref;
} UnkSpr_02068358;

u32 VramTex_Alloc(u32 size, BOOL flag);
u32 VramPltt_Alloc(u32 size, BOOL flag);

void func_02068358(UnkSpr_02068358 *s, UnkRef_02068358 *r, u32 sizeA, u32 sizeB) {
    s->ref = r;
    if (r->vramA == 0 && sizeA != 0)
        r->vramA = VramTex_Alloc(sizeA, FALSE);
    if (r->vramB == 0 && sizeB != 0)
        r->vramB = VramPltt_Alloc(sizeB, FALSE);
    r->refA++;
    r->refB++;
    s->vramA = r->vramA;
    s->vramB = r->vramB;
}
