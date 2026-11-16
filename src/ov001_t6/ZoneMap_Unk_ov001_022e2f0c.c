#include <nitro/types.h>

typedef struct {
    s32 state;
    s32 unk4;
    s32 unk8;
    u8 pad[0xa4 - 0xc];
    u8 *unka4;
} UnkStruct_022e2f0c;

BOOL ZoneMap_Unk_ov001_022e2f00(s32 a);
BOOL ZoneMap_Unk_ov001_022e2ee8(s32 a);
void ZoneMap_Unk_ov001_022e3180(void *p, s32 n);

#pragma thumb on
#pragma opt_propagation off
void ZoneMap_Unk_ov001_022e2f0c(UnkStruct_022e2f0c *p, s32 b) {
    s32 state = p->state;
    void *obj = *(void **)(p->unka4 + 0x1cc4);
    switch (state) {
    case 0:
        if (!ZoneMap_Unk_ov001_022e2f00(p->unk4)) return;
        switch (b) {
        case 1: ZoneMap_Unk_ov001_022e3180(obj, 4); return;
        case 2: ZoneMap_Unk_ov001_022e3180(obj, 5); return;
        case 3: ZoneMap_Unk_ov001_022e3180(obj, 6); return;
        case 6: ZoneMap_Unk_ov001_022e3180(obj, 7); return;
        case 8: ZoneMap_Unk_ov001_022e3180(obj, 8); return;
        case 12: ZoneMap_Unk_ov001_022e3180(obj, 9); return;
        case 13: ZoneMap_Unk_ov001_022e3180(obj, 10); return;
        case 14: ZoneMap_Unk_ov001_022e3180(obj, 11); return;
        case 15: ZoneMap_Unk_ov001_022e3180(obj, 12); return;
        case 16: ZoneMap_Unk_ov001_022e3180(obj, 14); return;
        case 7: ZoneMap_Unk_ov001_022e3180(obj, 17); return;
        case 9: ZoneMap_Unk_ov001_022e3180(obj, 18); return;
        }
        break;
    case 1:
        if (!ZoneMap_Unk_ov001_022e2ee8(p->unk8)) return;
        switch (b) {
        case 1: ZoneMap_Unk_ov001_022e3180(obj, 4); return;
        case 2: ZoneMap_Unk_ov001_022e3180(obj, 5); return;
        case 3: ZoneMap_Unk_ov001_022e3180(obj, 6); return;
        case 6: ZoneMap_Unk_ov001_022e3180(obj, 7); return;
        case 8: ZoneMap_Unk_ov001_022e3180(obj, 8); return;
        case 12: ZoneMap_Unk_ov001_022e3180(obj, 9); return;
        case 13: ZoneMap_Unk_ov001_022e3180(obj, 10); return;
        case 14: ZoneMap_Unk_ov001_022e3180(obj, 11); return;
        case 16: ZoneMap_Unk_ov001_022e3180(obj, 14); return;
        case 0: ZoneMap_Unk_ov001_022e3180(obj, 16); return;
        case 4: ZoneMap_Unk_ov001_022e3180(obj, 19); return;
        case 5: ZoneMap_Unk_ov001_022e3180(obj, 20); return;
        case 11: ZoneMap_Unk_ov001_022e3180(obj, 21); return;
        case 10: ZoneMap_Unk_ov001_022e3180(obj, 22); return;
        }
        break;
    }
}
#pragma thumb reset
