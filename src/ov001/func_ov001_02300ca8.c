#include <nitro/types.h>

typedef struct UnkStruct_02300ca8 { u8 pad[0x14]; u32 flags; u8 pad2[0x60]; void (*fn)(struct UnkStruct_02300ca8 *); } UnkStruct_02300ca8;
typedef struct { u8 pad[0xbc]; UnkStruct_02300ca8 *objs[0x2e]; } UnkStruct_0231f530;
extern UnkStruct_0231f530 *gExtraBossPlayerManager;
void func_ov001_02300a64(void *p);

void func_ov001_02300ca8(void) {
    s32 i;
    for (i = 0; i < 0x2e; i++) {
        UnkStruct_02300ca8 *o = gExtraBossPlayerManager->objs[i];
        if (o != NULL && !(o->flags & 0x20000) && !(o->flags & 0x40000)) {
            if (o->fn != NULL) {
                o->fn(o);
            } else {
                func_ov001_02300a64(o);
            }
        }
    }
}
