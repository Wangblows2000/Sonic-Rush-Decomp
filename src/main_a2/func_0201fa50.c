#include <nitro/types.h>

void func_02005d5c(void *thread, void (*func)(u8 *), void *arg, void *stack, u32 size, u32 prio);
void func_0201ec94(u8 *p);
void NNS_FndInitList(void *list, u16 offset);
void func_02006724(void *m);
void func_02005ba0(void *thread);

void func_0201fa50(u8 *p, u32 prio) {
    func_02005d5c(p, func_0201ec94, p, p + 0x494, 0x400, prio);
    NNS_FndInitList(p + 0x4AC, 0);
    func_02006724(p + 0x498);
    *(u16 *)(p + 0x494) = 0;
    func_02005ba0(p);
}
