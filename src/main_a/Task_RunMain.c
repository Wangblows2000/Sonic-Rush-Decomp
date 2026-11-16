#include <nitro/types.h>

extern u32 gTaskListMainHead;
extern u32 gTaskListMainTail;
void Task_RunList(u32 a, u32 b);

void Task_RunMain(void) {
    Task_RunList(gTaskListMainHead, gTaskListMainTail);
}
