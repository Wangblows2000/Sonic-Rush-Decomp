#pragma opt_propagation off
#include <nitro/types.h>

extern u32 data_ov000_02369ac0;
extern u16 data_02074990[];
extern u16 data_ov000_02364dbc[];
void func_02022270(s32 a, u16 b);

void func_ov000_0233dfdc(u16 *cnt) {
    if (!(data_ov000_02369ac0 & 1))
        return;
    (*cnt)++;
    if ((*cnt & ~0x8000) <= 6)
        return;
    if (*cnt & 0x8000) {
        u16 *p = data_02074990;
        func_02022270(0, p[0]);
        func_02022270(1, p[1]);
        func_02022270(2, p[2]);
    } else {
        func_02022270(0, data_ov000_02364dbc[0]);
        func_02022270(1, data_ov000_02364dbc[1]);
        func_02022270(2, data_ov000_02364dbc[2]);
    }
    *cnt &= 0x8000;
    *cnt ^= 0x8000;
}
