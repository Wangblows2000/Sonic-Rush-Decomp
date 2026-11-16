#include <nitro/types.h>

typedef struct UnkStruct_022dcb18_v {
    s32 x;
    s32 y;
    s32 z;
} UnkStruct_022dcb18_v;

typedef struct UnkStruct_022dcb18 UnkStruct_022dcb18;
struct UnkStruct_022dcb18 {
    u8 pad[0x10];
    s32 unk10;
    u8 pad0[0x54 - 0x14];
    u32 unk54;
    u8 pad1[0x254 - 0x58];
    UnkStruct_022dcb18_v unk254;
    u8 pad2[0x38c - 0x260];
    UnkStruct_022dcb18_v unk38C;
    u8 pad3[0x450 - 0x398];
    u8 unk450;
    u8 pad4[0x47c - 0x451];
    s32 unk47C;
    s32 unk480;
    u8 pad5[0x488 - 0x484];
    void (*unk488)(UnkStruct_022dcb18 *s);
    s32 unk48C;
};

typedef struct UnkTask_022dcb18 {
    u8 pad[0x10];
    UnkStruct_022dcb18 *unk10;
} UnkTask_022dcb18;

typedef struct UnkStruct_022dcb18_7 {
    u8 pad[4];
    s32 unk4;
} UnkStruct_022dcb18_7;

extern UnkTask_022dcb18 *gTaskCurrent;
extern UnkStruct_022dcb18_7 gBossStatus;

void Boss4_WrapArenaX(s32 *p);
void Boss4_Unk_ov000_022db7d4(void *a, u16 *ang, UnkStruct_022dcb18 *s);

void Boss4PartD0_State_Unk_ov000_022dcb18(void) {
    UnkStruct_022dcb18 *s = gTaskCurrent->unk10;
    u16 ang;
    if (gBossStatus.unk4 == 0x1a) {
        s->unk54 |= 8;
        s->unk54 |= 2;
        return;
    }
    s->unk47C++;
    s->unk48C++;
    if (s->unk488 != NULL)
        s->unk488(s);
    Boss4_WrapArenaX(&s->unk10);
    Boss4_Unk_ov000_022db7d4(&s->unk254, &ang, s);
    if (s->unk480 != 0) {
        if (s->unk450 & 2)
            s->unk480 = 0;
        else
            s->unk38C = s->unk254;
    }
    if (s->unk47C > 0x258) {
        s->unk54 |= 8;
        s->unk54 |= 2;
    }
}
