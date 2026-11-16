#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s32 unk4;
    s32 unk8;
    u8 padC[0x2C - 0xC];
    s32 unk2C;
    s32 unk30;
    u32 unk34;
    u8 pad38[0x64 - 0x38];
} UnkPlayer_02340e44;

typedef struct {
    UnkPlayer_02340e44 p0;
    u8 pad64[0xC8 - 0x64];
    u32 unkC8;
} UnkPlayers_02340e44;

typedef struct {
    u8 pad0[0x10];
    u16 unk10;
} UnkStruct_02340e44_abd8;

extern UnkPlayers_02340e44 data_ov000_0236aad8;
extern UnkPlayer_02340e44 data_ov000_0236ab3c;
extern UnkStruct_02340e44_abd8 data_ov000_0236abd8;

UnkPlayer_02340e44 *func_ov000_0233b62c(void);
UnkPlayer_02340e44 *func_ov000_0233b610(void);
s32 func_0200225c(s32 a, s32 b);

void func_ov000_02340e44(void) {
    UnkPlayer_02340e44 *p;
    UnkPlayer_02340e44 *q;

    if (data_ov000_0236aad8.unkC8 & 1) {
        p = func_ov000_0233b62c();
        q = func_ov000_0233b610();
        if (!(p->unk34 & 1)) {
            p->unk2C = p->unk4 / 14;
            p->unk30 = func_0200225c(p->unk8 * 0x30, (data_ov000_0236abd8.unk10 - 0xc0) << 8) >> 4;
        }
        if (q->unk34 & 1) {
            return;
        }
        q->unk2C = q->unk4 / 14;
        q->unk30 = (func_0200225c(p->unk8 * 0x30, (data_ov000_0236abd8.unk10 - 0xc0) << 8) >> 4) + 0x11000;
        return;
    }
    p = &data_ov000_0236aad8.p0;
    if (!(p->unk34 & 1)) {
        p->unk2C = p->unk4 / 14;
        p->unk30 = func_0200225c(p->unk8 * 0x30, (data_ov000_0236abd8.unk10 - 0xc0) << 8) >> 4;
    }
    p = &data_ov000_0236ab3c;
    if (p->unk34 & 1) {
        return;
    }
    p->unk2C = p->unk4 / 14;
    p->unk30 = func_0200225c(p->unk8 * 0x30, (data_ov000_0236abd8.unk10 - 0xc0) << 8) >> 4;
}
