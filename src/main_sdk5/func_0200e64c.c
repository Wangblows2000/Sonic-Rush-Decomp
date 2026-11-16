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

BOOL func_0200ea14(void);
void func_0200e2d0(u32 cmd);

u32 func_0200e64c(u32 idx, u32 value, u32 callback, u32 arg)
{
    u32 ret;
    if (!func_0200ea14()) {
        return 1;
    }
    ret = 0;
    data_0207f0c0[idx].flag = ret;
    data_0207f09c.callback = callback;
    data_0207f09c.arg = arg;
    data_0207f0c0[idx].value = value;
    func_0200e2d0((idx & 0xff) | 0x3006500);
    return ret;
}
