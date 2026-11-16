/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_02056ec8();
s32 func_02056e94();
void func_02055fa4();

void func_02056038(u8 *p0, u8 *p1) {
    *(u8 *)((u8 *)p1 + 0xd) = *(s16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x4) + 0x28);
    *(u8 *)((u8 *)p1 + 0xc) = *(u8 *)((u8 *)p1 + 0xd);
    s32 t0 = *(u8 *)((u8 *)p0 + 0x8);
    s32 t1 = *(u8 *)((u8 *)p0 + 0x9);
    s32 t2 = func_02056ec8(*(u8 *)((u8 *)p0 + 0x8), *(u8 *)((u8 *)p0 + 0x9));
    *(u32 *)((u8 *)p1 + 0x10) = t2;
    s32 t3 = func_02056e94(t0, t1);
    *(u32 *)((u8 *)p1 + 0x14) = t3;
    *(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x4) + 0x4) = (*(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x4) + 0x4) | 0x2000);
    *(u32 *)p1 = (u32)func_02055fa4;
}

#pragma thumb reset
