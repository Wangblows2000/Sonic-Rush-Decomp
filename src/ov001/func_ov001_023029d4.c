#include <nitro/types.h>

typedef struct { u8 data[0x5c]; } UnkStruct_023029d4;
extern void *data_ov001_0231f528;
extern u8 data_ov001_0231df18[];
extern void *data_ov001_0231f534;
extern void *data_ov001_0231f53c;
void func_02021310(UnkStruct_023029d4 *a, void *b, void *c);
void *func_02021264(UnkStruct_023029d4 *a, s32 i);
void func_020212e0(UnkStruct_023029d4 *a);

void func_ov001_023029d4(void) {
    UnkStruct_023029d4 arc;
    func_02021310(&arc, data_ov001_0231df18, data_ov001_0231f528);
    data_ov001_0231f534 = func_02021264(&arc, 0x24);
    data_ov001_0231f53c = func_02021264(&arc, 0x25);
    func_020212e0(&arc);
}
