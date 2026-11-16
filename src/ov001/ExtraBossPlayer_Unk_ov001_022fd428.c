#include <nitro/types.h>

void *ExtraBossObj_Create(s32 a, s32 b);
void *ExtraBossPlayer_Unk_ov001_022fd2dc(s32 a, s32 b);
void ExtraBossObj_State_Unk_ov001_022fd220(void);

u8 *ExtraBossPlayer_Unk_ov001_022fd428(s32 kind, void *parent, s32 x, s32 y, s32 z) {
    u8 *t = ExtraBossObj_Create(1, 0x3200);
    if (t == NULL) {
        return NULL;
    }
    *(s32 *)(t + 0x1c8) = kind;
    *(void **)(t + 0x84) = ExtraBossPlayer_Unk_ov001_022fd2dc(0, kind);
    *(void **)(t + 0x7c) = parent;
    *(u16 *)(t + 0x98) = 1;
    *(u32 *)(t + 0x14) |= 1;
    *(s32 *)(t + 0x18) = x;
    *(s32 *)(t + 0x1c) = y;
    *(s32 *)(t + 0x20) = z;
    *(void (**)(void))(t + 0x74) = ExtraBossObj_State_Unk_ov001_022fd220;
    return t;
}
