#include <nitro/types.h>

typedef struct UnkStruct_022c8e64_2 {
    s32 unk0;
} UnkStruct_022c8e64_2;

typedef struct UnkStruct_022c8e64 UnkStruct_022c8e64;
struct UnkStruct_022c8e64 {
    u8 pad[0x68];
    void (*func)(UnkStruct_022c8e64 *s);
    u8 pad1[0x20c - 0x6c];
    u8 unk20C[0x218 - 0x20c];
    u16 unk218;
    u8 pad2[0x334 - 0x21a];
    u8 unk334;
};

extern UnkStruct_022c8e64_2 gGameState;

void Boss1_UpdateCameraShake(void *p, s32 a, s32 b);
void func_ov000_022cc35c(s32 a, s32 b);
void ScreenShake_Start(s32 a);
void Boss1Neck_State_Unk_ov000_022c8dec(UnkStruct_022c8e64 *s);

void Boss1Neck_State_Unk_ov000_022c8e64(UnkStruct_022c8e64 *s) {
    Boss1_UpdateCameraShake(s->unk20C, 2, 7);
    s->unk218++;
    if ((s->unk218 & 3) == 0) {
        if (s->unk218 & 0x8000)
            func_ov000_022cc35c(gGameState.unk0, 0);
        else
            func_ov000_022cc35c(gGameState.unk0, 1);
        s->unk218 &= ~3;
        s->unk218 ^= 0x8000;
    }
    if (!(s->unk334 & 2))
        return;
    if (s->unk218 & 0x8000)
        return;
    ScreenShake_Start(0);
    s->func = Boss1Neck_State_Unk_ov000_022c8dec;
}
