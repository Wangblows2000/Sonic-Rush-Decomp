#include <nitro/types.h>

typedef struct {
    u32 f0;
    u32 f4;
    u32 f8;
    u32 fc;
    u8 pad10[0xc];
    u32 f1c;
} UnkWork_0200f160;

extern u16 data_0207f0e0;
extern UnkWork_0200f160 data_0207f0e4;

extern void func_02009fa8(void);
extern BOOL PXI_IsCallbackReady(u32 fifotag, u32 proc);
extern void func_0200a1b4(u32 fifotag, void (*callback)(u32, u32, BOOL));
extern void func_0200eb10(u32 tag, u32 data, BOOL err);

void func_0200f160(void)
{
    if (data_0207f0e0 != 0) {
        return;
    }
    data_0207f0e0 = 1;
    data_0207f0e4.f0 = 0;
    data_0207f0e4.f4 = 0;
    data_0207f0e4.f1c = 0;
    data_0207f0e4.f8 = 0;
    data_0207f0e4.fc = 0;
    func_02009fa8();
    while (!PXI_IsCallbackReady(5, 1)) {
    }
    func_0200a1b4(5, func_0200eb10);
}
