#include <nitro/types.h>

typedef struct UnkStruct_022cc3e8 {
    u8 pad[4];
    s32 unk4;
    s32 unk8;
} UnkStruct_022cc3e8;

extern UnkStruct_022cc3e8 data_ov000_02369ad0;
extern u16 data_ov000_02362c14[][4];
extern u16 data_ov000_02362ba4[][4];
extern u16 data_ov000_02362bf4[][4];
extern u16 data_ov000_02362c34[][4];
extern u16 data_ov000_02362c04[][4];
extern u16 data_ov000_02362c44[][4];
extern u16 data_ov000_02362bd4[][4];
extern u16 data_ov000_02362c24[][4];
extern u16 data_ov000_02362b74[];
extern u16 data_ov000_02362b7c[];
extern u32 data_ov000_02364f9c[];
extern u32 data_ov000_02364fac[];

BOOL func_ov000_022cc6d4(void);
void func_ov000_022c5d4c(s32 a, u32 *p1, u16 *p2, u16 *p3, u16 n);

void func_ov000_022cc3e8(s32 idx, s32 flag) {
    u16 *t1;
    u16 *t2;
    if (flag) {
        if (func_ov000_022cc6d4()) {
            t1 = data_ov000_02362c14[idx];
            t2 = data_ov000_02362ba4[idx];
        } else {
            t1 = data_ov000_02362bf4[idx];
            t2 = data_ov000_02362c34[idx];
        }
    } else {
        if (func_ov000_022cc6d4()) {
            t1 = data_ov000_02362c04[idx];
            t2 = data_ov000_02362c44[idx];
        } else {
            t1 = data_ov000_02362bd4[idx];
            t2 = data_ov000_02362c24[idx];
        }
    }
    func_ov000_022c5d4c(data_ov000_02369ad0.unk4, data_ov000_02364f9c, t1, data_ov000_02362b74, 4);
    func_ov000_022c5d4c(data_ov000_02369ad0.unk8, data_ov000_02364fac, t2, data_ov000_02362b7c, 4);
}
