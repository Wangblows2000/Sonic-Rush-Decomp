#include <nitro/types.h>

typedef struct {
    s32 unk0;
    u8 pad4[0x18];
    u8 zone;
    u8 act;
} UnkState_020554dc;

typedef struct {
    u8 pad0[0xc];
    u16 unkc;
} UnkProg_020554dc;

extern UnkState_020554dc data_022c4560;
extern UnkProg_020554dc data_022c4680;

void func_0204ed24(void);
void func_0205a4ec(void (*f)(void));
BOOL func_0204eb7c(void);
void func_02054fb8(void);
void func_0205500c(void);
void func_02054ff0(void);

void func_020554dc(void) {
    func_0204ed24();
    func_0205a4ec(NULL);
    if (data_022c4560.unk0 != 1 && data_022c4560.zone == 0 && data_022c4680.unkc < 2) {
        func_02054fb8();
        return;
    }
    if (func_0204eb7c() || (data_022c4560.zone == 7 && data_022c4560.act == 2))
        func_0205500c();
    else
        func_02054ff0();
}
