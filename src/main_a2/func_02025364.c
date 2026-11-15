#include <nitro/types.h>

extern u32 data_02077964;
extern volatile u32 data_02087ca4;
extern u32 data_02087ca8;

void func_02025268(u32 op, const u32 *args, u32 n);
void func_02025454(void);
void func_02025430(void *arg);
void func_02007fd8(u32 dma, const void *src, u32 size, void (*cb)(void *), void *arg);
void func_020081d4(u32 dma, const void *src, u32 size, void (*cb)(void *), void *arg);

void func_02025364(const u32 *data, u32 size) {
    if (size < 0x100 || data_02077964 == (u32)-1) {
        func_02025268(data[0], data + 1, (size >> 2) - 1);
        return;
    }
    func_02025454();
    data_02087ca4 = 1;
    if (data_02087ca8 != 0) {
        func_02007fd8(data_02077964, data, size, func_02025430, (void *)&data_02087ca4);
    } else {
        func_020081d4(data_02077964, data, size, func_02025430, (void *)&data_02087ca4);
    }
}
