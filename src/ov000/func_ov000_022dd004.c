#include <nitro/types.h>

typedef struct UnkStruct_022dd004_4 {
    s32 v[12];
} UnkStruct_022dd004_4;

typedef struct UnkStruct_022dd004_3 {
    u8 pad[0x24];
    UnkStruct_022dd004_4 unk24;
} UnkStruct_022dd004_3;

typedef struct UnkStruct_022dd004_2 {
    u8 pad[0x20c];
    UnkStruct_022dd004_3 *unk20C;
} UnkStruct_022dd004_2;

typedef struct UnkStruct_022dd004 {
    u8 pad[0x10];
    UnkStruct_022dd004_2 *unk10;
} UnkStruct_022dd004;

typedef struct UnkStruct_022dd004_5 {
    u8 pad[0xaf8];
    UnkStruct_022dd004_4 unkAF8;
} UnkStruct_022dd004_5;

extern UnkStruct_022dd004 *gTaskCurrent;
extern UnkStruct_022dd004_5 *data_ov000_02369b2c;

void Model_Animate(void *p);
void Model_Draw(void *p);

void func_ov000_022dd004(void) {
    UnkStruct_022dd004_3 *p = gTaskCurrent->unk10->unk20C;
    p->unk24 = data_ov000_02369b2c->unkAF8;
    Model_Animate(p);
    Model_Draw(p);
}
