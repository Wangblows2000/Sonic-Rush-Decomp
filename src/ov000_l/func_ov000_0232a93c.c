#include <nitro/types.h>

typedef struct {
    u8 pad0[0xe0];
    s32 unkE0;
    s32 unkE4;
} UnkStruct_0236aad8;

typedef struct {
    u8 pad0[0x1c];
    u8 unk1C;
    u8 unk1D;
} UnkStruct_022c4560;

extern UnkStruct_022c4560 data_022c4560;
extern UnkStruct_0236aad8 data_ov000_0236aad8;

void func_ov000_0232a7b8(s32 a, s32 b, s32 c, s32 d);
void func_ov000_0232a780(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

void func_ov000_0232a93c(s32 a, s32 b, s32 c, s32 d) {
    BOOL on;

    if (!(data_022c4560.unk1C < 9 && data_022c4560.unk1D < 2)) {
        if (data_ov000_0236aad8.unkE0 != 0 || data_ov000_0236aad8.unkE4 != 0)
            on = TRUE;
        else
            on = FALSE;
        if (on)
            func_ov000_0232a780(a, b, c, d, data_ov000_0236aad8.unkE0, data_ov000_0236aad8.unkE4);
        return;
    }
    func_ov000_0232a7b8(a, b, c, d);
}
