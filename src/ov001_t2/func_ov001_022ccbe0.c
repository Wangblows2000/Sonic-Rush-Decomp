#include <nitro/types.h>

typedef struct {
    s32 state;
    s32 sel;
} UnkNext_022ccbe0;

typedef struct {
    u8 pad0[0x14];
    s32 unk14;
    s32 unk18;
    u8 pad1c[4];
    s32 timer;
    u8 pad24[8];
    s32 unk2c;
    s32 unk30;
    u8 pad34[0x3ac - 0x34];
    UnkNext_022ccbe0 *next;
} UnkWork_022ccbe0;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022ccbe0 *work;
} UnkTask_022ccbe0;

extern UnkTask_022ccbe0 *data_022b4574;

void func_ov001_022cc17c(UnkWork_022ccbe0 *w);
void func_ov001_022cbf30(UnkWork_022ccbe0 *w);
void func_020371c8(UnkTask_022ccbe0 *t);

#pragma thumb on
void func_ov001_022ccbe0(void) {
    UnkWork_022ccbe0 *w = data_022b4574->work;

    func_ov001_022cc17c(w);
    func_ov001_022cbf30(w);
    w->unk14++;
    w->unk18++;
    w->timer++;
    if (w->timer >= 30) {
        w->next->state = 10;
        w->next->sel = 0;
        switch (w->unk2c) {
        case 0:
            switch (w->unk30) {
            case 0:
                w->next->sel = 1;
                break;
            case 1:
                break;
            }
            break;
        case 1:
            switch (w->unk30) {
            case 0:
                w->next->sel = 2;
                break;
            case 1:
                w->next->sel = 3;
                break;
            }
            break;
        }
        func_020371c8(data_022b4574);
    }
}
#pragma thumb reset
