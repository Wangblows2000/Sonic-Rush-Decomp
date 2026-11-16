/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_023425f4();
s32 func_ov000_02304064();
s32 MapObj74_Action_Unk_ov000_023041f4();
s32 func_ov000_0233b654();
s32 func_ov000_0233b648();
extern u8 gCamera[];

void MapObj74_State_Unk_ov000_023040fc(u8 *p0) {
    func_ov000_023425f4(*(u32 *)((u8 *)p0 + 0x10), *(u32 *)((u8 *)p0 + 0x14));
    s32 t0 = func_ov000_02304064(p0);
    if (t0 == 0) {
        MapObj74_Action_Unk_ov000_023041f4(p0);
    }
    s32 t1 = func_ov000_0233b654();
    s32 t2 = func_ov000_0233b648();
    *(u32 *)((u8 *)t1 + 0x4) = (*(u32 *)((u8 *)p0 + 0x10) - 0x8000);
    *(u32 *)((u8 *)t1 + 0x8) = (*(u32 *)((u8 *)p0 + 0x14) - 0x6000);
    *(u32 *)((u8 *)t2 + 0x4) = (*(u32 *)((u8 *)t1 + 0x4) + *(u32 *)((u8 *)gCamera + 0xd4));
    *(u32 *)((u8 *)t2 + 0x8) = (*(u32 *)((u8 *)t1 + 0x8) + *(u32 *)((u8 *)gCamera + 0xd8));
}
