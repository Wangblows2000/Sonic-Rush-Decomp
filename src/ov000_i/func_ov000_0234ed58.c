#include <nitro/types.h>

typedef struct {
    u8 pad0[0x34];
    u32 unk34;
    u8 pad38[0x64 - 0x38];
} UnkPlayer_0234ed58;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 pad4[0xc - 4];
    s32 unkC;
    u8 pad10[0x2e - 0x10];
    s16 unk2E;
    s16 unk30;
    u8 pad32[0x58 - 0x32];
    u32 unk58;
    u8 pad5C[0x68 - 0x5c];
    void *unk68;
    u8 pad6C[0x2bc - 0x6c];
    void *unk2BC;
    u8 pad2C0[0x32c - 0x2c0];
    u32 unk32C;
    u32 unk330;
    u8 pad334[0x3e8 - 0x334];
    void *unk3E8;
} UnkStruct_0234ed58;

extern UnkPlayer_0234ed58 gCamera[];
void Player_State_Glider(void);
void func_ov000_0234eccc(void);
void func_ov000_0234e984(void);
void BrightnessFade_Create(s32 a, s32 b);

void func_ov000_0234ed58(UnkStruct_0234ed58 *w, void *o) {
    if (w->unk68 != Player_State_Glider)
        return;
    w->unk58 |= 0x100;
    w->unk3E8 = o;
    BrightnessFade_Create(0x43, 0);
    BrightnessFade_Create(0x45, 0x2000);
    w->unkC = 0;
    w->unk2E = 0x1000;
    w->unk30 = 0;
    gCamera[0].unk34 |= 1;
    gCamera[1].unk34 |= 1;
    w->unk330 |= 0x200;
    w->unk330 |= 0x400;
    w->unk330 |= 0x30;
    w->unk330 |= 0xc0;
    w->unk32C |= 0x2000;
    w->unk3 = 0xc0;
    w->unk68 = func_ov000_0234eccc;
    w->unk2BC = func_ov000_0234e984;
}
