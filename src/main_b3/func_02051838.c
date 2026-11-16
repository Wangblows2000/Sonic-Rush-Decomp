#include <nitro/types.h>

typedef struct { u16 unk0; u16 unk2; } UnkStruct_0207ad9c;
extern void *data_022c4638;
extern void *data_022c4634;
extern u32 gSystemFlags;
extern UnkStruct_0207ad9c data_0207ad9c;

void Task_Destroy(void *t);
void func_0204331c(s32 a);
void func_02042d34(s32 a);
void func_02042ab8(void);

#pragma thumb on
void func_02051838(void) {
    if (data_022c4638 != NULL) {
        Task_Destroy(data_022c4638);
        data_022c4638 = NULL;
    }
    if (data_022c4634 != NULL) {
        Task_Destroy(data_022c4634);
        data_022c4634 = NULL;
    }
    gSystemFlags &= ~0x200;
    switch (data_0207ad9c.unk0) {
    case 0:
        func_0204331c(0);
        break;
    case 1:
        func_02042d34(0);
        break;
    case 2:
        func_02042ab8();
        break;
    }
}
#pragma thumb reset
