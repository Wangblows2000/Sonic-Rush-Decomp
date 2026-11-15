#include <nitro/types.h>

typedef struct { u8 pad[0xc]; u8 *p; u8 pad2[0x14]; s32 v; } UnkStruct_0230c0e0;
extern UnkStruct_0230c0e0 *data_ov001_0231f5c0;
void func_ov001_0230bc70(void);

void func_ov001_0230c0e0(s32 v) {
    UnkStruct_0230c0e0 *t = data_ov001_0231f5c0;
    t->v = v;
    *(void (**)(void))(t->p + 8) = func_ov001_0230bc70;
}
