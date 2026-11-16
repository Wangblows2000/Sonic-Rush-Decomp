#include <nitro/types.h>

typedef struct {
    u8 pad0[0x254];
    s16 unk254[2];
    s16 unk258[2];
    s16 unk25C[2];
    s16 unk260[2];
    s16 unk264[2];
} UnkStruct_0232d30c;

extern s8 data_02077944[];

s32 ObjShift_SetFromRef(s32 v, s32 a, s32 b, s32 c, s32 d, s32 e);

void func_ov000_0232d30c(UnkStruct_0232d30c *w, s32 i) {
    w->unk254[i] = ObjShift_SetFromRef(w->unk254[i], 0x100, 0x180, 0, 0x10, 0);
    w->unk258[i] = w->unk254[i];
    if (w->unk254[i] != 0x100)
        return;
    if (w->unk264[i] == 0) {
        w->unk25C[i] = 0;
        w->unk260[i] = 0;
        return;
    }
    w->unk25C[i] = data_02077944[(w->unk264[i] - 1) * 2];
    w->unk260[i] = data_02077944[(w->unk264[i] - 1) * 2 + 1];
    w->unk264[i]--;
}
