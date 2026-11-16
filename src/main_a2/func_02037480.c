#include <nitro/types.h>
#include "main_a2/task.h"

extern u32 data_022b4578;
extern UnkTask data_022b498c[];
extern UnkTask *data_022b458c[];
extern UnkTask *data_022b457c;
extern UnkTask *data_022b4580;
extern UnkTask *data_022b4584;
extern UnkTask *data_022b4588;
extern UnkTask *data_022b4574;

UnkTask *func_02036e9c(void);
void MIi_CpuClear32(u32 data, void *dst, u32 size);

void func_02037480(void) {
    s32 i;
    {
        vu32 zero = 0;
        data_022b4578 = 0;
        MIi_CpuClear32(zero, data_022b498c, sizeof(UnkTask) * 256);
    }
    for (i = 0; i < 256; i++) {
        data_022b458c[i] = &data_022b498c[i];
    }
    data_022b457c = func_02036e9c();
    data_022b4580 = func_02036e9c();
    data_022b4584 = func_02036e9c();
    data_022b4588 = func_02036e9c();
    data_022b457c->prev = NULL;
    data_022b457c->next = data_022b4580;
    data_022b457c->func = NULL;
    data_022b457c->dtor = NULL;
    data_022b457c->prio = 0x20;
    data_022b457c->id = 0;
    data_022b457c->unk1a = 0xffff;
    data_022b4580->prev = data_022b457c;
    data_022b4580->next = data_022b4584;
    data_022b4580->func = NULL;
    data_022b4580->dtor = NULL;
    data_022b4580->prio = 0x20;
    data_022b4580->id = 0xefff;
    data_022b4580->unk1a = 0xffff;
    data_022b4584->prev = data_022b4580;
    data_022b4584->next = data_022b4588;
    data_022b4584->func = NULL;
    data_022b4584->dtor = NULL;
    data_022b4584->prio = 0x20;
    data_022b4584->id = 0xf000;
    data_022b4584->unk1a = 0xffff;
    data_022b4588->prev = data_022b4584;
    data_022b4588->next = NULL;
    data_022b4588->func = NULL;
    data_022b4588->dtor = NULL;
    data_022b4588->prio = 0x20;
    data_022b4588->id = 0xffff;
    data_022b4588->unk1a = 0xffff;
    data_022b4574 = data_022b457c;
}
