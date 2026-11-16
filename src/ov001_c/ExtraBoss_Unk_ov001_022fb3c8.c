#include <nitro/types.h>

typedef struct UnkStruct_022fb3c8_4560 {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_022fb3c8_4560;

typedef struct UnkStruct_022fb3c8_g {
    u8 pad0[0x40];
    s32 unk40;
} UnkStruct_022fb3c8_g;

extern UnkStruct_022fb3c8_4560 gGameState;
extern s32 data_ov001_0231d63c[][5];
extern s32 data_ov001_0231d614[][5];

u8 *ExtraBossPlayer_CreateFxObj(s32 kind, void *parent, s32 x, s32 y, s32 z);
void *ExtraBoss_GetPlayer(void);
void ExtraBossObj_State_Unk_ov001_022fb28c(void);
void ExtraBoss_Unk_ov001_022faf00(void);
UnkStruct_022fb3c8_g *ExtraBoss_GetWork(void);
void ExtraBoss_InitCollider(void *a, void *b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i, s32 j, s32 k);
void Sound_PlaySeqArc(u32 *p, u32 a, u32 b);

static inline BOOL IsMode_022fb3c8(void) {
    BOOL r = TRUE;
    switch (gGameState.unk10) {
    case 2:
    case 3:
        break;
    default:
        r = FALSE;
        break;
    }
    return r;
}

static inline s32 GetLevel_022fb3c8(void) {
    if (IsMode_022fb3c8())
        return 1;
    return gGameState.unk14;
}

void ExtraBoss_Unk_ov001_022fb3c8(void *parent, s32 x, s32 y, s32 z) {
    u8 *t = ExtraBossPlayer_CreateFxObj(0xf, parent, x, y, z);

    if (t == NULL)
        return;
    *(u8 *)(*(u8 **)(t + 0x84) + 0x10c) |= 4;
    *(void **)(t + 0x80) = ExtraBoss_GetPlayer();
    *(u32 *)(t + 0x14) |= 0x10;
    *(u16 *)(t + 0x98) = 0xffff;
    *(void **)(t + 0x74) = ExtraBossObj_State_Unk_ov001_022fb28c;
    *(s32 *)(t + 0x50) = data_ov001_0231d63c[GetLevel_022fb3c8()][ExtraBoss_GetWork()->unk40];
    *(s32 *)(t + 0x5c) = data_ov001_0231d614[GetLevel_022fb3c8()][ExtraBoss_GetWork()->unk40];
    ExtraBoss_InitCollider(t + 0xb0, t, 2, 0, 0x8000, 0x8000, 0x10000, 0, 0, 0, 0);
    *(void **)(t + 0xb4) = ExtraBoss_Unk_ov001_022faf00;
    *(u32 *)(t + 0xb8) |= 0x2000;
    *(u8 **)(t + 0x9c) = t + 0xb0;
    ExtraBoss_InitCollider(t + 0xe8, t, 1, 1, 0x8000, 0x8000, 0x10000, 0, 0, 0, 0);
    *(u8 **)(t + 0xa0) = t + 0xe8;
    Sound_PlaySeqArc(0, 0, 0xae);
}
