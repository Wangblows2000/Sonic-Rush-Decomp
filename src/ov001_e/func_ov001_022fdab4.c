#include <nitro/types.h>

typedef struct UnkStruct_022fdab4_a {
    u8 data[0x5c];
} UnkStruct_022fdab4_a;

extern void *data_ov001_0231f528;
extern u8 data_ov001_0231d9ac[];
extern void *data_ov001_0231f320;

void func_02021310(UnkStruct_022fdab4_a *arc, void *name, void *narc);
void *func_02021264(UnkStruct_022fdab4_a *arc, s32 id);
void func_020212e0(UnkStruct_022fdab4_a *arc);

void func_ov001_022fdab4(void) {
    UnkStruct_022fdab4_a arc;

    func_02021310(&arc, data_ov001_0231d9ac, data_ov001_0231f528);
    data_ov001_0231f320 = func_02021264(&arc, 0x26);
    func_020212e0(&arc);
}
