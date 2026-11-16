#include <nitro/types.h>

typedef struct {
    u32 unk0;
} UnkStruct_0236aad8;

typedef struct {
    u8 pad0[4];
    u8 unk4;
    u8 pad5[0x68 - 5];
    void *unk68;
} UnkStruct_02304184;

extern UnkStruct_0236aad8 data_ov000_0236aad8;

void func_ov000_023040fc(UnkStruct_02304184 *p);

void func_ov000_02304184(UnkStruct_02304184 *p) {
    if (p->unk4 == 0) {
        p->unk4 = 1;
        data_ov000_0236aad8.unk0 |= 0x4000;
    }
    p->unk68 = func_ov000_023040fc;
}
