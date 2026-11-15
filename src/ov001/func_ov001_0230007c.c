#include <nitro/types.h>

typedef struct { u32 flags; s32 v4; u8 pad[0xc]; void *task; } UnkStruct_0231f52c;
extern UnkStruct_0231f52c *data_ov001_0231f52c;
void func_0203a43c(s32 a);
void func_0203a288(s32 a, s32 b, s32 c, s32 d, s32 e);
void GXS_SetGraphicsMode(s32 a);
void func_020371c8(void *p);

void func_ov001_0230007c(void) {
    func_0203a43c(0x80);
    func_0203a288(0x100, 0x100010, 0x40, 0, 0x100);
    *(vu16 *)0x0400100c = (*(vu16 *)0x0400100c & 0x43) | 0x404;
    GXS_SetGraphicsMode(0);
    data_ov001_0231f52c->v4 = 0;
    if (data_ov001_0231f52c->task != NULL) {
        func_020371c8(data_ov001_0231f52c->task);
        data_ov001_0231f52c->task = NULL;
    }
    data_ov001_0231f52c->flags &= ~1;
}
