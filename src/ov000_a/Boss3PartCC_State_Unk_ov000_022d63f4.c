#include <nitro/types.h>

typedef struct UnkStruct_022d63f4_a {
    u8 pad[0x10c];
    u8 unk10C;
    u8 pad1[0x114 - 0x10d];
    s32 unk114;
    u8 pad2[0x170 - 0x118];
} UnkStruct_022d63f4_a;

typedef struct UnkStruct_022d63f4 UnkStruct_022d63f4;
struct UnkStruct_022d63f4 {
    u8 pad[0x2e];
    s16 unk2E;
    u8 pad0[0x5c - 0x30];
    u16 unk5C;
    u8 pad1[0x68 - 0x5e];
    void (*func)(UnkStruct_022d63f4 *s);
    u8 pad2[0x258 - 0x6c];
    u16 unk258;
    u16 unk25A;
    u8 pad3[0x264 - 0x25c];
    u16 unk264;
    u8 pad4[0x2c4 - 0x266];
    UnkStruct_022d63f4_a unk2C4;
    void *unk434;
};

void Model_SetAnimation(void *p, s32 a, void *b, s32 c, s32 d);

extern s32 gGameState;
extern s32 data_ov000_02369b18[];
extern u32 data_ov000_02365488[];
extern u16 data_ov000_02363148[][6];
extern u16 data_ov000_02363160[][6];
extern u16 data_ov000_023630ac[];

void func_ov000_022dac94(u16 *p, s32 a, s32 b);
void Boss_LoadModelPalettes(s32 a, u32 *p1, u16 *p2, u16 *p3, u16 n);
void func_ov000_022da6b4(void);
void Boss3PartCC_State_Unk_ov000_022d63f0(UnkStruct_022d63f4 *s);

void Boss3PartCC_State_Unk_ov000_022d63f4(UnkStruct_022d63f4 *s) {
    s16 v;
    func_ov000_022dac94(&s->unk264, 2, 6);
    if (s->unk25A < 0x2d || (s->unk258 & 0x8000)) {
        s->unk25A++;
        s->unk258++;
        if (!(s->unk258 & 1)) {
            if (s->unk258 & 0x8000)
                Boss_LoadModelPalettes(data_ov000_02369b18[1], data_ov000_02365488, data_ov000_02363148[gGameState], data_ov000_023630ac, 6);
            else
                Boss_LoadModelPalettes(data_ov000_02369b18[1], data_ov000_02365488, data_ov000_02363160[gGameState], data_ov000_023630ac, 6);
            s->unk258 &= ~1;
            s->unk258 ^= 0x8000;
        }
    }
    v = s->unk2E;
    if (v < 0) {
        if (-v > 0xc)
            s->unk2E = v + 0xc;
        else
            s->unk2E = 0;
    } else if (v > 0) {
        if (v > 0xc)
            s->unk2E = v - 0xc;
        else
            s->unk2E = 0;
    }
    if (s->unk2C4.unk10C & 2) {
        func_ov000_022da6b4();
        s->func = Boss3PartCC_State_Unk_ov000_022d63f0;
    }
}
