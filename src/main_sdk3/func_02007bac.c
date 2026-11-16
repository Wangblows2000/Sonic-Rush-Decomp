#include <nitro/types.h>

void func_02007994(u32 dmaNo, u32 src, u32 size, u32 dir);
void func_02007b3c(u32 dmaNo);
void func_02005448(u32 dmaNo, void (*callback)(void *), void *arg);
void func_01ff85cc(u32 dmaNo, u32 src, u32 dest, u32 ctrl);

void func_02007bac(u32 dmaNo, const void *src, void *dest, u32 size, void (*callback)(void *), void *arg)
{
    func_02007994(dmaNo, (u32)src, size, 0);
    if (size == 0) {
        if (callback) {
            callback(arg);
        }
    } else {
        func_02007b3c(dmaNo);
        if (callback) {
            func_02005448(dmaNo, callback, arg);
            func_01ff85cc(dmaNo, (u32)src, (u32)dest, 0xc4000000 | (size >> 2));
        } else {
            func_01ff85cc(dmaNo, (u32)src, (u32)dest, 0x84000000 | (size >> 2));
        }
    }
}
