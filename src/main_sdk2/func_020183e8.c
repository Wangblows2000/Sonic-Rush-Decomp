#include <nitro/types.h>
#include <main_sdk2/wh.h>

void MBi_ChildCallback(UnkCallback_wh *arg);
s32 MBi_SetMPData(void (*cb)(UnkCallback_wh *), void *data, u16 size, u16 timeout, u16 port);

s32 func_020183e8(void *data, s32 size_, s32 port_)
{
    UnkWork_wh *work = data_02080f18;
    u16 size = size_;
    u16 port = port_;
    s32 r;

    if (work->unk528 == 0 || work->unk526 == 1) {
        return 1;
    }
    switch (work->unk524) {
    case 1:
        r = MBi_SetMPData(work->unk508, data, size, work->unk52c == 0 ? 1000 : 0, port);
        if (r == 2) {
            data_02080f18->unk50c = 1;
        }
        return r == 2 ? 0 : r;
    case 2:
        r = MBi_SetMPData(MBi_ChildCallback, data, size, 0, port);
        if (r == 2) {
            data_02080f18->unk50c = 1;
        }
        return r == 2 ? 0 : r;
    }
    return 1;
}
