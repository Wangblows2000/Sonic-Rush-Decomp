#include <nitro/types.h>

typedef struct UnkStruct_0234a878 UnkStruct_0234a878;
struct UnkStruct_0234a878 {
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
    void (*state)(UnkStruct_0234a878 *w);
    u8 pad6C[0x2CC - 0x6C];
    u8 unk2CC[0x324 - 0x2CC];
    u8 unk324;
    u8 pad325[0x32A - 0x325];
    s16 unk32A;
    u32 unk32C;
    u8 pad330[0x352 - 0x330];
    s16 unk352;
    u8 pad354[0x370 - 0x354];
    s16 unk370;
    u8 pad372[0x384 - 0x372];
    u8 unk384[0x3E8 - 0x384];
    UnkStruct_0234a878 *other;
    u8 pad3EC[0x3F0 - 0x3EC];
    s32 unk3F0;
    s32 unk3F4;
    s32 unk3F8;
};

extern s16 data_0206e470[];

#define IDX_0234a878(a) ((s32)((a) & 0xffff) >> 4)
#define SIN_0234a878(a) data_0206e470[(u32)(IDX_0234a878(a) << 1)]
#define COS_0234a878(a) data_0206e470[(u32)((IDX_0234a878(a) << 1) + 1)]

void func_ov000_0234a288(UnkStruct_0234a878 *w);
void Player_SetAnimation(UnkStruct_0234a878 *w, s32 a);
void Sound_PlaySeqArcEx(void *p, s32 a, s32 b);

void func_ov000_0234a878(UnkStruct_0234a878 *w) {
    u8 a;

    Player_SetAnimation(w, 0x18);
    w->unk5C |= 4;
    w->unk58 |= 0x10;
    w->unk58 |= 0x8000;
    w->state = func_ov000_0234a288;
    if (w->unk5C & 1) a = 0xC0; else a = 0x40;
    w->unk30 = 0;
    w->unk2E += (s16)((w->unk370 * SIN_0234a878((u16)(a << 8))) >> 12);
    w->unk30 += (s16)((-w->unk370 * COS_0234a878((u16)(a << 8))) >> 12);
    w->unk32C &= ~0xF;
    w->unk32C |= 1;
    w->unkC = 0;
    w->unk8 = 0;
    w->unk32A = 0;
    w->unk352 = 0xC;
    Sound_PlaySeqArcEx(w->unk384, 0, 8);
}
