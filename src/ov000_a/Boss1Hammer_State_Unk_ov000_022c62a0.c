#include <nitro/types.h>

typedef struct UnkStruct_022c62a0_3 {
    u8 pad[0x10];
    s32 unk10;
} UnkStruct_022c62a0_3;

typedef struct UnkStruct_022c62a0_2 {
    u8 pad[0x210];
    UnkStruct_022c62a0_3 *unk210;
    u8 pad1[0x228 - 0x214];
    s16 unk228;
} UnkStruct_022c62a0_2;

typedef struct UnkStruct_022c62a0 UnkStruct_022c62a0;
struct UnkStruct_022c62a0 {
    u8 pad[0x10];
    s32 unk10;
    u8 pad0[0x2e - 0x14];
    u16 unk2E;
    u8 pad1[0x68 - 0x30];
    void (*func)(UnkStruct_022c62a0 *s);
    u8 pad2[0x2bc - 0x6c];
    u16 unk2BC;
    u8 pad3[2];
    u16 unk2C0;
    u16 unk2C2;
    u8 pad4[0x2cc - 0x2c4];
    u16 unk2CC;
    s16 unk2CE;
};

extern UnkStruct_022c62a0_2 *gBoss1;

void Boss1Hammer_Unk_ov000_022c6e58(UnkStruct_022c62a0 *s);
void Boss1Hammer_Unk_ov000_022c7158(UnkStruct_022c62a0 *s);
void Boss1Hammer_State_Unk_ov000_022c5fbc(UnkStruct_022c62a0 *s);

void Boss1Hammer_State_Unk_ov000_022c62a0(UnkStruct_022c62a0 *s) {
    BOOL pos;
    s32 v;
    s32 x;
    s32 n;
    gBoss1->unk210->unk10 = s->unk10;
    s->unk2C0 += s->unk2C2;
    if ((s16)s->unk2C0 > 0x1000)
        s->unk2C0 = 0x1000;
    if (gBoss1->unk228 > 0) {
        s32 t = (s16)s->unk2C0;
        n = 2;
        x = 0x90000;
        do {
            x += (s32)(((s64)(0x18000 - x) * t + 0x800) >> 12);
        } while (n-- != 0);
        s->unk2E = (x >> 4) - s->unk10;
    } else {
        s32 t2 = (s16)s->unk2C0;
        s32 x2 = 0x132000;
        s32 n2 = 2;
        do {
            x2 += (s32)(((s64)(0x1aa000 - x2) * t2 + 0x800) >> 12);
        } while (n2-- != 0);
        s->unk2E = (x2 >> 4) - s->unk10;
    }
    Boss1Hammer_Unk_ov000_022c6e58(s);
    pos = TRUE;
    if (s->unk2CE < 0) {
        s->unk2CE = -s->unk2CE;
        pos = FALSE;
    }
    v = s->unk2CE;
    if (v > 0) {
        if (s->unk2CC >= 0xfc00 && v <= 0x400) {
            s->unk2CE = 0;
            s->unk2CC = 0;
        } else if (v > 0) {
            s->unk2CE -= 0xa0;
            if (s->unk2CE < 0x400)
                s->unk2CE = 0x400;
        }
    }
    if (!pos)
        s->unk2CE = -s->unk2CE;
    s->unk2CC += s->unk2CE;
    if (s->unk2CE == 0 && (s16)s->unk2C0 >= 0x1000) {
        s->unk2E = 0;
        s->func = Boss1Hammer_State_Unk_ov000_022c5fbc;
    }
    Boss1Hammer_Unk_ov000_022c7158(s);
}
