#include <nitro/types.h>

typedef struct UnkStruct_022f2ca8_s {
    s32 state;
    s32 next;
    s32 prev;
    s32 count;
    s32 unk10;
} UnkStruct_022f2ca8_s;

typedef struct UnkStruct_022f2ca8_g {
    u8 pad0[0x40];
    s32 unk40;
} UnkStruct_022f2ca8_g;

typedef struct UnkStruct_022f2ca8 {
    u8 pad0[0x3c];
    u16 unk3C;
    u16 unk3E;
    u16 unk40;
    u8 pad42[0x50 - 0x42];
    s32 unk50;
    s32 unk54;
    s32 unk58;
    u8 pad5C[0x1dc - 0x5c];
    UnkStruct_022f2ca8_s *unk1DC;
} UnkStruct_022f2ca8;

UnkStruct_022f2ca8_g *ExtraBoss_GetWork(void);
void ExtraBoss_Unk_ov001_022f2780(UnkStruct_022f2ca8 *p);
void ExtraBoss_Unk_ov001_022f1c0c(UnkStruct_022f2ca8 *p);
void ExtraBoss_Unk_ov001_022f0988(UnkStruct_022f2ca8 *p);
void ExtraBoss_Unk_ov001_022f2898(UnkStruct_022f2ca8 *p);

void ExtraBoss_Unk_ov001_022f2ca8(UnkStruct_022f2ca8 *p) {
    UnkStruct_022f2ca8_g *g = ExtraBoss_GetWork();
    UnkStruct_022f2ca8_s *s = p->unk1DC;
    s32 st;

    p->unk50 = p->unk54 = p->unk58 = 0;
    p->unk3C = p->unk3E = p->unk40 = 0;
    if (s->state == 2)
        s->state = s->prev;
    st = s->state;
    if (g->unk40 == 1) {
        switch (st) {
        case 0:
        case 1:
            switch (s->next) {
            case 4:
                ExtraBoss_Unk_ov001_022f2780(p);
                s->state = 4;
                break;
            case 5:
                ExtraBoss_Unk_ov001_022f1c0c(p);
                s->state = 5;
                break;
            case 6:
                ExtraBoss_Unk_ov001_022f0988(p);
                s->state = 6;
                break;
            }
            s->next = 1;
            break;
        case 4:
            s->count--;
            if (s->count > 0)
                s->next = 4;
            else if (s->unk10 == 2)
                s->next = 6;
            else
                s->next = 5;
            ExtraBoss_Unk_ov001_022f2898(p);
            s->state = 1;
            break;
        case 5:
            s->unk10 = 2;
            s->count = s->unk10;
            s->next = 4;
            ExtraBoss_Unk_ov001_022f2898(p);
            s->state = 1;
            break;
        case 6:
            s->unk10 = 1;
            s->count = s->unk10;
            s->next = 4;
            ExtraBoss_Unk_ov001_022f2898(p);
            s->state = 1;
            break;
        }
    } else {
        switch (st) {
        case 0:
        case 1:
            switch (s->next) {
            case 4:
                ExtraBoss_Unk_ov001_022f2780(p);
                s->state = 4;
                break;
            case 5:
                ExtraBoss_Unk_ov001_022f1c0c(p);
                s->state = 5;
                break;
            case 6:
                ExtraBoss_Unk_ov001_022f0988(p);
                s->state = 6;
                break;
            }
            s->next = 1;
            break;
        case 4:
        case 5:
            s->next = 6;
            ExtraBoss_Unk_ov001_022f2898(p);
            s->state = 1;
            break;
        case 6:
            if (s->prev == 4)
                s->next = 5;
            else
                s->next = 4;
            ExtraBoss_Unk_ov001_022f2898(p);
            s->state = 1;
            break;
        }
    }
    if (st != 1)
        s->prev = st;
}
