#include <nitro/types.h>

typedef struct UnkStruct_022fc4d4_g {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_022fc4d4_g;

extern UnkStruct_022fc4d4_g gGameState;

u8 *ExtraBossPlayer_Unk_ov001_022fd428(s32 a, void *b, s32 c, s32 d, s32 e);
void *ExtraBoss_GetPlayer(void);
u8 *ExtraBoss_GetWork(void);
void ExtraBoss_Unk_ov001_022fe8d4(void *a, void *b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i, s32 j, s32 k);
void ExtraBossObj_State_Unk_ov001_022fc494(void);
void Sound_PlaySeqArc(void *p, u32 a, u32 b);

static inline BOOL IsMode23(void) {
    BOOL r = TRUE;
    if (gGameState.unk10 != 2 && gGameState.unk10 != 3)
        r = FALSE;
    return r;
}

static inline s32 GetMode(void) {
    if (IsMode23())
        return 1;
    return gGameState.unk14;
}

void ExtraBoss_Unk_ov001_022fc4d4(void *p, s32 y, s32 z) {
    u8 *t;
    s32 x = *(s32 *)(ExtraBoss_GetWork() + 0x31c);
    t = ExtraBossPlayer_Unk_ov001_022fd428(6, p, x, y, z);
    if (t == NULL)
        return;
    *(void (**)(void))(t + 0x74) = ExtraBossObj_State_Unk_ov001_022fc494;
    *(u32 *)(t + 0x14) |= 0x10000;
    *(void **)(t + 0x80) = ExtraBoss_GetPlayer();
    *(u16 *)(t + 0x98) = 0xffff;
    *(s32 *)(t + 0x1cc) = GetMode() == 1 ? 0x10 : 0x18;
    ExtraBoss_Unk_ov001_022fe8d4(t + 0xe8, t, 1, 1, 0x80000, 0x10000, 0x9000, 0, 0, 0, 0);
    *(u32 *)(t + 0xf0) |= 0x2200;
    *(u8 **)(t + 0xa0) = t + 0xe8;
    Sound_PlaySeqArc(0, 0, 0xa8);
}
