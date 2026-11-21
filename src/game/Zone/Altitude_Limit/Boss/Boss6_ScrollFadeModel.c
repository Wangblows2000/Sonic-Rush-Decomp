#include <game/Stage/stageobj.h>

typedef struct {
    u8 pad0[0x48];
    s32 unk48;
    u8 pad4C[0x138 - 0x4C];
} UnkStruct_Sub138;

typedef struct {
    u8 pad0[0xC];
    u16 unkC;
    u16 unkE;
} UnkStruct_Res;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x210 - 0x20C];
    UnkStruct_Sub138 unk210[6];
    u8 pad960[0xD54 - 0x960];
    s32 unkD54[2];
    u16 unkD5C;
} UnkStruct_Boss;

extern void *data_ov000_02369b5c[];

UnkStruct_Res *NNS_G3dGetMdlSet(void *a);
void NNS_G3dMdlSetMdlAlphaAll(void *a, s32 b);

void Boss6_ScrollFadeModel(UnkStruct_Boss *w) {
    u16 i = w->unkD5C;
    UnkStruct_Sub138 *s = &w->unk210[i + 3];
    s32 d;
    UnkStruct_Res *r;

    s->unk48 += w->unkD54[i];
    if (w->unkD54[i] > 0) {
        if (s->unk48 > 0x1a0000) {
            s->unk48 = -0x23df10;
            w->unkD5C ^= 1;
        }
    } else if (s->unk48 < -0x23df10) {
        s->unk48 = 0x1a0000;
        w->unkD5C ^= 1;
    }
    d = s->unk48 - 0x1a0000;
    d += 0x1eef88;
    if (d < 0)
        d = -d;
    d = 0x1eef88 - d;
    if (d < 0x10df4c) {
        r = NNS_G3dGetMdlSet(data_ov000_02369b5c[i]);
        NNS_G3dMdlSetMdlAlphaAll((u8 *)r + *(s32 *)((u8 *)r + r->unkE + 0xC), ((d * 0x1f / 0x10df4c) & 0x1f) * 6 / 10);
    }
}
