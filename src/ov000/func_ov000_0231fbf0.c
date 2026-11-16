#include <nitro/types.h>

typedef struct UnkStruct_0231fbf0_2 {
    void (*func)(void *);
} UnkStruct_0231fbf0_2;

typedef struct UnkStruct_0231fbf0 {
    u8 pad[0x10];
    UnkStruct_0231fbf0_2 *unk10;
} UnkStruct_0231fbf0;

extern UnkStruct_0231fbf0 *gTaskCurrent;

void func_ov000_0231fbf0(void) {
    UnkStruct_0231fbf0_2 *p = gTaskCurrent->unk10;
    p->func(p);
}
