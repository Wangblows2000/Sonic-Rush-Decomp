#include <nitro/types.h>

typedef struct {
    u16 id;
    u8 dur;
    u8 pad3;
} UnkSeq_02067ddc;

typedef struct {
    u32 flags;
    s32 unk4;
    u32 unk8;
    u16 unkc;
    u16 unke;
    u16 unk10;
    u8 pad12[6];
    UnkSeq_02067ddc *seq;
} UnkWork_02067ddc;

typedef struct {
    u8 pad0[0x10];
    UnkWork_02067ddc *work;
} UnkTask_02067ddc;

extern UnkTask_02067ddc *data_022b4574;
extern u16 data_022c4ee4;
extern u32 data_02087cb8;
extern u8 data_022503e0[];

void func_020371c8(UnkTask_02067ddc *t);
void func_02035134(void *p, u16 id);

void func_02067ddc(void) {
    UnkWork_02067ddc *w = data_022b4574->work;
    UnkSeq_02067ddc *e;

    if (w->unke != data_022c4ee4) {
        if (w->unke == 3) {
            u16 i;
            for (i = 0; w->unk8 > i; i++) {
            }
        }
        w->unkc = 0;
        w->unk8 = 0;
        switch (data_022c4ee4) {
        default:
        case 0:
        case 1:
            data_02087cb8 &= ~0x40;
            w->flags &= ~3;
            break;
        case 2:
        case 4:
            w->unk10 = 8;
            data_02087cb8 |= 0x40;
            w->flags &= ~3;
            w->flags |= 2;
            break;
        case 3:
            data_02087cb8 &= ~0x40;
            w->flags &= ~3;
            w->flags |= 1;
            break;
        case 5:
        case 6:
            func_020371c8(data_022b4574);
            return;
        }
    }
    w->unke = data_022c4ee4;
    if (w->flags & 2) {
        if (w->unk10 != 0)
            w->unk10--;
        if (w->flags & 1) {
            w->flags &= ~1;
            w->unk8 = 0;
            w->unk4 = 0;
        }
        e = &w->seq[w->unk8];
        func_02035134(data_022503e0, e->id);
        w->unkc++;
        if (e->dur <= w->unkc) {
            w->unk8++;
            w->unkc = 0;
        }
        if (e->dur == 0) {
            w->flags &= ~2;
            data_022c4ee4 = 1;
            w->unkc = 0;
        }
    }
    w->unk4++;
}
