#include <nitro/types.h>

typedef struct {
    u32 f0;
    u32 f4;
} UnkWork_0200dda8;

typedef struct {
    u16 f0;
    u8 pad2[6];
} UnkEntry_0200dda8;

extern u16 data_0207f064;
extern UnkWork_0200dda8 data_0207f068;
extern UnkEntry_0200dda8 data_0207f0c0[4];
extern u8 data_0207f0ac[];
extern u32 data_0207f094;

extern void func_02009fa8(void);
extern BOOL PXI_IsCallbackReady(u32 fifotag, u32 proc);
extern void func_0200a1b4(u32 fifotag, void (*callback)(u32, u32, BOOL));
extern void func_0200dc58(u32 tag, u32 data, BOOL err);
extern void func_02006724(void *p);

void func_0200dda8(void)
{
    if (data_0207f064 != 0) {
        return;
    }
    data_0207f064 = 1;
    data_0207f068.f0 = 0;
    data_0207f068.f4 = 0;
    func_02009fa8();
    while (!PXI_IsCallbackReady(9, 1)) {
    }
    *(u32 *)0x027fff90 = 0;
    func_0200a1b4(9, func_0200dc58);
}
