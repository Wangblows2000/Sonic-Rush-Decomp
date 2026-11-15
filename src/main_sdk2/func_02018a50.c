#include <nitro/types.h>
#include <main_sdk2/wh.h>

extern void *data_02080f14;
extern u16 data_02080f10;

void func_0201853c(u32 a);
s32 func_02011b50(void *buf, void (*cb)(UnkCallback_wh *), u16 dma);
s32 func_0201154c(void (*cb)(UnkCallback_wh *));
void func_0201a014(void);

s32 func_02018a50(void)
{
    s32 r;

    data_02080f18->unk528 = 0;
    data_02080f18->unk52a = 0;
    data_02080f18->unk526 = 0;
    data_02080f18->unk548 = 0;
    func_0201853c(0xa);
    if (data_02080f1c->unk1320 == 0) {
        do {
            r = func_02011b50(data_02080f14, data_02080f18->unk508, data_02080f10);
        } while (r == 4);
        if (r != 2) {
            return 8;
        }
        func_0201154c(data_02080f18->unk508);
        data_02080f18->unk50d = 1;
        return 0;
    }
    func_0201154c(data_02080f18->unk508);
    data_02080f18->unk50d = 1;
    func_0201a014();
    return 0;
}
