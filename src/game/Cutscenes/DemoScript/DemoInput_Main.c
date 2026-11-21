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

extern UnkTask_02067ddc *gTaskCurrent;
extern u16 gDemoInputMode;
extern u32 gSystemFlags;
extern u8 gPad[];

void Task_Destroy(UnkTask_02067ddc *t);
void Pad_Update(void *p, u16 id);

void DemoInput_Main(void) {
    UnkWork_02067ddc *w = gTaskCurrent->work;
    UnkSeq_02067ddc *e;

    if (w->unke != gDemoInputMode) {
        if (w->unke == 3) {
            u16 i;
            for (i = 0; w->unk8 > i; i++) {
            }
        }
        w->unkc = 0;
        w->unk8 = 0;
        switch (gDemoInputMode) {
        default:
        case 0:
        case 1:
            gSystemFlags &= ~0x40;
            w->flags &= ~3;
            break;
        case 2:
        case 4:
            w->unk10 = 8;
            gSystemFlags |= 0x40;
            w->flags &= ~3;
            w->flags |= 2;
            break;
        case 3:
            gSystemFlags &= ~0x40;
            w->flags &= ~3;
            w->flags |= 1;
            break;
        case 5:
        case 6:
            Task_Destroy(gTaskCurrent);
            return;
        }
    }
    w->unke = gDemoInputMode;
    if (w->flags & 2) {
        if (w->unk10 != 0)
            w->unk10--;
        if (w->flags & 1) {
            w->flags &= ~1;
            w->unk8 = 0;
            w->unk4 = 0;
        }
        e = &w->seq[w->unk8];
        Pad_Update(gPad, e->id);
        w->unkc++;
        if (e->dur <= w->unkc) {
            w->unk8++;
            w->unkc = 0;
        }
        if (e->dur == 0) {
            w->flags &= ~2;
            gDemoInputMode = 1;
            w->unkc = 0;
        }
    }
    w->unk4++;
}
