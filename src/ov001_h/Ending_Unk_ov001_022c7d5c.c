#include <nitro/types.h>

typedef struct UnkRes_022c7d5c {
    u8 pad0[0x18];
    u16 unk18;
    u16 unk1A;
} UnkRes_022c7d5c;

typedef struct UnkHdr_022c7d5c {
    u8 pad0[0x18];
    u32 unk18;
} UnkHdr_022c7d5c;

typedef struct UnkSpr_022c7d5c {
    u8 pad0[0x2c];
    u32 unk2C;
    u8 pad30[0x44 - 0x30];
    u16 unk44;
    u8 pad46[0x48 - 0x46];
} UnkSpr_022c7d5c;

typedef struct UnkStruct_022c7d5c {
    u8 pad0[0x10];
    UnkRes_022c7d5c *res[0x13];
    u8 bgs[0x13][0x40];
    UnkHdr_022c7d5c *hdr[2];
    UnkSpr_022c7d5c sprs[5];
} UnkStruct_022c7d5c;

extern u8 data_ov001_02315ee4[];
extern u32 data_ov001_02315ef8[];
extern u8 data_ov001_02315eb4[];
extern u16 data_ov001_02315ed8[];
extern u16 data_ov001_02315ecc[];

void Background_Init(void *buf, void *res, s32 a, s32 b, s32 c, s32 d, s32 e);
u32 ObjVram_Alloc(u32 a, u32 b);
void AnimatorSprite_Init(void *s, void *h, u16 idx, u32 d, u32 e, u32 f, u32 g, u32 hh, u32 i);
void AnimatorSprite_ProcessAnimation(void *s, s32 a, s32 b);

#define ENT1C(h, idx) (*(u16 *)((u8 *)(h) + (h)->unk18 + (idx) * 0x14 + 0x1c))

void Ending_Unk_ov001_022c7d5c(UnkStruct_022c7d5c *w) {
    s32 i;
    for (i = 0; i < 0x13; i++) {
        Background_Init(w->bgs[i], w->res[i], 0x38, data_ov001_02315ef8[i], data_ov001_02315ee4[i], w->res[i]->unk18, w->res[i]->unk1A);
    }
    for (i = 0; i < 5; i++) {
        UnkHdr_022c7d5c *h = w->hdr[data_ov001_02315eb4[i]];
        UnkSpr_022c7d5c *s = &w->sprs[i];
        u16 idx = data_ov001_02315ed8[i];
        AnimatorSprite_Init(s, h, idx, 0x60, 1, 0, ObjVram_Alloc(1, ENT1C(h, idx)), 0, 0x5000600);
        s->unk2C |= 4;
        s->unk44 = data_ov001_02315ecc[i];
        AnimatorSprite_ProcessAnimation(s, 0, 0);
    }
}
