#include <nitro/types.h>

typedef struct UnkStruct_022f3e00 {
    u8 pad[0x2e];
    u16 unk2E;
    u8 pad1[0x68 - 0x30];
    void (*func)(void);
    u8 pad2[0x434 - 0x6c];
    s32 unk434;
    u16 unk438;
    u16 unk43A;
} UnkStruct_022f3e00;

extern u32 gRandSeed;
extern s32 gBossStatus[];

void func_ov000_022f9044(UnkStruct_022f3e00 *s);
void func_ov000_022f4cbc(UnkStruct_022f3e00 *s);
void Sound_PlayVoice(s32 a, s32 b, s32 c);
void BossFPartD8_State_Unk_ov000_022f3dd4(void);

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

void func_ov000_022f3e00(UnkStruct_022f3e00 *s) {
    s->unk2E = 0;
    func_ov000_022f9044(s);
    func_ov000_022f4cbc(s);
    s->unk434 = (Rand() & 1) ? 1 : 0;
    s->unk438 = 0;
    s->unk43A = 0;
    Sound_PlayVoice(0, 0, 0xd0);
    gBossStatus[1] = 0x30;
    s->func = BossFPartD8_State_Unk_ov000_022f3dd4;
}
