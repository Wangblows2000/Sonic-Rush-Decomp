#include <nitro/types.h>

typedef struct UnkStruct_0230ddf0 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_0230ddf0;

void ItemBox_State_CycleVsItem(void);

void ItemBox_Action_Init(UnkStruct_0230ddf0 *s) {
    s->func = ItemBox_State_CycleVsItem;
}
