#include <nitro/types.h>

typedef struct UnkStruct_0232a764_2 {
    u8 pad[0xc8];
    s32 unkC8;
    u8 pad2[0x110 - 0xcc];
    s32 unk110;
} UnkStruct_0232a764_2;

typedef struct UnkStruct_0232a764 {
    u8 pad[0x10];
    UnkStruct_0232a764_2 *unk10;
} UnkStruct_0232a764;

void StageObj_Destructor(UnkStruct_0232a764 *s);

void func_ov000_0232a764(UnkStruct_0232a764 *s) {
    UnkStruct_0232a764_2 *p = s->unk10;
    p->unkC8 = 0;
    p->unk110 = 0;
    StageObj_Destructor(s);
}
