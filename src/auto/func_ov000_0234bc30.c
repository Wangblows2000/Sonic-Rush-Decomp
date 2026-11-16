/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_0234c4a4();
void func_ov000_0234bb0c();
s32 ObjRect_SetAtkDefFlags();
extern u8 data_ov000_02368ba0[];
s32 Sound_PlaySeqArcEx();

void func_ov000_0234bc30(u8 *p0) {
    func_ov000_0234c4a4(p0, 0x31);
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) | 4);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0x10);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_0234bb0c;
    ObjRect_SetAtkDefFlags(((u8 *)p0 + 0x2cc), 0x13, 0, (u32)func_ov000_0234bb0c);
    *(u32 *)((u8 *)p0 + 0x2e8) = 0;
    *(u16 *)((u8 *)p0 + 0x2d6) = (*(u16 *)((u8 *)p0 + 0x2d6) & ~8);
    *(u16 *)((u8 *)p0 + 0x3c) = *(s16 *)((u8 *)data_ov000_02368ba0 + (*(u8 *)((u8 *)p0 + 0x324) * 0x30));
    *(u8 *)((u8 *)p0 + 0x4) = 0xe;
    if (*(u8 *)((u8 *)p0 + 0x324) != 1) {
        Sound_PlaySeqArcEx(((u8 *)p0 + 0x384), 0, 0, data_ov000_02368ba0);
        return;
    }
    Sound_PlaySeqArcEx(((u8 *)p0 + 0x384), 0, 0x11);
}
