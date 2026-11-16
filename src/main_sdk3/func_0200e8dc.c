#include <nitro/types.h>

typedef struct {
    u32 f0;
    u32 f4;
} UnkWork_0200e8dc;

typedef struct {
    u16 f0;
    u8 pad2[6];
} UnkEntry_0200e8dc;

extern u16 data_0207f084;
extern UnkWork_0200e8dc data_0207f09c;
extern UnkEntry_0200e8dc data_0207f0c0[4];
extern u8 data_0207f0ac[];
extern u32 data_0207f094;

extern void func_02009fa8(void);
extern BOOL PXI_IsCallbackReady(u32 fifotag, u32 proc);
extern void func_0200a1b4(u32 fifotag, void (*callback)(u32, u32, BOOL));
extern void func_0200e7f0(u32 tag, u32 data, BOOL err);
extern void func_02006724(void *p);

void func_0200e8dc(void)
{
    s32 i;
    if (data_0207f084 != 0) {
        return;
    }
    data_0207f084 = 1;
    data_0207f09c.f0 = 0;
    data_0207f09c.f4 = 0;
    func_02009fa8();
    while (!PXI_IsCallbackReady(8, 1)) {
    }
    func_0200a1b4(8, func_0200e7f0);
    for (i = 0; i < 4; i++) {
        data_0207f0c0[i].f0 = 0;
    }
    func_02006724(data_0207f0ac);
    data_0207f094 = *(u32 *)0x027ffc3c;
}
