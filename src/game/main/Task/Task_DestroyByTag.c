#include <nitro/types.h>

typedef struct UnkNode_02036fdc UnkNode_02036fdc;
struct UnkNode_02036fdc { u32 unk0; UnkNode_02036fdc *next; u8 unk8[0x12]; u16 unk1A; };
extern UnkNode_02036fdc *gTaskListMainHead;
extern UnkNode_02036fdc *gTaskListVBlankTail;
void Task_Destroy(UnkNode_02036fdc *n);

void Task_DestroyByTag(u16 id) {
    UnkNode_02036fdc *n;
    for (n = gTaskListMainHead->next; n != gTaskListVBlankTail; n = n->next) {
        if (id == n->unk1A) {
            Task_Destroy(n);
        }
    }
}
