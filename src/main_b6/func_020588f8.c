#include <nitro/types.h>

typedef struct {
    u8 pad0[2];
    u8 b2;
} UnkState_020588f8;

typedef struct {
    u32 unk0;
    u32 unk4;
} UnkSave_020588f8;

extern UnkState_020588f8 data_022c4994;
extern UnkSave_020588f8 data_022c4680;

u32 func_020588bc(UnkSave_020588f8 *s);

BOOL func_020588f8(void) {
    data_022c4994.b2 = (data_022c4994.b2 + 1) % 5;
    data_022c4680.unk4++;
    if ((data_022c4680.unk4 - data_022c4994.b2) % 5 != 0)
        return FALSE;
    data_022c4680.unk0 = func_020588bc(&data_022c4680);
    return TRUE;
}
