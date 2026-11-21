#include <nitro/types.h>

typedef struct {
    u32 busy;
    u32 callback;
    u32 arg;
} UnkStruct_0207f09c;

typedef struct {
    u16 flag;
    u16 pad;
    u32 value;
} UnkStruct_0207f0c0;

extern UnkStruct_0207f09c data_0207f09c;
extern UnkStruct_0207f0c0 data_0207f0c0[];

BOOL PMi_Lock(void);
void PMi_SendPxiData(u32 cmd);

u32 PMi_ReadRegisterAsync(u32 idx, u32 value, u32 callback, u32 arg)
{
    u32 ret;
    if (!PMi_Lock()) {
        return 1;
    }
    ret = 0;
    data_0207f0c0[idx].flag = ret;
    data_0207f09c.callback = callback;
    data_0207f09c.arg = arg;
    data_0207f0c0[idx].value = value;
    PMi_SendPxiData((idx & 0xff) | 0x3006500);
    return ret;
}
