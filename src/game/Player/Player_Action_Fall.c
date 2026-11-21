#include <nitro/types.h>

typedef struct UnkStruct_0234ab34 UnkStruct_0234ab34;
struct UnkStruct_0234ab34 {
    u8 unk0;
    u8 unk1;
    u8 pad2[8 - 2];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    u8 pad18[0x2E - 0x18];
    s16 unk2E;
    s16 unk30;
    u8 pad32[0x34 - 0x32];
    s16 unk34;
    u8 pad36[0x58 - 0x36];
    u32 unk58;
    u16 unk5C;
    u16 unk5E;
    u16 unk60;
    u8 pad62[0x68 - 0x62];
    void (*state)(UnkStruct_0234ab34 *w);
    u8 pad6C[0x2CC - 0x6C];
    u8 unk2CC[0x324 - 0x2CC];
    u8 unk324;
    u8 pad325[0x32A - 0x325];
    s16 unk32A;
    u32 unk32C;
    u8 pad330[0x384 - 0x330];
    u8 unk384[0x3E8 - 0x384];
    UnkStruct_0234ab34 *other;
    u8 pad3EC[0x3F0 - 0x3EC];
    s32 unk3F0;
    s32 unk3F4;
    s32 unk3F8;
};

extern s16 data_0206e470[];

#define IDX_0234ab34(a) ((s32)((a) & 0xffff) >> 4)
#define SIN_0234ab34(a) data_0206e470[(u32)(IDX_0234ab34(a) << 1)]
#define COS_0234ab34(a) data_0206e470[(u32)((IDX_0234ab34(a) << 1) + 1)]

void Player_State_InAir(UnkStruct_0234ab34 *w);
void Player_SetAnimation(UnkStruct_0234ab34 *w, s32 a);
void ObjRect_SetAtkDefFlags(void *p, s32 a, s32 b);

void Player_Action_Fall(UnkStruct_0234ab34 *w) {
    Player_SetAnimation(w, 2);
    w->unk5C |= 4;
    w->unk58 |= 0x10;
    w->unk58 |= 0x8000;
    w->state = Player_State_InAir;
    w->unk2E = (w->unk34 * COS_0234ab34((u16)(w->unk1 << 8))) >> 12;
    w->unk30 = (w->unk34 * SIN_0234ab34((u16)(w->unk1 << 8))) >> 12;
    ObjRect_SetAtkDefFlags(w->unk2CC, 0, 0);
    w->unk32C &= ~0xF;
    w->unk32C |= 1;
    if (((w->unk5E | w->unk60) & 1) && w->unk30 < -0x80)
        w->unk32C |= 1;
    w->unkC = 0;
    w->unk8 = 0;
    w->unk32A = 0;
}
