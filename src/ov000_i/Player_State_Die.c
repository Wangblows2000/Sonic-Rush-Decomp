#include <nitro/types.h>

typedef struct {
    u8 pad0[0x1c];
    u8 unk1C;
    u8 unk1D;
} UnkCfg_02348f58;

extern UnkCfg_02348f58 data_022c4560;
void Player_SetAnimation(u8 *w, s32 a);
void BrightnessFade_Create(s32 a, s32 b);
BOOL BrightnessFade_IsDone(void);
void func_ov000_0234d0bc(u8 *w);

void Player_State_Die(u8 *w) {
    (*(s32 *)(w + 0xc))++;
    if (*(s16 *)(w + 0x328) == 0x11 && (*(u16 *)(w + 0x5c) & 8)) {
        Player_SetAnimation(w, 0x12);
        *(u16 *)(w + 0x5c) |= 4;
    }
    if (*(s32 *)(w + 0xc) == 0x3c) {
        if (data_022c4560.unk1C >= 9 || data_022c4560.unk1D >= 2)
            BrightnessFade_Create(4, 0x40);
        else
            BrightnessFade_Create(4, 0x100);
    }
    if (*(s32 *)(w + 0xc) < 0x3c)
        return;
    if (!BrightnessFade_IsDone())
        return;
    BrightnessFade_Create(2, 0x100);
    *(u32 *)(w + 0x32c) &= ~0x400;
    func_ov000_0234d0bc(w);
    (*(void (**)(u8 *))(w + 0x334))(w);
}
