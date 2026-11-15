#include <nitro/types.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
void func_0202076c(void *list, void *obj);
extern u8 data_02083f50[];

void func_0201f904(void *obj) {
    u32 e = OS_DisableInterrupts();
    func_0202076c(data_02083f50, obj);
    OS_RestoreInterrupts(e);
}
