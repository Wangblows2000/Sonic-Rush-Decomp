#include <nitro/types.h>

typedef struct { u8 unk0[0x10C]; s32 unk10C_0 : 1; u8 unk110[0x4C]; } UnkStruct_020201bc;
extern UnkStruct_020201bc data_020847b8[];
void func_0201fc60(UnkStruct_020201bc *s, u32 a);

void func_020201bc(u32 a) {
    s32 i;
    for (i = 0; i < 4; i++) {
        if (data_020847b8[i].unk10C_0) {
            func_0201fc60(&data_020847b8[i], a);
        }
    }
}
