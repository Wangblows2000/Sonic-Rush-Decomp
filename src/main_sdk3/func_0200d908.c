#include <nitro/types.h>

typedef struct {
    void *callback;   /* 0x00 */
    u8 pad4[8];
    u16 fc;           /* 0x0c */
    u8 pade[2];
    u32 f10;          /* 0x10 */
    u8 pad14[0x1c];
    u16 f30;          /* 0x30 */
    u16 f32;          /* 0x32 */
    u16 f34;          /* 0x34 */
    u16 f36;          /* 0x36 */
} UnkTPWork_0200d908;

extern u16 data_0207f028;
extern UnkTPWork_0200d908 data_0207f02c;

extern void func_02009fa8(void);
extern BOOL PXI_IsCallbackReady(u32 fifotag, u32 proc);
extern void func_0200a1b4(u32 fifotag, void (*callback)(u32, u32, BOOL));
extern void func_0200d998(u32 tag, u32 data, BOOL err);

void func_0200d908(void)
{
    if (data_0207f028 != 0) {
        return;
    }
    data_0207f028 = 1;
    func_02009fa8();
    data_0207f02c.f32 = 0;
    data_0207f02c.f36 = 0;
    data_0207f02c.fc = 0;
    data_0207f02c.callback = NULL;
    data_0207f02c.f10 = 0;
    data_0207f02c.f30 = 0;
    data_0207f02c.f34 = 0;
    while (!PXI_IsCallbackReady(6, 1)) {
    }
    func_0200a1b4(6, func_0200d998);
}
