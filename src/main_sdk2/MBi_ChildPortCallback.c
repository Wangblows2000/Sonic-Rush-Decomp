#include <nitro/types.h>
#include <main_sdk2/wh.h>

void MBi_ChildPortCallback(UnkCallback_wh *arg)
{
    if (arg->errcode != 0) {
        return;
    }
    switch (arg->state4) {
    case 7:
        break;
    case 9:
        break;
    case 0x15:
        data_02080f18->callback(9, arg);
        break;
    }
}
