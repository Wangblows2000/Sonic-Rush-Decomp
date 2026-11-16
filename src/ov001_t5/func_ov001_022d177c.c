#include <nitro/types.h>

typedef struct {
    u8 pad0[0xd08];
    s32 count;
} UnkStruct_022d177c;

void func_ov001_022d0070(UnkStruct_022d177c *p);
void StaffRoll_SetState(UnkStruct_022d177c *p, void (*fn)(UnkStruct_022d177c *));
void func_ov001_022d1710(UnkStruct_022d177c *p);
void func_ov001_022d1904(UnkStruct_022d177c *p);

#pragma thumb on
void func_ov001_022d177c(UnkStruct_022d177c *p) {
    s32 *c;

    func_ov001_022d0070(p);
    c = &p->count;
    (*c)++;
    if (*c >= 4) {
        StaffRoll_SetState(p, func_ov001_022d1710);
        return;
    }
    StaffRoll_SetState(p, func_ov001_022d1904);
}
#pragma thumb reset
