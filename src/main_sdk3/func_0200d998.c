#include <nitro/types.h>

typedef struct {
    u16 x;
    u16 y;
    u16 touch;
    u16 validity;
} UnkTPData_0200d998;

typedef struct {
    u32 x : 12;
    u32 y : 12;
    u32 touch : 1;
    u32 validity : 2;
    u32 : 5;
} UnkTPRaw_0200d998;

typedef struct {
    void (*callback)(u32 command, u8 result, u8 index); /* 0x00 */
    UnkTPData_0200d998 last;       /* 0x04 */
    u16 index;                     /* 0x0c */
    u8 padE[2];
    UnkTPData_0200d998 *buf;       /* 0x10 */
    u16 frequence;                 /* 0x14 */
    u8 pad16[0x32 - 0x16];
    u16 state;                     /* 0x32 */
    vu16 errFlags;                 /* 0x34 */
    vu16 cmdFlags;                 /* 0x36 */
} UnkTPState_0200d998;

extern UnkTPState_0200d998 data_0207f02c;

void OS_Terminate(void);

static inline void func_0200d998_Copy(UnkTPData_0200d998 *tp)
{
    UnkTPRaw_0200d998 raw;
    *(u16 *)&raw = *(u16 *)0x027fffaa;
    *((u16 *)&raw + 1) = *(u16 *)0x027fffac;
    tp->x = raw.x;
    tp->y = raw.y;
    tp->touch = (u8)raw.touch;
    tp->validity = (u8)raw.validity;
}

void func_0200d998(u32 tag, u32 data, BOOL err)
{
    u16 result = (u16)data;
    u16 command = (u16)((result & 0x7f00) >> 8);
    u32 res;

    if (err) {
        data_0207f02c.errFlags |= 1 << command;
        if (data_0207f02c.callback != NULL) {
            data_0207f02c.callback(command, 4, 0);
        }
        return;
    }
    if (command == 0x10) {
        data_0207f02c.index++;
        if (data_0207f02c.index >= data_0207f02c.frequence) {
            data_0207f02c.index = 0;
        }
        func_0200d998_Copy(&data_0207f02c.buf[data_0207f02c.index]);
        if (data_0207f02c.callback != NULL) {
            data_0207f02c.callback(command, 0, (u8)data_0207f02c.index);
        }
        return;
    }
    if (!(data & 0x1000000)) {
        return;
    }
    switch ((u8)result) {
    case 0:
        switch (command) {
        case 0:
            func_0200d998_Copy(&data_0207f02c.last);
            data_0207f02c.state = 0;
            break;
        case 1:
            data_0207f02c.state = 2;
            break;
        case 2:
            data_0207f02c.state = 0;
            break;
        }
        data_0207f02c.cmdFlags &= ~(1 << command);
        if (data_0207f02c.callback != NULL) {
            data_0207f02c.callback(command, 0, 0);
        }
        return;
    case 4:
        res = 3;
        goto error;
    case 2:
        res = 1;
        goto error;
    case 3:
        res = 2;
    error:
        data_0207f02c.errFlags |= 1 << command;
        data_0207f02c.cmdFlags &= ~(1 << command);
        if (data_0207f02c.callback != NULL) {
            data_0207f02c.callback(command, res, 0);
        }
        return;
    case 1:
    default:
        OS_Terminate();
        return;
    }
}
