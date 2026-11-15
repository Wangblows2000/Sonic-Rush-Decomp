#include <nitro/types.h>

extern u32 data_0208f7e0;
extern u32 data_0208f7dc;
void func_020337a4(u32 a, u32 b, u32 c);
void func_0203383c(void);

void func_020321e4(u32 a, u32 b, u32 c) {
    u32 head = data_0208f7e0;
    u32 tail = data_0208f7dc;
    data_0208f7e0 = 0;
    data_0208f7dc = 0;
    func_020337a4(a, b, c);
    func_0203383c();
    data_0208f7e0 = head;
    data_0208f7dc = tail;
}
