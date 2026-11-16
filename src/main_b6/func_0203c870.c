#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    u32 unk8;
    u8 padc[0x1c];
    s32 unk28;
    u32 unk2c;
    u32 pad30;
    s32 unk34;
} UnkFile_0203c870;

typedef struct {
    u8 pad0[0xc];
    s32 unkc;
    void (*cb10)(s32 a);
    void *(*fn14)(void *a, void *b, s32 c);
    void *unk18;
    UnkFile_0203c870 *unk1c;
    void *unk20;
    void *unk24;
    s32 unk28;
    s32 unk2c;
} UnkCtx_0203c870;

typedef struct {
    u32 pad0;
    void *unk4;
} UnkEnt_0203c870;

typedef struct {
    UnkEnt_0203c870 *ent[16];
    void *arr40[16];
    u8 buf80[0xa7c];
    u32 unkafc;
    u32 padb00;
    u32 unkb04;
} UnkData_0203c870;

typedef struct {
    u32 kind : 4;
    u32 b4 : 4;
    u32 val : 24;
    u32 w4;
} UnkSub_0203c870;

typedef struct {
    s32 unk0;
    s32 unk4;
    u16 unk8;
    u16 unka;
    u8 padc[0xa];
    s16 unk16;
    s16 unk18;
} UnkArg_0203c870;

extern UnkData_0203c870 data_022b9020;
extern UnkCtx_0203c870 data_022b8be0;

s32 WBT_AidbitmapToAid(s32 v);
void WBT_GetBlockInfo(u16 id, u32 arg1, const void *src, void *arg3);
void func_0203cb9c(UnkData_0203c870 *d, s32 n);
void func_0203d268(UnkFile_0203c870 *f);
void WBT_GetBlock(u16 a, u32 b, const void *c, u32 d, const void *f, void *e);
void DC_FlushRange(void *p, u32 size);
u32 func_0203bd08(void *p);
s32 func_0203d8b4(s32 a, s32 b, u32 c, s32 d);
void WBT_RequestSync(u16 a, void *b);
void FS_NotifyArchiveAsyncEnd(u32 a, s32 b);
u32 OS_DisableInterrupts(void);
void OS_RestoreInterrupts(u32 e);
void OS_Terminate(void);

void func_0203c870(UnkArg_0203c870 *arg) {
    s32 idx = WBT_AidbitmapToAid(arg->unka);
    UnkData_0203c870 *d = &data_022b9020;

    switch (arg->unk0) {
    case 2:
        if (arg->unk8 != 0)
            return;
        data_022b8be0.unk28 = arg->unk16 + 0xe;
        data_022b8be0.unk2c = arg->unk18 + 0xe;
        if (data_022b8be0.unk20 == NULL) {
            WBT_GetBlockInfo(1, 0, d, func_0203c870);
            return;
        }
        if (data_022b8be0.unk1c == NULL)
            return;
        func_0203cb9c(d, -1);
        func_0203d268(data_022b8be0.unk1c);
        break;
    case 6: {
        void *v;
        u32 e;
        void *r;
        v = d->ent[idx]->unk4;
        data_022b8be0.unk24 = v;
        e = OS_DisableInterrupts();
        r = data_022b8be0.fn14(data_022b8be0.unk18, v, 0);
        OS_RestoreInterrupts(e);
        data_022b8be0.unk20 = r;
        func_0203cb9c(d, (s32)data_022b8be0.unk24);
        d->arr40[idx] = data_022b8be0.unk20;
        WBT_GetBlock(1, 0x20000, d->arr40, (u32)data_022b8be0.unk24, d->buf80, func_0203c870);
        break;
    }
    case 4:
        if (arg->unk8 != 0)
            return;
        if (data_022b8be0.unkc != 2) {
            DC_FlushRange(data_022b8be0.unk20, (u32)data_022b8be0.unk24);
            d->unkafc = func_0203bd08(data_022b8be0.unk20);
            data_022b8be0.unkc = 2;
            if (data_022b8be0.cb10 != NULL)
                data_022b8be0.cb10(0);
        } else {
            if (func_0203d8b4(2, 1, d->unkb04, 1) == 0)
                OS_Terminate();
        }
        break;
    case 12: {
        UnkSub_0203c870 *p;
        if (arg->unk4 != 10)
            return;
        p = (UnkSub_0203c870 *)((u8 *)arg + 0x14);
        switch (p->kind) {
        case 1:
            if (p->val == 0) {
                WBT_RequestSync(1, func_0203c870);
                return;
            }
            d->unkb04 = p->w4;
            d->arr40[idx] = (void *)data_022b8be0.unk1c->unk2c;
            WBT_GetBlock(1, d->unkb04, d->arr40, data_022b8be0.unk1c->unk34, d->buf80, func_0203c870);
            break;
        case 3: {
            UnkFile_0203c870 *f = data_022b8be0.unk1c;
            u32 v = f->unk8;
            data_022b8be0.unk1c = NULL;
            f->unk28 += f->unk34;
            FS_NotifyArchiveAsyncEnd(v, 0);
            break;
        }
        }
        break;
    }
    }
}
