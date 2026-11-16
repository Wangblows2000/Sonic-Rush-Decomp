#include <nitro/types.h>

typedef struct UnkNode_02034174 {
    struct UnkNode_02034174 *prev;
    struct UnkNode_02034174 *next;
    u8 unk8[0x5c];
} UnkNode_02034174;

extern u32 data_0208f7e8;
extern UnkNode_02034174 *data_0208f7f0[];
extern UnkNode_02034174 data_0208f830[];
extern UnkNode_02034174 *data_0208f7ec;
extern UnkNode_02034174 *data_0208f7e4;

void func_0200be98(s32 arg);
void MIi_CpuClear32(u32 data, void *dst, u32 size);
UnkNode_02034174 *func_02033db0(void);

void func_02034174(void) {
    s32 i;
    func_0200be98(-1);
    {
        vu32 zero = 0;
        data_0208f7e8 = 0;
        MIi_CpuClear32(zero, data_0208f830, sizeof(UnkNode_02034174) * 16);
    }
    for (i = 0; i < 16; i++) {
        data_0208f7f0[i] = &data_0208f830[i];
    }
    data_0208f7ec = func_02033db0();
    data_0208f7e4 = func_02033db0();
    data_0208f7ec->prev = NULL;
    data_0208f7ec->next = data_0208f7e4;
    data_0208f7e4->prev = data_0208f7ec;
    data_0208f7e4->next = NULL;
}
