#include <nitro/types.h>
#include <main_sdk2/wh.h>

#define ENTRY_02015e6c(k) (((UnkStruct_02080f1c *)((u8 *)data_02080f1c + (k) * sizeof(UnkEntry1788_wh)))->unk1788[0])

u16 *WM_ReadMPData(void *hdr, u16 aid);
void MBi_CommParentRecvDataPerChild(const u16 *data, u16 aid);

void func_02015e6c(void *hdr)
{
    u16 i;
    for (i = 0; i < 16; i++) {
        if (ENTRY_02015e6c(i).unk5ca != 0) {
            ENTRY_02015e6c(i).unk5c2 = 0;
        }
    }
    for (i = 1; i <= 15; i++) {
        u16 *d = WM_ReadMPData(hdr, i);
        if (d != NULL && *d != 0xffff && *d != 0) {
            MBi_CommParentRecvDataPerChild(d, i);
        }
    }
}
