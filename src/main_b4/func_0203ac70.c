#include <nitro/types.h>

void func_0203a9d0(s32 a);
void func_0203a6bc(s32 a, s32 b, s32 c, s32 d, s32 e);
void func_0203a590(s32 a);
void func_0203a504(s32 a);
void func_02039ef8(s32 a);
void func_02039d40(s32 a);
void func_02002ec8(void);
void func_0203a43c(s32 a);
void func_0203a288(s32 a, s32 b, s32 c, s32 d, s32 e);
void func_0203a1f8(s32 a);
void func_0203a18c(s32 a);
void func_02002eb4(void);
void func_02002ea0(void);
void func_02039d08(s32 i);

void func_0203ac70(s32 mode) {
    func_0203a9d0(0);
    func_0203a6bc(0, 0x10, 0x40, 0, 0);
    func_0203a590(0);
    func_0203a504(0);
    func_02039ef8(0);
    func_02039d40(0);
    func_02002ec8();
    func_0203a43c(0);
    func_0203a288(0, 0x10, 0x40, 0, 0);
    func_0203a1f8(0);
    func_0203a18c(0);
    func_02002eb4();
    func_02002ea0();
    switch (mode) {
    case 0:
        func_0203a9d0(1);
        func_0203a6bc(2, 0x200010, 0x40, 0, 0x400);
        func_0203a590(0x10);
        func_0203a504(0x20);
        func_02039d08(0);
        func_0203a43c(4);
        func_0203a288(8, 0x200010, 0x40, 0, 0x400);
        func_0203a1f8(0x80);
        func_0203a18c(0x100);
        func_02039d08(1);
        break;
    case 1:
        func_02039ef8(7);
        func_02039d40(0x10);
        func_0203a9d0(8);
        func_0203a6bc(0x60, 0x10, 0x40, 0, 0x400);
        func_02039d08(0);
        func_0203a43c(0x80);
        func_0203a288(0x100, 0x10, 0x40, 0, 0x200);
        func_02039d08(1);
        break;
    case 2:
        func_02039ef8(0xf);
        func_02039d40(0x70);
        func_0203a6bc(0, 0x10, 0x40, 0, 0);
        func_0203a43c(0x80);
        func_0203a288(0x100, 0x10, 0x40, 0, 0x200);
        func_02039d08(1);
        break;
    }
}
