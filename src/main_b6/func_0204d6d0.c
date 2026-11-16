#include <nitro/types.h>

typedef struct {
    u8 pad0[0x18];
    u32 flags;
    u8 zone;
    u8 act;
} UnkState_0204d6d0;

typedef struct {
    u8 pad0[0x10];
    u32 *work;
} UnkTask_0204d6d0;

extern UnkState_0204d6d0 data_022c4560;

void func_0204d214(BOOL f);
void func_0204ce7c(BOOL f);
void *Task_Create(void (*func)(void), void *dtor, u16 prio, u16 id, u16 unk1a, u32 workSize);
void func_0204bf24(void);

void func_0204d6d0(u32 v) {
    BOOL f;

    if ((data_022c4560.flags & 0x40) == 0)
        data_022c4560.flags &= ~2;
    f = (data_022c4560.flags & 0x10) ? TRUE : FALSE;
    if (data_022c4560.zone < 9 && data_022c4560.act < 2)
        func_0204d214(f);
    else
        func_0204ce7c(f);
    *((UnkTask_0204d6d0 *)Task_Create(func_0204bf24, 0, 0, 0, 0, 0xc))->work = v;
}
