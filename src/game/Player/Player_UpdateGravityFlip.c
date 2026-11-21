/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Stage_IsGravityFlipped();
s32 Stage_SelectBGMLayer();
extern u8 data_022c4590[];
extern u8 gGameState[];
s32 Sound_PlaySeqArcEx();

void Player_UpdateGravityFlip(u8 *p0) {
    s32 t0 = Stage_IsGravityFlipped(*(u32 *)((u8 *)p0 + 0x10), *(u32 *)((u8 *)p0 + 0x14));
    if (t0 != 0) {
        if (*(u8 *)((u8 *)p0 + 0x5) == 0) {
            if (*(s16 *)((u8 *)p0 + 0x3b4) == 0) {
                Stage_SelectBGMLayer(1);
            }
            *(u16 *)((u8 *)p0 + 0x2e) = -*(s16 *)((u8 *)p0 + 0x2e);
            *(u16 *)((u8 *)p0 + 0x30) = -*(s16 *)((u8 *)p0 + 0x30);
            *(u8 *)((u8 *)p0 + 0x1) = (*(u8 *)((u8 *)p0 + 0x1) + 0x80);
            *(u16 *)((u8 *)p0 + 0x42e) = 0x10;
            *(u16 *)((u8 *)p0 + 0x42c) = 0x20;
            if ((u32)(*(u32 *)((u8 *)data_022c4590 + 0x18) + 0x1) < (u32)*(u32 *)((u8 *)gGameState + 0x20)) {
                if ((*(u32 *)((u8 *)p0 + 0x32c) & 0x400) == 0) {
                    Sound_PlaySeqArcEx(((u8 *)p0 + 0x384), 0, 0x66, 0x20);
                }
            }
        }
        *(u8 *)((u8 *)p0 + 0x5) = 0x80;
        return;
    }
    if (*(u8 *)((u8 *)p0 + 0x5) == 0x80) {
        if (*(s16 *)((u8 *)p0 + 0x3b4) == 0) {
            Stage_SelectBGMLayer(0);
        }
        *(u16 *)((u8 *)p0 + 0x2e) = -*(s16 *)((u8 *)p0 + 0x2e);
        *(u16 *)((u8 *)p0 + 0x30) = -*(s16 *)((u8 *)p0 + 0x30);
        *(u8 *)((u8 *)p0 + 0x1) = (*(u8 *)((u8 *)p0 + 0x1) + 0x80);
        *(u16 *)((u8 *)p0 + 0x42e) = 0x20;
        *(u16 *)((u8 *)p0 + 0x42c) = 0x10;
        if ((u32)(*(u32 *)((u8 *)data_022c4590 + 0x18) + 0x1) < (u32)*(u32 *)((u8 *)gGameState + 0x20)) {
            if ((*(u32 *)((u8 *)p0 + 0x32c) & 0x400) == 0) {
                Sound_PlaySeqArcEx(((u8 *)p0 + 0x384), 0, 0x66, 0x10);
            }
        }
    }
    *(u8 *)((u8 *)p0 + 0x5) = 0;
}
