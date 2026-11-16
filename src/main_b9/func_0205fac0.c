#include <nitro/types.h>

typedef struct {
    u16 unk0;
    u16 unk2;
    u16 unk4;
} UnkStruct_0205fac0;

extern UnkStruct_0205fac0 data_0208924c;

#pragma thumb on
void func_0205fac0(void) {
    UnkStruct_0205fac0 *p = (void *)&data_0208924c;

    p->unk0 = 0;
    p->unk2 = 0;
    p->unk4 = 0;
}
#pragma thumb reset
