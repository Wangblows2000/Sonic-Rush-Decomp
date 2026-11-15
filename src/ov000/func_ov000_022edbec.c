#include <nitro/types.h>

typedef struct UnkStruct_022edbec {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad1[0x230 - 0x6c];
    u16 unk230;
} UnkStruct_022edbec;

extern u8 data_02250464;

void func_0201c070(void *p, s32 a);
void func_ov000_022edbb8(void);

void func_ov000_022edbec(UnkStruct_022edbec *s) {
    s->unk230 = 0;
    func_0201c070(&data_02250464, 0x3c);
    s->func = func_ov000_022edbb8;
}
