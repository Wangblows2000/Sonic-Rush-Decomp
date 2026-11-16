#include <nitro/types.h>

typedef struct {
    u8 pad0[0x38];
    u8 file38[0xc];
    u32 unk44;
    u8 pad48[0x38];
    s32 unk80;
    u32 unk84;
    u32 unk88;
    u8 pad8c[8];
    s32 unk94;
    s32 unk98;
    s32 unk9c;
    s32 unka0[2];
    u8 pada8[0x18];
    u8 bufc0[2][0x400];
} UnkNode_0203cc68;

typedef struct {
    u32 w0;
    u32 w4;
    u32 w8;
} UnkEnt_0203cc68;

typedef struct {
    UnkEnt_0203cc68 ent[16];
    u8 padc0[0x834c];
    u32 unk840c;
    u8 pad8410[0xc];
    u32 unk841c;
    u32 unk8420;
    u32 pad8424;
    u32 unk8428;
    u32 pad842c;
    s32 unk8430[16];
} UnkData_0203cc68;

typedef struct {
    u32 kind : 4;
    u32 b4 : 4;
    u32 val : 24;
    u32 w4;
    u8 b8;
} UnkSub_0203cc68;

typedef struct {
    u32 w0;
    u32 w4;
    void *w8;
    s16 hc;
} UnkSub2_0203cc68;

typedef struct {
    s32 unk0;
    s32 unk4;
    u16 unk8;
    u16 unka;
    u8 padc[8];
    UnkEnt_0203cc68 sub;
} UnkArg_0203cc68;

typedef struct {
    u8 pad0[0x28];
    u32 unk28;
} UnkCtx_0203cc68;

extern UnkData_0203cc68 data_022b9020;
extern UnkCtx_0203cc68 data_022b8be0;

s32 WBT_AidbitmapToAid(s32 v);
UnkNode_0203cc68 *WFSi_FindAlive(UnkData_0203cc68 *s, u32 a, u32 len);
UnkNode_0203cc68 *WFSi_FindBusy(UnkData_0203cc68 *s, u32 a, u32 len);
UnkNode_0203cc68 *WFSi_FindAliveForID(UnkData_0203cc68 *s, u32 id);
void WBT_UnregisterBlock(u32 id);
void WFSi_FromAliveToBusy(UnkData_0203cc68 *s, UnkNode_0203cc68 *n);
void WFSi_NotifyBusy(void);
BOOL FS_SeekFile(void *f, u32 pos, u32 whence);
void FS_ReadFileAsync(void *f, void *buf, u32 size);
u32 OS_DisableInterrupts(void);
void OS_RestoreInterrupts(u32 e);

static inline BOOL Flag_0203cc68(UnkNode_0203cc68 *n) {
    if (n->unk44 & 1)
        return TRUE;
    return FALSE;
}

void WFSi_OnParentSystemCallback(UnkArg_0203cc68 *arg) {
    s32 idx;
    UnkSub_0203cc68 *p;
    UnkData_0203cc68 *d;
    s32 off;

    idx = WBT_AidbitmapToAid(arg->unka);
    if (arg->unk0 != 12)
        return;
    switch (arg->unk4) {
    case 2:
        break;
    case 8:
        break;
    case 10: {
        d = &data_022b9020;
        p = (UnkSub_0203cc68 *)&arg->sub;
        off = idx * 12;
        *(UnkEnt_0203cc68 *)((u8 *)d->ent + off) = arg->sub;
        switch (p->kind) {
        case 0: {
            UnkNode_0203cc68 *n;
            u32 len2 = p->val;
            u32 v2 = p->w4;
            u32 key2 = (p->b4 << 8) | p->b8;
            if (d->unk8420 != 0 || key2 != data_022b8be0.unk28) {
                d->unk8420 = 1;
                d->unk8428 |= 1 << idx;
                d->unk841c |= 1 << idx;
                return;
            }
            n = WFSi_FindAlive(d, v2, len2);
            if (n != NULL) {
                d->unk840c |= 1 << idx;
                ((UnkEnt_0203cc68 *)((u8 *)d->ent + off))->w4 = n->unk88;
                n->unk80++;
            } else {
                n = WFSi_FindBusy(d, v2, len2);
                n->unk84 |= 1 << idx;
            }
            d->unk841c |= 1 << idx;
            break;
        }
        case 2: {
            u32 e = OS_DisableInterrupts();
            UnkNode_0203cc68 *n;
            d->unk840c |= 1 << idx;
            n = WFSi_FindAliveForID(d, p->w4);
            if (n != NULL) {
                n->unk80--;
                if (n->unk80 <= 0) {
                    n->unk84 = 0;
                    WBT_UnregisterBlock(p->w4);
                    WFSi_FromAliveToBusy(d, n);
                    WFSi_NotifyBusy();
                }
                d->unk8430[idx]++;
            }
            OS_RestoreInterrupts(e);
            break;
        }
        }
        break;
    }
    case 13: {
        UnkSub2_0203cc68 *q = (UnkSub2_0203cc68 *)&arg->sub;
        UnkNode_0203cc68 *n;
        s32 sz;
        s32 old;
        s32 nv;
        s32 a;
        s32 cur;
        s32 i;
        s32 j;
        s32 off;
        s32 k;
        u32 id;
        id = arg->sub.w0;
        q->w8 = NULL;
        n = WFSi_FindAliveForID(&data_022b9020, id);
        if (n == NULL)
            return;
        sz = q->hc;
        old = n->unk94;
        nv = q->w4;
        if (n->unk98 != 2 && !!(n->unk44 & 1) == 0)
            n->unk98 = 2;
        a = sz * old;
        n->unk94 = nv;
        cur = n->unk98;
        for (i = 0; i < 2; i++) {
            if (i != cur) {
                off = a - n->unka0[i];
                if (off >= 0 && off + sz <= 0x400) {
                    q->w4 = old;
                    q->w8 = &n->bufc0[i][off];
                    break;
                }
            }
        }
        if (n->unk98 != 2)
            return;
        a = sz * nv;
        for (j = 0; j < 2; j++) {
            off = a - n->unka0[j];
            if (off >= 0 && off + sz <= 0x400)
                break;
        }
        if (j < 2)
            return;
        k = n->unk9c;
        do {
            k++;
            if (k >= 2)
                k = 0;
        } while (k == i);
        n->unk9c = k;
        n->unk98 = k;
        n->unka0[k] = a & ~0x1ff;
        FS_SeekFile(n->file38, a & ~0x1ff, 0);
        FS_ReadFileAsync(n->file38, n->bufc0[k], 0x400);
        break;
    }
    }
}
