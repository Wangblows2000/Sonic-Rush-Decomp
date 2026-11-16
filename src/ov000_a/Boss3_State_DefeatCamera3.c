#include <nitro/types.h>

typedef struct UnkStruct_022d9538_a {
    u8 pad[0x48];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 pad1[0x10c - 0x54];
    u8 unk10C;
    u8 pad2[0x114 - 0x10d];
    s32 unk114;
    u8 pad3[0x170 - 0x118];
} UnkStruct_022d9538_a;

typedef struct UnkStruct_022d9538_brl {
    u8 pad[0x54];
    u32 unk54;
    u8 pad1[0x2c4 - 0x58];
    UnkStruct_022d9538_a unk2C4;
    void *unk434;
} UnkStruct_022d9538_brl;

typedef struct UnkStruct_022d9538_rock {
    u8 pad[0x5c];
    u16 unk5C;
} UnkStruct_022d9538_rock;

typedef struct UnkStruct_022d9538 UnkStruct_022d9538;
struct UnkStruct_022d9538 {
    u8 pad[0x54];
    u32 unk54;
    u8 pad1[0x68 - 0x58];
    void (*func)(UnkStruct_022d9538 *s);
    u8 pad2[0x214 - 0x6c];
    UnkStruct_022d9538_brl *unk214;
    UnkStruct_022d9538_rock *unk218;
    u8 pad3[0x224 - 0x21c];
    u16 unk224;
    u8 pad4[0x350 - 0x226];
    s32 unk350;
};

typedef struct UnkStruct_022d9538_c {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    u8 pad[0x18 - 0xc];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad1[0x30 - 0x24];
    u16 unk30;
    u8 pad2[0x3c - 0x32];
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    s32 unk54;
    s32 unk58;
    s32 unk5C;
    u8 pad3[0x6c - 0x60];
    u16 unk6C;
    u8 pad4[0x78 - 0x6e];
    s32 unk78;
    u8 pad5[0x80 - 0x7c];
    u16 unk80;
    u8 pad6[0x84 - 0x82];
    s32 unk84;
    u8 pad7[0x8c - 0x88];
    u16 unk8C;
    u16 unk8E;
    u8 pad8[0x92 - 0x90];
    u16 unk92;
} UnkStruct_022d9538_c;

typedef struct UnkStruct_022d9538_d {
    u8 pad[0xdc];
    s32 unkDC;
} UnkStruct_022d9538_d;

typedef struct UnkStruct_022d9538_q {
    u8 pad[0x13e];
    u8 unk13E;
    u8 unk13F;
    u8 unk140;
    u8 pad1[0x248 - 0x141];
    s32 unk248;
} UnkStruct_022d9538_q;

typedef struct UnkStruct_022d9538_g {
    u8 pad[8];
    u16 unk8;
    u16 unkA;
    u8 pad1[0x48 - 0xc];
    s16 unk48;
} UnkStruct_022d9538_g;

extern UnkStruct_022d9538_c data_ov000_0236a48c;
extern UnkStruct_022d9538_g gDisplayMain;
extern void *gZoneActArchive;
extern u8 data_ov000_023654a0[];

void NNS_FndMountArchive(void *ctx, void *a, void *b);
void *NNS_FndGetArchiveFileByIndex(void *ctx, u32 id);
void NNS_FndUnmountArchive(void *ctx);
void Background_Init(void *buf, void *res, s32 a, s32 b, s32 c, s32 d, s32 e);
void Background_Draw(void *buf);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void Boss3_State_DefeatFade_ov000_022d94fc(UnkStruct_022d9538 *s);

static inline u32 GX_GetVisiblePlane(void) {
    return (*(vu32 *)0x4000000 & 0x1f00) >> 8;
}

static inline void GX_SetVisiblePlane(u32 p) {
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | (p << 8);
}

void Boss3_State_DefeatCamera3(UnkStruct_022d9538 *s) {
    UnkStruct_022d9538_brl *a = s->unk214;
    UnkStruct_022d9538_c *c = (UnkStruct_022d9538_c *)(u32)&data_ov000_0236a48c;
    u8 buf[0x40];
    u8 arc[0x5c];
    c->unk30 = 0x1000;
    c->unk44 = 0;
    c->unk40 = c->unk44;
    c->unk3C = c->unk40;
    c->unk8 = 0;
    c->unk4 = c->unk8;
    c->unk0 = c->unk4;
    c->unk50 = 0;
    c->unk4C = c->unk50;
    c->unk48 = c->unk4C;
    c->unk54 = a->unk2C4.unk48 - 0x20000;
    c->unk58 = a->unk2C4.unk4C + 0x20000;
    c->unk5C = a->unk2C4.unk50;
    c->unk6C = 0x1000;
    c->unk78 = 0x78000;
    c->unk80 = 0x1000;
    c->unk84 = 0x20000;
    c->unk8C = 0x1000;
    c->unk8E = 0xf555;
    c->unk92 = 0x1000;
    *(vu16 *)0x400000c = (*(vu16 *)0x400000c & 0x43) | 0x700;
    NNS_FndMountArchive(arc, data_ov000_023654a0, gZoneActArchive);
    Background_Init(buf, NNS_FndGetArchiveFileByIndex(arc, 6), 0x38, 0, 2, 0x20, 0x18);
    Background_Draw(buf);
    gDisplayMain.unk8 = 0;
    gDisplayMain.unkA = 0;
    GX_SetVisiblePlane(GX_GetVisiblePlane() | 4);
    NNS_FndUnmountArchive(arc);
    gDisplayMain.unk48 = 0x10;
    Sound_PlaySeqArc(0, 0, 0x7c);
    s->unk224 = 0;
    s->func = Boss3_State_DefeatFade_ov000_022d94fc;
}
