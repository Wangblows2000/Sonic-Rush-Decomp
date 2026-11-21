#include <nitro/types.h>

typedef struct UnkStruct_0230ce24 {
    u8 pad[0x18];
    u8 *unk18;
} UnkStruct_0230ce24;

extern s32 data_ov000_0236a00c;

void IronBall_OnHit_Unk_ov000_0230ce24(void *unused, UnkStruct_0230ce24 *s) {
    u8 *p = s->unk18;
    if (p == NULL)
        return;
    if (*p == 1)
        data_ov000_0236a00c = 1;
}
