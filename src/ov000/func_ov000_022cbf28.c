#include <nitro/types.h>

typedef struct UnkStruct_022cbf28 {
    u8 pad[0x210];
    void *unk210;
    void *unk214;
    u8 pad1[0x22a - 0x218];
    u16 unk22A;
} UnkStruct_022cbf28;

extern UnkStruct_022cbf28 *data_ov000_02369acc;

void func_ov000_022cb088(UnkStruct_022cbf28 *s);
void func_ov000_022c95a8(void *p);
void func_ov000_022c6944(void *p);

void func_ov000_022cbf28(u16 v) {
    data_ov000_02369acc->unk22A = v;
    func_ov000_022cb088(data_ov000_02369acc);
    func_ov000_022c95a8(data_ov000_02369acc->unk210);
    func_ov000_022c6944(data_ov000_02369acc->unk214);
}
