#include <nitro/types.h>

typedef struct UnkNode_02037134 UnkNode_02037134;
struct UnkNode_02037134 { u32 unk0; UnkNode_02037134 *next; };
extern UnkNode_02037134 *gTaskListMainHead;
extern UnkNode_02037134 *gTaskListMainTail;
extern UnkNode_02037134 *gTaskListVBlankHead;
extern UnkNode_02037134 *gTaskListVBlankTail;
void Task_Destroy(UnkNode_02037134 *n);

void Task_DestroyAll(void) {
    UnkNode_02037134 *n;
    for (n = gTaskListMainHead->next; n != gTaskListMainTail; n = n->next) {
        Task_Destroy(n);
    }
    for (n = gTaskListVBlankHead->next; n != gTaskListVBlankTail; n = n->next) {
        Task_Destroy(n);
    }
}
