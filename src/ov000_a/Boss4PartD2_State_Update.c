#include <nitro/types.h>

typedef struct UnkStruct_022dbfb4_a {
    u8 pad[0x24];
    u8 unk24[0x24];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 pad1[0x10c - 0x54];
    u8 unk10C;
    u8 pad2[0x138 - 0x10d];
} UnkStruct_022dbfb4_a;

typedef struct UnkStruct_022dbfb4 UnkStruct_022dbfb4;
struct UnkStruct_022dbfb4 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    u8 pad0[0x54 - 0x18];
    u32 unk54;
    u8 pad1[0x5c - 0x58];
    u16 unk5C;
    u8 pad2[0x20c - 0x5e];
    UnkStruct_022dbfb4_a unk20C;
    UnkStruct_022dbfb4_a unk344;
    s32 unk47C;
    u8 pad4[0x4d8 - 0x480];
    s32 unk4D8;
    s32 unk4DC;
    s32 unk4E0;
    s32 unk4E4;
    u16 unk4E8;
    u16 unk4EA;
    u16 unk4EC;
    u8 pad5[0x4f0 - 0x4ee];
    void (*unk4F0)(UnkStruct_022dbfb4 *s);
    s32 unk4F4;
};

typedef struct UnkTask_022dbfb4 {
    u8 pad[0x10];
    UnkStruct_022dbfb4 *unk10;
} UnkTask_022dbfb4;

extern UnkTask_022dbfb4 *gTaskCurrent;

typedef struct UnkStruct_022dbfb4_q {
    u8 pad[0x10];
    s32 unk10;
} UnkStruct_022dbfb4_q;

typedef struct UnkStruct_022dbfb4_w {
    u8 pad[0x20c];
    UnkStruct_022dbfb4_q *unk20C;
} UnkStruct_022dbfb4_w;

typedef struct UnkStruct_022dbfb4_7 {
    u8 pad[4];
    s32 unk4;
} UnkStruct_022dbfb4_7;

extern UnkStruct_022dbfb4_7 gBossStatus;
extern UnkStruct_022dbfb4_w *gBoss4;
extern s16 data_0206e470[];
extern s16 data_02070470[];

void Boss4_WrapArenaX(s32 *p);
void Boss4_ArenaPosToWorld(s32 *v, u16 *ang, s32 x, s32 y);

void Boss4PartD2_State_Update(void) {
    UnkStruct_022dbfb4 *s = gTaskCurrent->unk10;
    if (gBossStatus.unk4 == 0x1a) {
        s->unk54 |= 8;
        s->unk54 |= 2;
        return;
    }
    s->unk47C++;
    s->unk4F4++;
    if (s->unk4F0 != NULL)
        s->unk4F0(s);
    Boss4_WrapArenaX(&s->unk4E0);
    Boss4_WrapArenaX(&s->unk4E4);
    Boss4_ArenaPosToWorld(NULL, &s->unk4EA, s->unk4E0, 0);
    Boss4_ArenaPosToWorld(NULL, &s->unk4EC, s->unk4E4, 0);
    if (s->unk4D8 == 0) {
        u16 d = s->unk4EA - s->unk4E8;
        if (data_0206e470[((d & 0xffff) >> 4) * 2] < data_02070470[0x70]) {
            s->unk4D8 = 1;
            Boss4_ArenaPosToWorld(&s->unk344.unk48, NULL, gBoss4->unk20C->unk10, s->unk14);
        }
    } else if (s->unk4D8 != 0 && (s->unk344.unk10C & 2)) {
        s->unk54 |= 8;
        s->unk54 |= 2;
    }
    if (s->unk47C > 0x258) {
        s->unk54 |= 8;
        s->unk54 |= 2;
    }
}
