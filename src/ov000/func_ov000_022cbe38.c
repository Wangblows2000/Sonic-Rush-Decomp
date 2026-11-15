#include <nitro/types.h>

typedef struct UnkStruct_022cbe38 {
    u8 pad[0x210];
    void *unk210;
    void *unk214;
    u8 pad1[0x22a - 0x218];
    u16 unk22A;
} UnkStruct_022cbe38;

extern UnkStruct_022cbe38 *data_ov000_02369acc;

void func_ov000_022cafa4(UnkStruct_022cbe38 *s);
void func_ov000_022c7f1c(void *p);
void func_ov000_022c5f94(void *p);

void func_ov000_022cbe38(u16 v) {
    data_ov000_02369acc->unk22A = v;
    func_ov000_022cafa4(data_ov000_02369acc);
    func_ov000_022c7f1c(data_ov000_02369acc->unk210);
    func_ov000_022c5f94(data_ov000_02369acc->unk214);
}
