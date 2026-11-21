#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    s32 unk8;
    s32 unkc;
} UnkCfg_022cc044;

typedef struct {
    u32 type;
    u8 pad4[0x2c];
} UnkItem_022cc044;

typedef struct {
    u8 pad0[4];
    s32 cur;
    u8 pad8[0x1c - 8];
    s32 unk1c;
    u8 pad20[0x2c - 0x20];
    s32 unk2c;
    s32 unk30;
    u8 pad34[4];
    s32 unk38;
    s32 unk3c;
    s32 unk40;
    s32 unk44;
    u8 pad48[4];
    s32 unk4c;
    s32 unk50;
    s32 unk54;
    s32 unk58;
    u8 pad5c[0x1e0 - 0x5c];
    s32 unk1e0;
    u16 unk1e4;
    u16 unk1e6;
    u8 pad1e8[4];
    u8 unk1ec[0x1c];
    u8 pad208[0x28c - 0x208];
    UnkItem_022cc044 items[6];
    UnkCfg_022cc044 *cfg;
} UnkStruct_022cc044;

void MIi_CpuClear16(u16 data, void *dest, u32 size);
void SoundTest_BuildTrackNameText(void *p, s32 a, s32 b);

#pragma thumb on
void Options_ResetItemPageState(UnkStruct_022cc044 *p) {
    vu16 zero;

    switch (p->items[p->cur].type) {
    case 0:
        p->unk2c = 0;
        p->unk30 = 0;
        p->unk1c = -1;
        break;
    case 1:
        p->unk2c = 0;
        switch (p->cfg->unk8) {
        case 0:
            p->unk30 = 0;
            break;
        case 1:
            p->unk30 = 1;
            break;
        }
        p->unk1c = -1;
        break;
    case 2:
        p->unk2c = 0;
        if (p->cfg->unkc != 0) {
            p->unk30 = 0;
        } else {
            p->unk30 = 1;
        }
        p->unk1c = -1;
        break;
    case 3:
        p->unk2c = 0;
        p->unk30 = 1;
        p->unk1c = -1;
        p->unk38 = 0;
        p->unk3c = 0;
        p->unk40 = 0;
        p->unk44 = 0;
        p->unk1e0 = 0;
        p->unk1e4 = 0;
        p->unk1e6 = 0;
        p->unk54 = 0;
        p->unk58 = 5;
        SoundTest_BuildTrackNameText(&p->unk38, p->unk4c, p->unk50);
        break;
    case 4:
        p->unk2c = 0;
        p->unk30 = 1;
        p->unk1c = -1;
        break;
    case 5:
        p->unk30 = -1;
        p->unk1c = -1;
        zero = 0;
        MIi_CpuClear16(zero, p->unk1ec, sizeof(p->unk1ec));
        break;
    }
    if (p->items[p->cur].type == 3) {
        *(vu32 *)0x4001000 = (*(vu32 *)0x4001000 & ~0x1f00) | 0x1a00;
    } else {
        *(vu32 *)0x4001000 = (*(vu32 *)0x4001000 & ~0x1f00) | 0x1900;
    }
}
#pragma thumb reset
