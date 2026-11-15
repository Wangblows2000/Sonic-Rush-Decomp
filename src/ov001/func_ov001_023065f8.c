#include <nitro/types.h>

typedef struct { u8 data[0x5c]; } UnkStruct_023029d4;
extern void *data_ov001_0231f528;
extern u8 data_ov001_0231df74[];
extern void *data_ov001_0231f5ac;
extern void *data_ov001_0231f5b0[];
void func_02021310(UnkStruct_023029d4 *a, void *b, void *c);
void *func_02021264(UnkStruct_023029d4 *a, s32 i);
void func_020212e0(UnkStruct_023029d4 *a);

void func_ov001_023065f8(void) {
    UnkStruct_023029d4 arc;
    func_02021310(&arc, data_ov001_0231df74, data_ov001_0231f528);
    data_ov001_0231f5ac = func_02021264(&arc, 0x21);
    data_ov001_0231f5b0[0] = func_02021264(&arc, 0x22);
    data_ov001_0231f5b0[1] = func_02021264(&arc, 0x23);
    func_020212e0(&arc);
}
