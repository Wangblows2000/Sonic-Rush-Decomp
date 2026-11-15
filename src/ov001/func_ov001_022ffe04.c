#include <nitro/types.h>

typedef struct { u8 pad[0x3c]; s32 v3c; s32 v40; s32 v44; s32 v48; } UnkStruct_0231f52c;
extern UnkStruct_0231f52c *data_ov001_0231f52c;
void func_020367b4(s32 a, s32 b);
s32 func_0200203c(s32 a, s32 b);
void func_ov001_02302be8(s32 a);
void func_ov001_0230582c(void);
void func_02048fdc(s32 a, s32 b, s32 c, s32 d, s32 e);
void func_ov001_022ffeb0(void);

void func_ov001_022ffe04(void) {
    data_ov001_0231f52c->v40++;
    if (data_ov001_0231f52c->v40 == 3) {
        func_020367b4(0, 0x15);
    }
    data_ov001_0231f52c->v3c = func_0200203c(data_ov001_0231f52c->v40, 2);
    func_ov001_02302be8(data_ov001_0231f52c->v3c);
    func_ov001_0230582c();
    func_02048fdc(0x10, 0x10, 0, 3, 0);
    func_ov001_022ffeb0();
    if (data_ov001_0231f52c->v3c == 0) {
        data_ov001_0231f52c->v48 = data_ov001_0231f52c->v44;
    }
}
