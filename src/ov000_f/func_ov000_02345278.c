#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    u32 unk4;
    u8 pad8[0x114 - 8];
    s32 unk114;
} UnkStruct_02345278_Spr;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 pad4[0x10 - 4];
    s32 x;
    s32 y;
    s32 z;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s16 unk28;
    s16 unk2A;
    u8 pad2C[0x2E - 0x2C];
    s16 unk2E;
    u8 pad30[0x34 - 0x30];
    s16 unk34;
    u8 pad36[0x58 - 0x36];
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x62 - 0x5E];
    s16 unk62;
    u8 pad64[0xBE - 0x64];
    s16 unkBE;
    u8 padC0[0x106 - 0xC0];
    s16 unk106;
    u8 pad108[0x124 - 0x108];
    UnkStruct_02345278_Spr *unk124;
    u8 pad128[0x2A0 - 0x128];
    u8 unk2A0[0x325 - 0x2A0];
    u8 unk325;
    u8 unk326;
    u8 unk327;
    s16 unk328;
    u8 pad32A[0x32C - 0x32A];
    u32 unk32C;
    u32 unk330;
    u8 pad334[0x384 - 0x334];
    u8 unk384[0x3BE - 0x384];
    s16 unk3BE;
} UnkStruct_02345278;

typedef struct {
    s32 x;
    s32 y;
    s32 z;
    u16 unkC;
    s16 unkE;
    u8 unk10;
    u8 unk11;
    u8 unk12;
    u8 unk13;
    u32 unk14;
    u32 unk18;
    u32 unk1C;
    s16 unk20;
    s16 unk22;
    u8 pad24[0x2C - 0x24];
    u32 unk2C;
} UnkStruct_02345278_Frame;

typedef struct {
    u8 pad0[0x18];
    u32 unk18;
    u8 pad1C[0x20 - 0x1C];
    u32 unk20;
} UnkStruct_02345278_4560;

typedef struct {
    u8 pad0[0x40];
    s32 unk40;
} UnkStruct_02345278_4590;

typedef struct {
    u32 flags;
    s32 x;
    s32 y;
} UnkStruct_02345278_Cam;

extern UnkStruct_02345278_4560 data_022c4560;
extern UnkStruct_02345278_4590 data_022c4590;
extern UnkStruct_02345278_Cam data_ov000_0236aad8;
extern UnkStruct_02345278 *volatile gPlayers[];

void func_0206c95c(void);
UnkStruct_02345278_Frame *func_0206c938(s32 a, u8 b);
void func_ov000_0233b1b8(u8 a);
void func_ov000_0233b198(u8 a);
void Player_SetAnimation(UnkStruct_02345278 *w, u8 state);
s32 ObjRect_HitCenterX(void *a, void *b);
s32 ObjRect_HitCenterY(void *a, void *b);
void func_ov000_0232ff6c(s32 a, s32 b);
void Sound_PlaySeqArcEx(void *p, s32 a, s32 b);

void func_ov000_02345278(UnkStruct_02345278 *w) {
    UnkStruct_02345278_Frame *f;
    UnkStruct_02345278 *p;
    s32 a;

    if (!(data_022c4560.unk18 & 0x20)) {
        return;
    }
    if (w->unk325 == 0) {
        return;
    }
    func_0206c95c();
    f = func_0206c938(1, w->unk326);
    if (f == NULL) {
        return;
    }
    w->unk1C = w->x;
    w->unk20 = w->y;
    w->unk24 = w->z;
    w->x = f->x;
    w->y = f->y;
    w->z = f->z;
    func_ov000_0233b1b8(w->unk327);
    func_ov000_0233b198(w->unk327);
    if (w->unk328 != f->unk10) {
        w->unk5C &= ~0x400;
        Player_SetAnimation(w, f->unk10);
    }
    w->unkBE = f->unkE;
    w->unk106 = f->unkE;
    w->unk124->unk114 = (f->unkE << 12) >> 4;
    w->unk5C &= 0x1000;
    w->unk5C |= f->unkC & ~0x1040;
    w->unk5C ^= 0x1000;
    w->unkBE = f->unkE << 1;
    w->unk106 = f->unkE << 1;
    w->unk124->unk114 = ((f->unkE << 12) >> 4) << 1;
    w->unk1 = f->unk11;
    w->unk2 = f->unk12;
    w->unk3 = f->unk13;
    w->unk58 &= ~0xf;
    w->unk58 = f->unk14 & 0xf;
    w->unk32C = f->unk18;
    w->unk32C &= ~0x80000;
    if (f->unk14 & 0x2100) {
        w->unk32C |= 0x80000;
    }
    w->unk330 = f->unk1C;
    w->unk28 = f->unk20;
    w->unk2A = f->unk22;
    if ((w->unk32C & 0x1000000) && data_022c4590.unk40 != 3 && data_022c4560.unk20 > f->unk2C) {
        data_022c4560.unk20 = f->unk2C;
    }
    w->unk58 |= 0x2100;
    if (w->unk32C & 0x20000) {
        gPlayers[0]->unk3BE = 0xc;
        if (f->unk20 < 0) {
            p = gPlayers[0];
            if (p->unk58 & 0x10) {
                if (p->unk2E > -0x480) {
                    p->unk2E = -0x480;
                }
            } else {
                if (p->unk34 > -0x480) {
                    p->unk34 = -0x480;
                }
            }
        } else {
            p = gPlayers[0];
            if (p->unk58 & 0x10) {
                if (p->unk2E < 0x480) {
                    p->unk2E = 0x480;
                }
            } else {
                if (p->unk34 < 0x480) {
                    p->unk34 = 0x480;
                }
            }
        }
        gPlayers[0]->unk62 = 0x44;
        a = ObjRect_HitCenterX(w->unk2A0, gPlayers[0]->unk2A0);
        func_ov000_0232ff6c(a, ObjRect_HitCenterY(w->unk2A0, gPlayers[0]->unk2A0));
        Sound_PlaySeqArcEx(w->unk384, 0, 0x40);
    }
    if (data_ov000_0236aad8.x - 0x2000 < w->x && data_ov000_0236aad8.x + 0x12000 > w->x &&
        data_ov000_0236aad8.y - 0x2000 < w->y && data_ov000_0236aad8.y + 0x10000 > w->y) {
        w->unk124->unk4 &= ~1;
        return;
    }
    w->unk124->unk4 |= 1;
}
