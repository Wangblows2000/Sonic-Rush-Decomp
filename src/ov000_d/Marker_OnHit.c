#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    u16 unk4;
} UnkStruct_023102b4_Prm;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
    u8 pad18[0x54 - 0x18];
    u32 unk54;
    u8 pad58[4];
    u16 unk5C;
    u8 pad5E[0x1E0 - 0x5E];
    UnkStruct_023102b4_Prm *unk1E0;
} UnkStruct_023102b4_Obj;

typedef struct {
    u8 pad0[0x28];
    s16 unk28;
    u8 pad2A[0x67 - 0x2A];
    s8 unk67;
    u8 pad68[0x327 - 0x68];
    u8 unk327;
    u8 pad328[0x32C - 0x328];
    u32 unk32C;
    u8 pad330[0x37C - 0x330];
    s16 unk37C;
} UnkStruct_023102b4_Ply;

typedef struct {
    u8 pad0[0x18];
    void *unk18;
} UnkStruct_023102b4;

typedef struct {
    u8 pad0[8];
    s32 unk8;
    s32 unkC;
    u8 pad10[0x15 - 0x10];
    u8 unk15;
    u8 pad16[2];
    u32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad24[0x38 - 0x24];
    u16 unk38;
} UnkStruct_023102b4_4590;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    u8 pad14[0x20 - 0x14];
    u32 unk20;
} UnkStruct_023102b4_4560;

typedef struct {
    u16 unk0;
    u8 pad2[0x62];
} UnkStruct_023102b4_Cam;

extern UnkStruct_023102b4_4590 data_022c4590;
extern UnkStruct_023102b4_4560 gGameState;
extern UnkStruct_023102b4_Cam data_ov000_0236ab3a[];

void StageObj_SetAnimation(void *w, s32 a);
void Player_AddTension(UnkStruct_023102b4_Ply *w, s16 a);
void Sound_PlaySeqArcEx(void *p, s32 a, s32 b);
void CheckpointTimePopup_Create(UnkStruct_023102b4_Ply *w, u32 a);

void Marker_OnHit(UnkStruct_023102b4 *a, UnkStruct_023102b4 *b) {
    UnkStruct_023102b4_Obj *o;
    UnkStruct_023102b4_4590 *g;
    s8 h;
    UnkStruct_023102b4_Ply *p;

    p = b->unk18;
    o = a->unk18;
    g = &data_022c4590;

    if (p != NULL) {
        h = p->unk67 + 1;
        StageObj_SetAnimation(o, 1);
        g->unk1C = o->unk10 >> 8;
        g->unk20 = (o->unk14 >> 8) - h;
        if (o->unk5C & 2) {
            g->unk20 = h + (o->unk14 >> 8);
        }
        g->unk15 = o->unk1E0->unk4;
        g->unk38 = data_ov000_0236ab3a[p->unk327].unk0;
        g->unk18 = gGameState.unk20;
        Player_AddTension(p, 800);
        if (p->unk32C & 0x80) {
            data_022c4590.unk8 += 10000;
        } else if (p->unk32C & 0x100) {
            data_022c4590.unk8 += 8000;
        } else {
            s32 v = p->unk28;
            if (v < 0) {
                v = -v;
            }
            if (p->unk37C <= v) {
                data_022c4590.unk8 += 4000;
            }
        }
        data_022c4590.unkC++;
        Sound_PlaySeqArcEx(NULL, 0, 0x44);
        if (gGameState.unk10 == 2) {
            CheckpointTimePopup_Create(p, gGameState.unk20);
        }
    }
    o->unk54 |= 2;
}
