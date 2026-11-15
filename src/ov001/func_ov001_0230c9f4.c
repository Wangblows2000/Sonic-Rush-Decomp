#include <nitro/types.h>

typedef struct {
    s32 w0;
    u8 pad4[0xc];
    s32 w10;
    s32 w14;
    s32 w18;
    u8 b1c;
    u8 b1d;
    u16 h1e;
} UnkStruct_022c4560;
extern UnkStruct_022c4560 data_022c4560;
extern s32 data_ov001_0231f698;
void func_ov001_0230c364(s32 a);

void func_ov001_0230c9f4(void) {
    data_ov001_0231f698 = 0;
    func_ov001_0230c364(data_022c4560.b1c);
}
