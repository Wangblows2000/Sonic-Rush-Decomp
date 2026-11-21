#include <nitro/types.h>

typedef struct UnkEntry_0203c5cc UnkEntry_0203c5cc;
struct UnkEntry_0203c5cc {
    UnkEntry_0203c5cc *next;
    u8 unk4[0x34];
    u8 unk38[0x44];
    u32 unk7c;
    u32 unk80;
    u8 pad84[4];
    s32 id;
    u8 pad8c[0x8c0 - 0x8c];
};

typedef struct {
    u8 pad0[0xc0];
    UnkEntry_0203c5cc e[15];
    UnkEntry_0203c5cc *free;
    UnkEntry_0203c5cc *used;
    u32 unk8408;
    u32 m40c;
    u32 m410;
    u32 busy414;
    u32 m418;
    u32 m41c;
    u32 f420;
    u32 f424;
    u32 m428;
    u32 v42c;
    u32 unk8430[0x10];
} UnkSub_0203c5cc;

typedef struct {
    s32 unk0;
    u8 pad4[8];
    s32 unkc;
    u8 pad10[0x10];
    u32 unk20;
    u32 unk24;
    u32 unk28;
    u32 unk2c;
} UnkStruct_022b8be0;

extern s32 data_022b8bc0;
extern UnkStruct_022b8be0 data_022b8be0;
extern u8 data_02073214[];

s32 OS_DisableInterrupts(void);
s32 OS_RestoreInterrupts(s32 e);
void WFSi_InitCommon(s32 a, s32 b, void (*cb)(void), s32 d);
void DC_FlushRange(void *p, u32 size);
void DC_WaitWriteBufferEmpty(void);
void FS_InitFile(void *p);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void MI_CpuFill8(void *dst, u8 v, u32 size);
void WBT_InitParent(u32 a, u32 b, void (*cb)(void));
void WFSi_OnParentSystemCallback(void);
void WFSi_LoadTables(s32 a, s32 b);
void WBT_RegisterBlock(void *p, u32 size, void *a, u32 b, u32 c, s32 d);
void WFSi_CreateTaskThread(void);

void WFS_InitParent(s32 a, s32 b, void (*cb)(void), s32 d, u32 e, s32 f, s32 g) {
    s32 save;
    UnkStruct_022b8be0 *m;
    UnkSub_0203c5cc *s;
    UnkEntry_0203c5cc *p;

    save = OS_DisableInterrupts();
    if (data_022b8bc0 != 0)
        return;
    data_022b8bc0 = 1;
    WFSi_InitCommon(a, b, cb, d);
    OS_RestoreInterrupts(save);
    m = &data_022b8be0;
    s = (UnkSub_0203c5cc *)((u32)m + 0x440);
    m->unk0 = 1;
    DC_FlushRange(s, sizeof(UnkSub_0203c5cc));
    DC_WaitWriteBufferEmpty();
    p = s->e;
    s->free = p;
    for (; p + 1 < &s->e[15]; p++) {
        p->next = p + 1;
        FS_InitFile(p->unk38);
        p->unk80 = 0;
        p->unk7c = 0;
        p->id = p + 0x10001 - s->e;
    }
    p->next = NULL;
    s->m410 = 0;
    s->m40c = 0;
    s->busy414 = 0;
    s->used = NULL;
    s->unk8408 = 0;
    m->unk28 = e;
    s->m418 = 1;
    s->m41c = 0;
    s->f420 = 0;
    s->m428 = 0;
    s->f424 = 0;
    s->v42c = m->unk28;
    {
        vu32 zero = 0;
        MIi_CpuClear32(zero, s->unk8430, sizeof(s->unk8430));
    }
    DC_FlushRange(s, sizeof(UnkSub_0203c5cc));
    DC_WaitWriteBufferEmpty();
    WBT_InitParent(m->unk28, m->unk2c, WFSi_OnParentSystemCallback);
    WFSi_LoadTables(f, g);
    p = s->free;
    s->free = p->next;
    p->next = s->used;
    s->used = p;
    p->unk80 = 1;
    MI_CpuFill8(p, 0, sizeof(UnkEntry_0203c5cc));
    WBT_RegisterBlock(p->unk4, 0x20000, data_02073214, m->unk20, data_022b8be0.unk24, 0);
    m->unkc = 1;
    WFSi_CreateTaskThread();
}
