#include <nitro/types.h>

typedef struct { u8 pad[0x88]; void *res[2]; } UnkStruct_022cf90c;
extern void *data_ov001_0231abe0[];
void *File_Load(void *p, s32 v);

#pragma thumb on
void func_ov001_022cf90c(UnkStruct_022cf90c *p) {
    s32 i;
    for (i = 0; i < 2; i++) {
        p->res[i] = File_Load(data_ov001_0231abe0[i], 0);
    }
}
#pragma thumb reset
