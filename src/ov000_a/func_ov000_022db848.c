#include <nitro/types.h>

typedef struct UnkStruct_022db848 {
    u8 pad[0x10];
    s32 unk10;
} UnkStruct_022db848;

extern UnkStruct_022db848 *data_ov000_0236ac60;

BOOL func_ov000_022db848(UnkStruct_022db848 *o) {
    s32 d = data_ov000_0236ac60->unk10 - o->unk10;
    BOOL f = d > 0;
    if ((d < 0 ? -d : d) <= 0x384ff)
        return f;
    return !f;
}
