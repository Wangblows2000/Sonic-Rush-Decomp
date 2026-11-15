#include <nitro/types.h>

typedef struct UnkStruct_0232b870_2 {
    u8 pad[0xc8];
    s32 unkC8;
    u8 pad2[0x110 - 0xcc];
    s32 unk110;
} UnkStruct_0232b870_2;

typedef struct UnkStruct_0232b870 {
    u8 pad[0x10];
    UnkStruct_0232b870_2 *unk10;
} UnkStruct_0232b870;

extern s32 data_ov000_0236a980;

void func_0206bef4(UnkStruct_0232b870 *s);

void func_ov000_0232b870(UnkStruct_0232b870 *s) {
    UnkStruct_0232b870_2 *p = s->unk10;
    data_ov000_0236a980 = 0;
    p->unkC8 = 0;
    p->unk110 = 0;
    func_0206bef4(s);
}
