/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_02039d08();
s32 func_02039b48();
s32 func_020395f4();
s32 func_0203a9d0();
s32 func_0203a6bc();
s32 func_0203a590();
s32 func_0203a504();
s32 func_0203a43c();
s32 func_0203a288();
s32 func_0203a1f8();
s32 func_0203a18c();
s32 func_02039ef8();
s32 func_02039d40();
void func_0203938c();
extern u8 data_02077d40[];
void func_02039360();
extern u8 data_02077d44[];
void func_0203931c();
extern u8 data_02077d48[];
void func_020392f0();
extern u8 data_02077d4c[];

void func_0203ae78(void) {
    func_02039d08(0);
    func_02039d08(1);
    func_02039b48();
    func_020395f4();
    func_0203a9d0(0);
    func_0203a6bc(0, 0x10, 0x40, 0, 0);
    func_0203a590(0);
    func_0203a504(0);
    func_0203a43c(0);
    func_0203a288(0, 0x10, 0x40, 0, 0);
    func_0203a1f8(0);
    func_0203a18c(0);
    func_02039ef8(0);
    func_02039d40(0);
    *(u32 *)data_02077d40 = (u32)func_0203938c;
    *(u32 *)data_02077d44 = (u32)func_02039360;
    *(u32 *)data_02077d48 = (u32)func_0203931c;
    *(u32 *)data_02077d4c = (u32)func_020392f0;
}
