#include <nitro/types.h>

typedef struct UnkStruct_022dc528 {
    u8 pad[0x5c];
    u16 unk5C;
    u8 pad1[0x20c - 0x5e];
    u8 unk20C[0x138];
} UnkStruct_022dc528;

typedef struct UnkTask_022dc528 {
    u8 pad[0x10];
    UnkStruct_022dc528 *unk10;
} UnkTask_022dc528;

extern UnkTask_022dc528 *gTaskCurrent;

void Model_Animate(void *a);
void Model_Draw(void *a);

void Boss4PartD1_Draw(void) {
    UnkStruct_022dc528 *s = gTaskCurrent->unk10;
    void *a = s->unk20C;
    if (s->unk5C & 0x20)
        return;
    Model_Animate(a);
    Model_Draw(a);
}
