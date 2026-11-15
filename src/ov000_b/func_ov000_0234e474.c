#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_0234e474_Target;

typedef struct {
    u8 pad0[2];
    u8 unk2;
    u8 unk3;
    u8 pad4[4];
    s32 unk8;
    u8 padC[4];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    u8 pad28[0x330 - 0x28];
    u32 unk330;
    u8 pad334[0x3E8 - 0x334];
    UnkStruct_0234e474_Target *unk3E8;
    s16 unk3EC;
    u8 pad3EE[2];
    s32 unk3F0;
    s32 unk3F4;
    s32 unk3F8;
} UnkStruct_0234e474;

typedef struct {
    u8 pad0[0x34];
    u32 unk34;
    u8 pad38[0x98 - 0x38];
    u32 unk98;
} UnkStruct_0236aad8;

extern UnkStruct_0236aad8 data_ov000_0236aad8;

s32 func_02068bd8(s32 cur, s32 target, s32 start, s32 a, s32 b, s32 c);
void func_ov000_0234e3c4(UnkStruct_0234e474 *s);

void func_ov000_0234e474(UnkStruct_0234e474 *s) {
    s->unk1C = s->unk10;
    s->unk20 = s->unk14;
    s->unk24 = s->unk18;
    switch (s->unk8) {
    case 0:
        s->unk10 = func_02068bd8(s->unk10, s->unk3E8->unk10, s->unk3F0, 4, 0x800, 0x80);
        s->unk14 = func_02068bd8(s->unk14, s->unk3E8->unk14, s->unk3F4, 4, 0x800, 0x80);
        s->unk18 = func_02068bd8(s->unk18, -0x6000, s->unk3F8, 4, 0x800, 0x80);
        if (s->unk10 != s->unk3E8->unk10)
            return;
        if (s->unk14 != s->unk3E8->unk14)
            return;
        if (s->unk18 != -0x6000)
            return;
        s->unk8 = 1;
        s->unk3F0 = s->unk10;
        s->unk3F4 = s->unk14;
        s->unk3F8 = s->unk18;
        s->unk3EC = -0x80;
        s->unk330 &= ~0x40;
        data_ov000_0236aad8.unk34 &= ~1;
        data_ov000_0236aad8.unk98 &= ~1;
        break;
    case 1:
        s->unk10 = func_02068bd8(s->unk10, s->unk3F0 + 0x5000, s->unk3F0, 4, 0x200, 0x80);
        s->unk18 = func_02068bd8(s->unk18, 0x6000, s->unk3F8, 2, 0x800, 0x80);
        if (s->unk3 != 0)
            s->unk3++;
        if (s->unk10 != s->unk3F0 + 0x5000)
            return;
        if (s->unk18 != 0x6000)
            return;
        s->unk8 = 2;
        s->unk3F0 = s->unk10;
        s->unk3F4 = s->unk14;
        s->unk3F8 = s->unk18;
        s->unk2--;
        break;
    case 2:
        s->unk10 = func_02068bd8(s->unk10, s->unk3F0 + 0xe600, s->unk3F0, 4, 0x800, 0x200);
        if (s->unk2 > 0xc0)
            s->unk2--;
        if (s->unk10 != s->unk3F0 + 0xe600)
            return;
        s->unk3F0 = s->unk10;
        s->unk8 = 3;
        s->unk2 = 0;
        break;
    case 3:
        s->unk10 = func_02068bd8(s->unk10, s->unk3F0 - 0xb600, s->unk3F0, 4, 0x300, 0x80);
        s->unk18 = func_02068bd8(s->unk18, 0, s->unk3F8, 4, 0x200, 0x80);
        if (s->unk10 != s->unk3F0 - 0xb600)
            return;
        s->unk3E8 = NULL;
        func_ov000_0234e3c4(s);
        break;
    }
}
