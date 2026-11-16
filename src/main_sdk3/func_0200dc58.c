#include <nitro/types.h>

typedef struct {
    u32 lock;                               /* 0x00 */
    void (*callback)(u32 result, void *arg); /* 0x04 */
    void *arg;                              /* 0x08 */
    u8 padC[4];
    void (*cb2)(u32 result, void *arg);     /* 0x10 */
    void *arg2;                             /* 0x14 */
    u16 *buffer;                            /* 0x18 */
} UnkPMWork_0200dc58;

extern UnkPMWork_0200dc58 data_0207f068;

void func_0200dc58(u32 tag, u32 data, BOOL err)
{
    u16 command;
    u16 result;
    u32 r;
    void (*cb)(u32 result, void *arg);

    if (err) {
        if (data_0207f068.lock) {
            data_0207f068.lock = 0;
        }
        cb = data_0207f068.callback;
        if (cb != NULL) {
            data_0207f068.callback = NULL;
            cb(6, data_0207f068.arg);
        }
    }
    result = (u16)(data & 0xff);
    command = (u16)((data & 0x7f00) >> 8);
    switch (result) {
    case 0:
        r = 0;
        break;
    case 1:
        r = 4;
        break;
    case 2:
        r = 2;
        break;
    case 3:
        r = 5;
        break;
    case 4:
        r = 1;
        break;
    default:
        r = 6;
        break;
    }
    if (command == 0x51) {
        if (data_0207f068.cb2 != NULL) {
            data_0207f068.cb2(r, data_0207f068.arg2);
        }
        return;
    }
    if (command == 0x40) {
        if (data_0207f068.buffer != NULL) {
            *data_0207f068.buffer = *(u16 *)0x027fff94;
        }
    }
    if (data_0207f068.lock) {
        data_0207f068.lock = 0;
    }
    cb = data_0207f068.callback;
    if (cb != NULL) {
        data_0207f068.callback = NULL;
        cb(r, data_0207f068.arg);
    }
}
