#include <ov000_c/stageobj.h>

typedef struct {
    u32 unk0;
    u8 pad[0x10 - 4];
    s32 unk10;
    s32 unk14;
} UnkStruct_022c4560;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x434 - 0x20C];
    s32 unk434;
    u16 unk438;
    u8 pad43A[0x458 - 0x43A];
    s32 unk458;
} UnkStruct_022f3bb8;

extern UnkStruct_022c4560 gGameState;
extern s16 data_ov000_02364020[][9];

void func_ov000_022f4ce4(UnkStruct_022f3bb8 *s, s32 a, s32 b, s32 c);
void BossFPartD8_State_Unk_ov000_022f39e0(UnkStruct_022f3bb8 *s);

static inline s32 GetIdxB(void) {
    BOOL b = TRUE;
    if (gGameState.unk10 != 2 && gGameState.unk10 != 3)
        b = FALSE;
    if (b)
        return 1;
    return gGameState.unk14;
}

void BossFPartD8_State_Unk_ov000_022f3bb8(UnkStruct_022f3bb8 *s) {
    s32 a;

    s->base.unk2E = 0;
    if (s->unk458 >= 6 && s->unk458 <= 8 && s->unk438 != 0) {
        if (s->unk434 != 0)
            a = 0xa;
        else
            a = 0xb;
    } else {
        if (s->unk434 != 0)
            a = 8;
        else
            a = 9;
    }
    func_ov000_022f4ce4(s, a, data_ov000_02364020[GetIdxB()][s->unk458], 0);
    s->base.unk68 = BossFPartD8_State_Unk_ov000_022f39e0;
    ((void (*)(UnkStruct_022f3bb8 *))s->base.unk68)(s);
}
