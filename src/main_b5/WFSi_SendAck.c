#include <nitro/types.h>

typedef struct {
    u32 type : 4;
    u32 rest : 28;
    u32 unk4;
    u32 unk8;
} UnkEntry_0203d644;

typedef struct {
    UnkEntry_0203d644 e[0x840c / 12];
    s32 m40c;
    s32 m410;
    s32 busy414;
    s32 m418;
    s32 m41c;
    s32 f420;
    s32 f424;
    s32 m428;
    u32 v42c;
} UnkSub_0203d644;

typedef struct {
    s32 unk0;
    u8 pad4[0x24];
    u32 unk28;
    u32 unk2c;
    u8 pad30[0x410];
    UnkSub_0203d644 sub;
} UnkStruct_022b8be0;

extern UnkStruct_022b8be0 data_022b8be0;

void WBT_SetPacketSize(u32 a, u32 b);
BOOL WFSi_SendMessage(u8 type, u16 mask, u32 v, s32 flag);

void WFSi_SendAck(void) {
    UnkStruct_022b8be0 *m;
    UnkSub_0203d644 *s;
    s32 mask;
    BOOL flag;
    UnkEntry_0203d644 *e;
    s32 i;
    s32 m410;
    s32 bit;
    u32 v;
    u32 w;
    s32 z;

    m = &data_022b8be0;
    s = (UnkSub_0203d644 *)((u32)m + 0x440);
    if (m->unk0 == 0)
        return;
    if (s->busy414 != 0)
        return;
    s->m40c &= s->m418;
    s->m410 &= s->m418;
    s->m41c &= s->m418;
    s->m428 &= s->m418;
    if (s->f420 != 0 && s->f424 == 0) {
        v = s->v42c;
        w = m->unk2c;
        z = 0;
        s->f420 = z;
        m->unk28 = v;
        WBT_SetPacketSize(v, w);
        if (s->m428 == 0)
            return;
        if (!WFSi_SendMessage(1, s->m428, z, z))
            return;
        s->busy414 = 1;
        s->m428 = z;
        return;
    }
    mask = s->m40c;
    if (mask == 0)
        return;
    m410 = s->m410;
    e = NULL;
    if (m410 != 0 && m410 == (m410 & mask))
        flag = TRUE;
    else
        flag = FALSE;
    if (flag)
        mask = m410;
    else
        mask &= ~m410;
    for (i = 0;; i++) {
        bit = 1 << i;
        if (bit > mask)
            break;
        if (bit & mask) {
            if (e == NULL)
                e = &s->e[i];
            else if (e->type != s->e[i].type || e->unk4 != s->e[i].unk4)
                mask &= ~bit;
        }
    }
    if (flag && mask != m410)
        s->m410 = 0;
    if (e == NULL)
        return;
    switch (e->type) {
    case 0:
        e->type = 1;
        break;
    case 2:
        e->type = 3;
        s->m41c &= ~mask;
        break;
    }
    if (!WFSi_SendMessage(e->type, mask, e->unk4, 1))
        return;
    s->busy414 = 1;
    s->m40c &= ~mask;
}
