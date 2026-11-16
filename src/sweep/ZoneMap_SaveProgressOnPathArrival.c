#include <nitro/types.h>

typedef struct {
    s32 mode;
    s32 unk4;
    s32 unk8;
    u8 padc[0x94 - 0xc];
    s32 unk94;
    u32 unk98;
} UnkStruct_022e21ec;

void ZoneMap_SaveProgress(UnkStruct_022e21ec *p);

#pragma thumb on
void ZoneMap_SaveProgressOnPathArrival(UnkStruct_022e21ec *p) {
    s32 save;
    s32 v;
    s32 w;

    if (p->mode == 0) {
        v = p->unk94;
        if (v == 0x24) return;
        save = p->unk4;
        if (v == 0x15) v = 0x16;
        if (v == 0x1a) v = 0x1b;
        switch (v) {
        case 5:
        case 0xb:
        case 0x11:
        case 0x13:
        case 0x16:
        case 0x18:
        case 0x1b:
        case 0x1e:
        case 0x21:
            p->unk4 = v;
            ZoneMap_SaveProgress(p);
            break;
        }
        p->unk4 = save;
    } else {
        w = p->unk98;
        if (w == 0x23) return;
        save = p->unk8;
        switch (w) {
        case 3:
        case 11:
        case 13:
        case 15:
        case 17:
        case 19:
        case 21:
        case 23:
        case 25:
        case 27:
        case 29:
        case 32:
            p->unk8 = w;
            ZoneMap_SaveProgress(p);
            break;
        case 31:
            break;
        }
        p->unk8 = save;
    }
}
#pragma thumb reset
