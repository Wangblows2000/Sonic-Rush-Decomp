#include <nitro/types.h>

typedef struct UnkStruct_022dc568 UnkStruct_022dc568;
struct UnkStruct_022dc568 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    u8 pad0[0x54 - 0x18];
    u32 unk54;
    u8 pad1[0x230 - 0x58];
    u8 unk230[0x24];
    u8 unk254[0x344 - 0x254];
    s32 unk344;
    s32 unk348;
    u16 unk34C;
    u8 pad2[0x3a8 - 0x34e];
    s32 unk3A8;
    s32 unk3AC;
    s32 unk3B0;
    s32 unk3B4;
    s32 unk3B8;
    s32 unk3BC;
    void (*unk3C0)(UnkStruct_022dc568 *s);
    s32 unk3C4;
};

typedef struct UnkTask_022dc568 {
    u8 pad[0x10];
    UnkStruct_022dc568 *unk10;
} UnkTask_022dc568;

typedef struct UnkStruct_022dc568_7 {
    u8 pad[4];
    s32 unk4;
} UnkStruct_022dc568_7;

extern UnkTask_022dc568 *gTaskCurrent;
extern UnkStruct_022dc568_7 gBossStatus;
extern s16 data_0206e470[];

void Boss4_WrapArenaX(s32 *p);
void func_ov000_022db7d4(void *a, u16 *ang, UnkStruct_022dc568 *s);
void MTX_RotY33_(void *m, s32 s, s32 c);

void Boss4PartD1_State_Unk_ov000_022dc568(void) {
    UnkStruct_022dc568 *s = gTaskCurrent->unk10;
    u16 ang;
    if (gBossStatus.unk4 == 0x1a) {
        s->unk54 |= 8;
        s->unk54 |= 2;
        return;
    }
    s->unk344++;
    s->unk3C4++;
    s->unk3A8 += s->unk3AC;
    if (s->unk3A8 > s->unk3B0)
        s->unk3A8 = s->unk3B0;
    s->unk3B4 += s->unk3B8;
    if (s->unk3B4 > s->unk3BC)
        s->unk3B4 = s->unk3BC;
    if (s->unk348)
        s->unk10 += s->unk3A8;
    else
        s->unk10 -= s->unk3A8;
    s->unk14 += s->unk3B4;
    if (s->unk3C0 != NULL)
        s->unk3C0(s);
    Boss4_WrapArenaX(&s->unk10);
    func_ov000_022db7d4(s->unk254, &ang, s);
    {
        u16 r = s->unk34C + ang;
        MTX_RotY33_(s->unk230, data_0206e470[((r & 0xffff) >> 4) * 2], data_0206e470[((r & 0xffff) >> 4) * 2 + 1]);
    }
}
