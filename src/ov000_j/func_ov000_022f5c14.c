#include <nitro/types.h>

typedef struct {
    u8 pad0[0x14];
    s32 unk14;
    u8 pad18[0x68 - 0x18];
    void *unk68;
    u8 pad6C[0x214 - 0x6c];
    u16 unk214;
} UnkStruct_022f5c14;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_022c4560;

extern UnkStruct_022c4560 data_022c4560;
extern s32 data_ov000_02364198[][10];

void func_ov000_022f5b6c(UnkStruct_022f5c14 *p);

void func_ov000_022f5c14(UnkStruct_022f5c14 *p) {
    BOOL b;
    s32 idx;

    b = data_022c4560.unk10 == 2 || data_022c4560.unk10 == 3;
    if (b)
        idx = 1;
    else
        idx = data_022c4560.unk14;
    if (data_ov000_02364198[idx][p->unk214] > p->unk14)
        p->unk68 = func_ov000_022f5b6c;
}
