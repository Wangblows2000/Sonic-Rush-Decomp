#include <ov000_c/stageobj.h>

typedef struct {
    u32 unk0;
    u8 pad[0x10 - 4];
    s32 unk10;
    s32 unk14;
} UnkStruct_022c4560;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x216 - 0x20C];
    u16 unk216;
    u8 pad218[0x450 - 0x218];
    u16 unk450;
    u8 pad452[2];
    u16 unk454;
    u16 unk456;
} UnkStruct_022eff30;

extern UnkStruct_022c4560 gGameState;
extern u16 data_ov000_02363dd4[];

void func_ov000_022f0fdc(UnkStruct_022eff30 *s, s32 a);
void BossF_UpdateCameraShake(u16 *p, s32 a, s32 b);
void BossFPartD8_InitHitboxes(UnkStruct_022eff30 *s);
void func_ov000_022f4cbc(UnkStruct_022eff30 *s);
void BossF_LoadPalette(u32 a, s32 b);
void func_ov000_022efdf0(void);

static inline s32 GetIdxB(void) {
    BOOL b = TRUE;
    if (gGameState.unk10 != 2 && gGameState.unk10 != 3)
        b = FALSE;
    if (b)
        return 1;
    return gGameState.unk14;
}

void func_ov000_022eff30(UnkStruct_022eff30 *s) {
    func_ov000_022f0fdc(s, 0x10);
    s->unk450 = 0x100;
    BossF_UpdateCameraShake(&s->unk450, 4, 8);
    if (s->unk454 > 0xb4) {
        if (!(s->unk216 & 4)) {
            BossFPartD8_InitHitboxes(s);
            func_ov000_022f4cbc(s);
            s->unk216 |= 4;
        }
        s->unk456++;
        if (!(s->unk456 & 3)) {
            if (s->unk456 & 0x8000)
                BossF_LoadPalette(gGameState.unk0, 0);
            else
                BossF_LoadPalette(gGameState.unk0, 1);
            s->unk456 &= ~3;
            s->unk456 ^= 0x8000;
        }
    }
    if (data_ov000_02363dd4[GetIdxB()] >= s->unk454++)
        return;
    if (!(s->unk456 & 0x8000))
        s->base.unk68 = func_ov000_022efdf0;
}
