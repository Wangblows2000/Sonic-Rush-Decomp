#include <nitro/types.h>

typedef struct UnkStruct_022dcc10 {
    u8 pad[0xbd0];
    s32 unkBD0;
    s32 unkBD4;
} UnkStruct_022dcc10;

typedef struct UnkStruct_022dcc10_7 {
    u8 pad[8];
    u16 unk8;
} UnkStruct_022dcc10_7;

typedef struct UnkStruct_022dcc10_4 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_022dcc10_4;

extern UnkStruct_022dcc10_7 data_ov000_02369ac0;
extern UnkStruct_022dcc10_4 data_022c4560;
extern u32 data_0208fe70;
extern s16 data_ov000_023636a8[][8][2];

static inline u16 Rand(void) {
    data_0208fe70 = data_0208fe70 * 0x196225 + 0x3c6ef35f;
    return (u16)(data_0208fe70 >> 16);
}

static inline s32 GetIdx(void) {
    BOOL b = TRUE;
    if (data_022c4560.unk10 != 2 && data_022c4560.unk10 != 3)
        b = FALSE;
    if (b)
        return 1;
    return data_022c4560.unk14;
}

void func_ov000_022dcc10(UnkStruct_022dcc10 *s) {
    u16 r;
    s->unkBD0 = 0;
    r = Rand();
    s->unkBD4 = data_ov000_023636a8[GetIdx()][data_ov000_02369ac0.unk8 - 1][r & 1];
    s->unkBD4 -= 0x3c;
}
