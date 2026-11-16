#include <nitro/types.h>

typedef struct UnkStruct_022c5fe4_2 {
    u8 pad[0x228];
    s16 unk228;
} UnkStruct_022c5fe4_2;

typedef struct UnkStruct_022c5fe4 UnkStruct_022c5fe4;
struct UnkStruct_022c5fe4 {
    u8 pad[0x10];
    s32 unk10;
    u8 pad0[0x2e - 0x14];
    u16 unk2E;
    u8 pad1[0x68 - 0x30];
    void (*func)(UnkStruct_022c5fe4 *s);
    u8 pad2[0x2bc - 0x6c];
    u16 unk2BC;
    u8 pad3[2];
    u16 unk2C0;
    u16 unk2C2;
    u8 pad4[0x2cc - 0x2c4];
    u16 unk2CC;
    s16 unk2CE;
};

extern UnkStruct_022c5fe4_2 *gBoss1;

void func_ov000_022c6e58(UnkStruct_022c5fe4 *s);
void func_ov000_022c7158(UnkStruct_022c5fe4 *s);
void Boss1Hammer_State_Unk_ov000_022c66a4(UnkStruct_022c5fe4 *s);

void Boss1Hammer_State_Unk_ov000_022c5fe4(UnkStruct_022c5fe4 *s) {
    s32 v;
    s32 x;
    s32 n;
    if (gBoss1->unk228 > 0)
        s->unk2CE -= 0x80;
    else
        s->unk2CE += 0x80;
    v = s->unk2CE;
    if (v < -0x3800)
        v = -0x3800;
    else if (v > 0x3800)
        v = 0x3800;
    s->unk2CE = v;
    s->unk2CC += s->unk2CE;
    func_ov000_022c6e58(s);
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
    if ((s16)s->unk2C0 >= 0x1000) {
        if (gBoss1->unk228 > 0)
            gBoss1->unk228--;
        else
            gBoss1->unk228++;
        gBoss1->unk228 = -gBoss1->unk228;
        s->unk2BC++;
        s->func = Boss1Hammer_State_Unk_ov000_022c66a4;
    }
    func_ov000_022c7158(s);
}
