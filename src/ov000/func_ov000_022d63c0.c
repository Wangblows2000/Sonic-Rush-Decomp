#include <nitro/types.h>

typedef struct UnkStruct_022d63c0 {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad1[0x2c0 - 0x6c];
    void *unk2C0;
} UnkStruct_022d63c0;

void func_0201c070(void *p, s32 a);
void func_0201bf1c(void *p);
void func_ov000_022d63bc(void);

void func_ov000_022d63c0(UnkStruct_022d63c0 *s) {
    func_0201c070(s->unk2C0, 0x20);
    func_0201bf1c(s->unk2C0);
    s->func = func_ov000_022d63bc;
}
