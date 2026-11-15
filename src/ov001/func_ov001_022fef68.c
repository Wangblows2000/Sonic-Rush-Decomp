#include <nitro/types.h>

typedef struct { u32 flags; s32 v4; u8 pad[0x10]; u16 h18; u16 h1a; u16 h1c; u16 h1e; u16 h20; } UnkStruct_0231f52c;
extern UnkStruct_0231f52c *data_ov001_0231f52c;
void func_ov001_022fea98(void);
void func_ov001_02304e80(void);
void func_ov001_022f7224(s32 a);
void func_ov001_02300db4(void);
void func_ov001_022ffcf8(void);
void func_ov001_022ffe04(void);

void func_ov001_022fef68(void) {
    data_ov001_0231f52c->h20 = 0;
    data_ov001_0231f52c->h1e = data_ov001_0231f52c->h20;
    data_ov001_0231f52c->h1c = data_ov001_0231f52c->h1e;
    data_ov001_0231f52c->h1a = data_ov001_0231f52c->h1c;
    data_ov001_0231f52c->h18 = data_ov001_0231f52c->h1a;
    func_ov001_022fea98();
    func_ov001_02304e80();
    func_ov001_022f7224(data_ov001_0231f52c->v4);
    func_ov001_02300db4();
    if (data_ov001_0231f52c->flags & 0x400) {
        data_ov001_0231f52c->flags &= ~0x400;
        func_ov001_022ffcf8();
        func_ov001_022ffe04();
    }
}
