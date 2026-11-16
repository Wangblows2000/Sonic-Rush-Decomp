#include <nitro/types.h>
typedef void (*UnkCallback_0200e9a0)(u32 a, u32 b);
typedef struct {
    vu32 busy;
    UnkCallback_0200e9a0 callback;
    u32 arg;
} UnkStruct_0207f09c;
extern UnkStruct_0207f09c data_0207f09c;
void func_0200e9a0(u32 result)
{
    UnkCallback_0200e9a0 cb = data_0207f09c.callback;
    u32 arg = data_0207f09c.arg;
    if (data_0207f09c.busy) {
        data_0207f09c.busy = FALSE;
    }
    if (cb) {
        data_0207f09c.callback = NULL;
        cb(result, arg);
    }
}
