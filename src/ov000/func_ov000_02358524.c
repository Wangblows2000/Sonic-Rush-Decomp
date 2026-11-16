#include <nitro/types.h>

typedef struct UnkStruct_02358524 {
    u8 pad[8];
    void (*unk8)(void);
} UnkStruct_02358524;

extern UnkStruct_02358524 *gTaskCurrent;

u8 *func_020514e4(void);
void func_ov000_02358444(void);

#pragma thumb on
void func_ov000_02358524(void) {
    func_020514e4()[3] = 0;
    gTaskCurrent->unk8 = func_ov000_02358444;
}
#pragma thumb reset
