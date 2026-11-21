#include <nitro/types.h>
#include <main_sdk2/wh.h>

#define ENTRY_02015728(k) (((UnkStruct_02080f1c *)((u8 *)data_02080f1c + (k) * sizeof(UnkEntry1788_wh)))->unk1788[0])

void MBi_calc_sendblock(s32 k)
{
    u16 a;
    u16 b;
    if (ENTRY_02015728(k).unk5ca == 0) {
        return;
    }
    if (ENTRY_02015728(k).unk5c4 == 0) {
        return;
    }
    a = ENTRY_02015728(k).unk5c0;
    b = ENTRY_02015728(k).unk5c2;
    if (b <= a && a <= b + 2) {
        ENTRY_02015728(k).unk5c0 = a + 1;
    } else {
        ENTRY_02015728(k).unk5c0 = b;
    }
}
