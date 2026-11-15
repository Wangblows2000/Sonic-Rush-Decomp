#include <nitro/types.h>

typedef struct { u32 flags; u8 pad[0x18]; } UnkStruct_0231d6b4;
typedef struct { s32 id; void *anim[5]; } UnkStruct_0231f098;
extern UnkStruct_0231d6b4 data_ov001_0231d6b4[];
extern UnkStruct_0231f098 data_ov001_0231f098[];
void func_0202566c(s32 id);

void func_ov001_022fd500(void) {
    s32 i, j;
    for (i = 0; i < 0x1b; i++) {
        if (data_ov001_0231d6b4[i].flags & 1) {
            func_0202566c(data_ov001_0231f098[i].id);
        }
        for (j = 0; j < 6; j++) {
            ((s32 *)&data_ov001_0231f098[i])[j] = 0;
        }
    }
}
