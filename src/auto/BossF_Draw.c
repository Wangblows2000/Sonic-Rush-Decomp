/* paramclone of Boss4PartD1_Draw (src/ov000_a/Boss4PartD1_Draw.c); immediates and relocations remapped. */
#include <nitro/types.h>

typedef struct UnkStruct_022f84bc {
    u8 pad[0x5c];
    u16 unk5C;
    u8 pad1[0x244 - 0x5e];
    u8 unk20C[0x138];
} UnkStruct_022f84bc;

typedef struct UnkTask_022f84bc {
    u8 pad[0x10];
    UnkStruct_022f84bc *unk10;
} UnkTask_022f84bc;

extern UnkTask_022f84bc *gTaskCurrent;

void Model_Animate(void *a);
void Model_Draw(void *a);

void BossF_Draw(void) {
    UnkStruct_022f84bc *s = gTaskCurrent->unk10;
    void *a = s->unk20C;
    if (s->unk5C & 0x20)
        return;
    Model_Animate(a);
    Model_Draw(a);
}
