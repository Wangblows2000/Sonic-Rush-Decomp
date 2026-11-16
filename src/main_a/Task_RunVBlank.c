#include <nitro/types.h>

extern u32 gTaskListVBlankHead;
extern u32 gTaskListVBlankTail;
void Task_RunList(u32 a, u32 b);

void Task_RunVBlank(void) {
    Task_RunList(gTaskListVBlankHead, gTaskListVBlankTail);
}
