#include <nitro/types.h>

typedef struct UnkStruct_022da4b4_p {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    u8 pad1[0x58 - 0x18];
    u32 unk58;
    u16 unk5C;
    u8 pad2[0x158 - 0x5e];
    u8 unk158[0x24];
    s32 unk17C;
    s32 unk180;
    s32 unk184;
    u8 pad3[0x32c - 0x188];
    u32 unk32C;
    u8 pad4[0x420 - 0x330];
    u16 unk420;
} UnkStruct_022da4b4_p;

typedef struct UnkStruct_022da4b4_r {
    u8 pad[0x10];
    s32 unk10;
    u8 pad1[0x2e - 0x14];
    s16 unk2E;
} UnkStruct_022da4b4_r;

typedef struct UnkStruct_022da4b4_b {
    u8 pad[0x20c];
    u32 unk20C;
    u8 pad1[0x214 - 0x210];
    UnkStruct_022da4b4_r *unk214;
    UnkStruct_022da4b4_r *unk218;
} UnkStruct_022da4b4_b;

typedef struct UnkStruct_022da4b4_c {
    u8 pad[0xd6];
    u16 unkD6;
} UnkStruct_022da4b4_c;

extern UnkStruct_022da4b4_p *gPlayers;
extern UnkStruct_022da4b4_b *gBoss3;
extern UnkStruct_022da4b4_c data_ov000_0236a444;
extern s16 data_0206e470[];

void BossArena_ArcToCirclePos(s32 len, s32 b, s32 a, s32 *outX, s32 *outY, u16 *outAng);
void MTX_RotY33_(void *m, s32 s, s32 c);
s32 Boss3_Unk_ov000_022da7c8(s32 a, s32 b, s32 c);

void Boss3_Draw_Unk_ov000_022da4b4(void) {
    UnkStruct_022da4b4_p *p = gPlayers;
    u16 ang;
    BOOL f;
    if (p->unk5C & 0x20)
        return;
    if (p->unk10 < 0x4000)
        p->unk10 += 0x5fe43;
    else if (p->unk10 > 0x63e43)
        p->unk10 -= 0x5fe43;
    BossArena_ArcToCirclePos(0xf42f8, 0x5fe435, p->unk10 << 4, &p->unk17C, &p->unk184, &ang);
    p->unk180 = -p->unk14 << 4;
    {
        u16 r;
        if (p->unk5C & 1)
            r = 0xc000;
        else
            r = 0x4000;
        r += ang;
        MTX_RotY33_(p->unk158, data_0206e470[((r & 0xffff) >> 4) * 2], data_0206e470[((r & 0xffff) >> 4) * 2 + 1]);
    }
    data_ov000_0236a444.unkD6 = ang;
    if (!(gBoss3->unk20C & 4))
        return;
    if (gBoss3->unk218 != NULL) {
        f = FALSE;
        p->unk420 = 0;
        if (!(p->unk58 & 1))
            return;
        if (gBoss3->unk218->unk2E <= 0)
            f = TRUE;
        if (Boss3_Unk_ov000_022da7c8(gBoss3->unk214->unk10, p->unk10, f) < 0x1f400) {
            if (f)
                p->unk420 |= 0x20;
            else
                p->unk420 |= 0x10;
            return;
        }
        p->unk32C &= ~0x200000;
        gBoss3->unk20C &= ~4;
        return;
    }
    p->unk32C &= ~0x200000;
    gBoss3->unk20C &= ~4;
}
