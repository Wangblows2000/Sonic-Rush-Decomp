#include <game/Stage/stageobj.h>

typedef struct {
    u8 pad0[0x20];
    union {
        u16 raw;
        struct {
            u16 b0 : 1;
            u16 b1 : 5;
            u16 b6 : 2;
            u16 b8 : 8;
        } bits;
    } unk20;
    u8 pad22[2];
    struct {
        u16 b0 : 5;
        u16 b5 : 11;
    } unk24;
} UnkStruct_0208922c;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x230 - 0x20C];
    u16 unk230;
    u16 unk232;
    s32 unk234;
    u8 pad238[0x23C - 0x238];
    void *unk23C;
    u8 pad240[0x248 - 0x240];
    void *unk248;
} UnkStruct_022ed78c;

extern UnkStruct_0208922c gDisplayMain;

u8 *NNS_G3dGetMdlSet(void *p);
void NNSi_G3dModifyMatFlag(void *m, s32 a, s32 b);
void NNS_G3dGlbMaterialColorDiffAmb(s32 a, s32 b, s32 c);
void Boss7_State_ClashFlash(void);

void Boss7_State_ClashStartFlash(UnkStruct_022ed78c *s) {
    u8 *r;

    s->unk230 = 0;
    s->unk232 = 0xc00;
    gDisplayMain.unk20.raw = 0;
    gDisplayMain.unk20.bits.b0 = 1;
    gDisplayMain.unk20.bits.b6 = 2;
    gDisplayMain.unk24.b0 = (u16)(s->unk232 >> 8);
    s->unk234 = 0x1f00;
    r = NNS_G3dGetMdlSet(s->unk23C);
    NNSi_G3dModifyMatFlag(r + *(u32 *)(r + *(u16 *)(r + 0xe) + 0xc), 0, 0x40);
    r = NNS_G3dGetMdlSet(s->unk23C);
    NNSi_G3dModifyMatFlag(r + *(u32 *)(r + *(u16 *)(r + 0xe) + 0xc), 0, 0x80);
    r = NNS_G3dGetMdlSet(s->unk248);
    NNSi_G3dModifyMatFlag(r + *(u32 *)(r + *(u16 *)(r + 0xe) + 0xc), 0, 0x40);
    r = NNS_G3dGetMdlSet(s->unk248);
    NNSi_G3dModifyMatFlag(r + *(u32 *)(r + *(u16 *)(r + 0xe) + 0xc), 0, 0x80);
    NNS_G3dGlbMaterialColorDiffAmb(0x7fff, 0x7fff, 0);
    s->base.unk68 = Boss7_State_ClashFlash;
}
