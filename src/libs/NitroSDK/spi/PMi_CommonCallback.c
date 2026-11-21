#include <nitro/types.h>

typedef struct {
    u16 flag;
    u8 pad2[2];
    u16 *ptr;
} UnkEntry_0200e7f0;

typedef struct {
    u8 pad0[0xc];
    u32 *ptr;
} UnkWork_0200e7f0;

extern UnkEntry_0200e7f0 data_0207f0c0[];
extern u32 data_0207f088;
extern u32 data_0207f08c;
extern UnkWork_0200e7f0 data_0207f09c;

void PMi_CallCallbackAndUnlock(u32 result);

void PMi_CommonCallback(u32 tag, u32 data, BOOL err)
{
    u16 command;
    u16 result;

    if (err) {
        PMi_CallCallbackAndUnlock(2);
        return;
    }
    command = (u16)((data & 0x7f00) >> 8);
    result = (u16)(data & 0xff);
    if (command >= 0x70 && command <= 0x73) {
        u32 idx = command - 0x70;
        u16 *p = data_0207f0c0[idx].ptr;
        u16 v = (u16)(result & 0xff);
        if (p != NULL) {
            *p = v;
        }
        data_0207f0c0[idx].flag = 1;
        result = 0;
    } else if (command == 0x60) {
        data_0207f088 = 1;
    } else if (command == 0x62) {
        data_0207f08c = 1;
    } else if (command == 0x67) {
        if (data_0207f09c.ptr != NULL) {
            *data_0207f09c.ptr = result;
        }
        result = 0;
    }
    PMi_CallCallbackAndUnlock(result);
}
