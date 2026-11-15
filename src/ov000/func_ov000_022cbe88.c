#include <nitro/types.h>

typedef struct UnkStruct_022cbe88 {
    u8 pad[0x210];
    void *unk210;
    void *unk214;
    u8 pad1[0x22a - 0x218];
    u16 unk22A;
} UnkStruct_022cbe88;

extern UnkStruct_022cbe88 *data_ov000_02369acc;

void func_ov000_022caff0(UnkStruct_022cbe88 *s);
void func_ov000_022c8940(void *p);
void func_ov000_022c5fa8(void *p);

void func_ov000_022cbe88(u16 v) {
    data_ov000_02369acc->unk22A = v;
    func_ov000_022caff0(data_ov000_02369acc);
    func_ov000_022c8940(data_ov000_02369acc->unk210);
    func_ov000_022c5fa8(data_ov000_02369acc->unk214);
}
