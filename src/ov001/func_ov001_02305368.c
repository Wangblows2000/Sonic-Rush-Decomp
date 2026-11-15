#include <nitro/types.h>

typedef struct { u8 data[0x5c]; } UnkStruct_023029d4;
extern void *data_ov001_0231f528;
extern u8 data_ov001_0231df20[];
extern void *data_ov001_0231f550[];
void func_02021310(UnkStruct_023029d4 *a, void *b, void *c);
void *func_02021264(UnkStruct_023029d4 *a, s32 i);
void func_020212e0(UnkStruct_023029d4 *a);

void func_ov001_02305368(void) {
    UnkStruct_023029d4 arc;
    s32 i;
    func_02021310(&arc, data_ov001_0231df20, data_ov001_0231f528);
    for (i = 0; i < 0x16; i++) {
        data_ov001_0231f550[i] = func_02021264(&arc, i + 0xb);
    }
    func_020212e0(&arc);
}
