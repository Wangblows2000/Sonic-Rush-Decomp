#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    void (*unk8)(void);
} UnkStruct_02361378_Task;

extern UnkStruct_02361378_Task *data_022b4574;

s32 func_0204b6cc(void);
void func_02054dbc(void);
u32 func_0203e944(void);
void func_020516b4(void);
void func_ov000_0236133c(void);

#pragma thumb on

void func_ov000_02361378(void) {
    s32 s = func_0204b6cc();

    if (s == 2) {
        func_02054dbc();
        return;
    }
    if (func_0203e944() & 0) {
        func_02054dbc();
        return;
    }
    if (s == 1) {
        func_020516b4();
        data_022b4574->unk8 = func_ov000_0236133c;
    }
}

#pragma thumb reset
