#include <nitro/types.h>

typedef struct {
    u8 pad0[0x1C];
    u8 unk1C;
    u8 unk1D;
} UnkStruct_022c4560;

typedef struct {
    u8 pad0[0xE0];
    s32 unkE0;
    s32 unkE4;
} UnkStruct_0236aad8;

extern u32 data_0208fe70;
extern UnkStruct_022c4560 data_022c4560;
extern UnkStruct_0236aad8 data_ov000_0236aad8;

void func_ov000_02329d38(void *a, s16 x, s16 y, s32 b, s32 c, s32 d, s32 e);
void func_ov000_02329f50(void *a, s16 x, s16 y, s32 b, s32 c);

static inline u16 Rand(void) {
    data_0208fe70 = data_0208fe70 * 0x196225 + 0x3c6ef35f;
    return (u16)(data_0208fe70 >> 16);
}

void func_ov000_0232a11c(void *a, s32 x, s32 y, s32 c) {
    u16 r = Rand() & 3;
    BOOL b;

    if (data_022c4560.unk1C >= 9 || data_022c4560.unk1D >= 2) {
        if (data_ov000_0236aad8.unkE0 != 0 || data_ov000_0236aad8.unkE4 != 0)
            b = TRUE;
        else
            b = FALSE;
        if (!b)
            return;
        switch (r) {
        case 0:
            func_ov000_02329d38(a, x, y, 0, c, data_ov000_0236aad8.unkE0, data_ov000_0236aad8.unkE4);
            break;
        case 1:
            func_ov000_02329d38(a, x + 4, y + 8, 1, c, data_ov000_0236aad8.unkE0, data_ov000_0236aad8.unkE4);
            func_ov000_02329d38(a, x + 1, y - 3, 1, c, data_ov000_0236aad8.unkE0, data_ov000_0236aad8.unkE4);
            break;
        case 2:
            func_ov000_02329d38(a, x, y, 0, c, data_ov000_0236aad8.unkE0, data_ov000_0236aad8.unkE4);
            func_ov000_02329d38(a, x + 2, y + 6, 1, c, data_ov000_0236aad8.unkE0, data_ov000_0236aad8.unkE4);
            break;
        case 3:
            func_ov000_02329d38(a, x, y, 0, c, data_ov000_0236aad8.unkE0, data_ov000_0236aad8.unkE4);
            func_ov000_02329d38(a, x - 2, y + 6, 1, c, data_ov000_0236aad8.unkE0, data_ov000_0236aad8.unkE4);
            func_ov000_02329d38(a, x + 4, y + 9, 1, c, data_ov000_0236aad8.unkE0, data_ov000_0236aad8.unkE4);
            break;
        }
    } else {
        switch (r) {
        case 0:
            func_ov000_02329f50(a, x, y, 0, c);
            break;
        case 1:
            func_ov000_02329f50(a, x + 4, y + 8, 1, c);
            func_ov000_02329f50(a, x + 1, y - 3, 1, c);
            break;
        case 2:
            func_ov000_02329f50(a, x, y, 0, c);
            func_ov000_02329f50(a, x + 2, y + 6, 1, c);
            break;
        case 3:
            func_ov000_02329f50(a, x, y, 0, c);
            func_ov000_02329f50(a, x - 2, y + 6, 1, c);
            func_ov000_02329f50(a, x + 4, y + 9, 1, c);
            break;
        }
    }
}
