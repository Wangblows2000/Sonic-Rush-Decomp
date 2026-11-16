#pragma opt_common_subs off
#include <nitro/types.h>

typedef struct {
    u32 flags;
    u8 unk4[4];
    u8 unk8[2];
    u8 unkA[2];
} UnkStruct_0233d324;

extern UnkStruct_0233d324 *data_ov000_0236ac18;
extern u32 data_02087cb8;
void func_ov000_0233d43c(UnkStruct_0233d324 *w, u8 i);
void func_ov000_0233d404(UnkStruct_0233d324 *w, u8 i);

void func_ov000_0233d324(void) {
    UnkStruct_0233d324 *w = data_ov000_0236ac18;
    u8 i;

    if (w == NULL)
        return;
    if (w->flags & 0x80000000)
        return;
    for (i = 0; i < 2; i++) {
        w->unkA[i] = w->unk8[i];
        if (!(data_02087cb8 & 0x80000000)) {
            if (w->unk4[i] == 1)
                func_ov000_0233d43c(w, i);
            else
                func_ov000_0233d404(w, i);
        }
    }
}
