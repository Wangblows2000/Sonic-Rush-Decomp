#include <nitro/types.h>

typedef struct UnkStruct_022c7514 UnkStruct_022c7514;
struct UnkStruct_022c7514 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad0[0x68 - 0x1c];
    void (*func)(UnkStruct_022c7514 *s);
    u8 pad1[0x224 - 0x6c];
    s32 unk224;
    u8 unk228[4];
};

void func_ov000_022cc00c(void *p, s32 a, s32 b, s32 c, s32 d);
void func_ov000_022cc224(void);
void func_020366f4(s32 a, s32 b, s32 c);
void func_ov000_022c74fc(UnkStruct_022c7514 *s);

void func_ov000_022c7514(UnkStruct_022c7514 *s) {
    func_ov000_022cc00c(s->unk228, s->unk224, 5, 0x1000, 0);
    s->unk10 = 0xe100;
    s->unk14 = 0x20000;
    s->unk18 = -0x8e00;
    func_ov000_022cc224();
    func_020366f4(0, 0, 0x7e);
    s->func = func_ov000_022c74fc;
}
