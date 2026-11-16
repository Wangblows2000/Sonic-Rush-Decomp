#include <nitro/types.h>

typedef struct {
    u8 pad0[0x68];
    void *unk68;
} UnkStruct_022edc24;

extern s32 data_022c4560;
extern u16 data_022c4da0;

void func_02060f40(void);
void func_ov000_022edbec(UnkStruct_022edc24 *p);

void func_ov000_022edc24(UnkStruct_022edc24 *p) {
    if (data_022c4560 == 0)
        data_022c4da0 = 0xa;
    else
        data_022c4da0 = 0xb;
    func_02060f40();
    p->unk68 = func_ov000_022edbec;
}
