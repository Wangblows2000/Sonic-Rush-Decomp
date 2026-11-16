#include <nitro/types.h>

typedef struct { u8 pad[4]; void *tasks[5]; } UnkStruct_0231f5a8;
extern UnkStruct_0231f5a8 *data_ov001_0231f5a8;
extern u32 gSystemFlags;
extern s32 gDisplaySub[];
void Task_Destroy(void *p);

void ExtraBoss_Unk_ov001_023063a0(void) {
    s32 i;
    for (i = 0; i < 5; i++) {
        if (data_ov001_0231f5a8->tasks[i] != NULL) {
            Task_Destroy(data_ov001_0231f5a8->tasks[i]);
            data_ov001_0231f5a8->tasks[i] = NULL;
        }
    }
    *(vu32 *)0x04001000 &= ~0x1f00;
    gDisplaySub[7] = 0;
    gSystemFlags &= ~0x400;
}
