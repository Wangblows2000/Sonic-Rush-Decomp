#include <nitro/types.h>

typedef struct UnkStruct_022c66a4 UnkStruct_022c66a4;

typedef struct UnkStruct_022c66a4_2 {
    u8 pad[0x210];
    UnkStruct_022c66a4 *unk210;
    u8 pad1[0x228 - 0x214];
    s16 unk228;
} UnkStruct_022c66a4_2;

struct UnkStruct_022c66a4 {
    u8 pad[0x10];
    s32 unk10;
    u8 pad0[0x2e - 0x14];
    u16 unk2E;
    u8 pad1[0x68 - 0x30];
    void (*func)(UnkStruct_022c66a4 *s);
    u8 pad2[0x2bc - 0x6c];
    u16 unk2BC;
    u8 pad3[2];
    u16 unk2C0;
    u16 unk2C2;
    u8 pad4[0x2cc - 0x2c4];
    u16 unk2CC;
    s16 unk2CE;
};

extern UnkStruct_022c66a4_2 *gBoss1;

void Boss1Hammer_SpawnDebris(UnkStruct_022c66a4 *s);
void Boss1Hammer_ApplySpinRotation(UnkStruct_022c66a4 *s);
void Sound_PlayVoice(s32 a, s32 b, s32 c);
void Boss1Hammer_State_LaunchRoll(UnkStruct_022c66a4 *s);

void Boss1Hammer_State_SpinUp(UnkStruct_022c66a4 *s) {
    u16 n;
    s16 k;
    s32 v;
    n = (s->unk2BC > 4) ? 4 : s->unk2BC;
    k = ((n * 0x80) >> 2) + 0x80;
    Boss1Hammer_SpawnDebris(s);
    if (gBoss1->unk228 > 0)
        s->unk2CE += k;
    else
        s->unk2CE -= k;
    v = s->unk2CE;
    if (v < -0x3800)
        v = -0x3800;
    else if (v > 0x3800)
        v = 0x3800;
    s->unk2CE = v;
    s->unk2CC += s->unk2CE;
    Boss1Hammer_ApplySpinRotation(s);
    v = s->unk2CE;
    if (v < 0)
        v = -v;
    if (v != 0x3800)
        return;
    Sound_PlayVoice(0, 0, 0xd1);
    s->func = Boss1Hammer_State_LaunchRoll;
}
