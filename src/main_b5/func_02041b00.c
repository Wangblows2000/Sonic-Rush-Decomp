#include <nitro/types.h>

typedef struct {
    u8 b[6];
} UnkMac_02041b00;

typedef struct {
    void (*fn)(void);
    u8 pad4[0x16];
    u16 unk1a;
} UnkWork_02041b00;

extern UnkMac_02041b00 data_02078b0c;
extern u8 data_022c4380[];

u32 func_0203e934(void);
void func_0203e990(void *p, u32 size);
void func_0203f900(void (*fn)(void), UnkMac_02041b00 *mac, s32 a);
void func_02042404(void);
void func_02041a54(void);

void func_02041b00(UnkWork_02041b00 *w) {
    UnkMac_02041b00 mac;

    if (func_0203e934() != 1)
        return;
    mac = data_02078b0c;
    func_0203e990(data_022c4380, 0x18);
    func_0203f900(func_02042404, &mac, 0);
    w->unk1a = 0;
    w->fn = func_02041a54;
}
