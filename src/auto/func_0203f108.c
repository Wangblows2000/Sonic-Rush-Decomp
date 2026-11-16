/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_020405bc();
s32 func_020405e0();
extern u8 data_022c197c[];
extern u8 data_02078850[];
s32 func_0203f070();
extern u8 data_02078864[];
extern u8 data_022c195c[];
extern u8 data_022c196c[];
s32 func_0203c350();
extern u8 data_02078884[];
s32 func_02040658();
extern u8 data_020788a0[];

void func_0203f108(u8 *p0, s32 p1) {
    s32 t0 = *(u16 *)((u8 *)p0 + 0x2);
    if (t0 != 0) {
        func_020405bc(t0);
        s32 t1 = *(u16 *)((u8 *)p0 + 0x2);
        if (t1 == 0xc) {
            func_020405e0(9);
            return;
        }
        if (t1 == 0xb) {
            func_020405e0(9);
            return;
        }
        if (t1 == 1) {
            func_020405e0(8);
            return;
        }
        func_020405e0(9);
        return;
    }
    if (*(u16 *)((u8 *)p0 + 0x8) == 8) {
        return;
    }
    s32 t2 = *(u16 *)((u8 *)p0 + 0x8);
    if (t2 == 7) {
        s32 t3 = *(u32 *)data_022c197c;
        if (t3 != 0) {
            ((s32 (*)())t3)(data_02078850, t3);
        }
        func_020405e0(4);
        s32 t4 = func_0203f070();
        if (t4 == 0) {
            s32 t5 = *(u32 *)data_022c197c;
            if (t5 != 0) {
                ((s32 (*)())t5)(data_02078864, t5);
            }
            func_020405e0(3);
            return;
        }
        *(u16 *)data_022c195c = *(u16 *)((u8 *)p0 + 0xa);
        return;
    }
    if (t2 == 6) {
        return;
    }
    if (t2 == 9) {
        s32 t6 = *(u32 *)data_022c196c;
        if (t6 == 7) {
            func_0203c350(t6, p1);
        }
        s32 t7 = *(u32 *)data_022c197c;
        if (t7 != 0) {
            ((s32 (*)())t7)(data_02078884, t7);
        }
        func_020405bc(0x14);
        func_020405e0(9);
        return;
    }
    s32 t8 = *(u32 *)data_022c197c;
    if (t8 != 0) {
        s32 t9 = func_02040658(t2, t8);
        ((s32 (*)())*(u32 *)data_022c197c)(data_020788a0, *(u16 *)((u8 *)p0 + 0x8), t9, *(u32 *)data_022c197c);
    }
    func_020405e0(9);
}
