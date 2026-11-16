#include <nitro/types.h>

typedef struct UnkStruct_02304518_a {
    u8 data[0x5c];
} UnkStruct_02304518_a;

extern void *data_ov001_0231f528;
extern u8 data_ov001_0231df1c[];
extern void *data_ov001_0231f544;

void func_02021310(UnkStruct_02304518_a *arc, void *name, void *narc);
void *func_02021264(UnkStruct_02304518_a *arc, s32 id);
void func_020212e0(UnkStruct_02304518_a *arc);

void func_ov001_02304518(void) {
    UnkStruct_02304518_a arc;

    func_02021310(&arc, data_ov001_0231df1c, data_ov001_0231f528);
    data_ov001_0231f544 = func_02021264(&arc, 0x27);
    func_020212e0(&arc);
}
