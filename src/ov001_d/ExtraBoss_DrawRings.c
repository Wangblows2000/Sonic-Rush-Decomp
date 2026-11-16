#include <nitro/types.h>

typedef struct UnkStruct_02304028_spr {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x48 - 8];
} UnkStruct_02304028_spr;

typedef struct UnkStruct_02304028_n {
    s32 x;
    s32 y;
    u8 pad8[0x28 - 8];
    struct UnkStruct_02304028_n *next;
} UnkStruct_02304028_n;

typedef struct UnkStruct_02304028 {
    UnkStruct_02304028_spr spr0;
    UnkStruct_02304028_spr spr1;
    u8 pad90[0x98 - 0x90];
    UnkStruct_02304028_n *list98;
    u8 pad9C[4];
    UnkStruct_02304028_n *listA0;
    u8 padA4[4];
    UnkStruct_02304028_n *listA8;
    u8 padAC[4];
    UnkStruct_02304028_n *listB0;
} UnkStruct_02304028;

extern UnkStruct_02304028 *gExtraBossRingWork;
extern s32 data_ov001_0231f520;
extern s32 data_ov001_0231f524;

void AnimatorSprite_Draw(void *p);

void ExtraBoss_DrawRings(void) {
    UnkStruct_02304028 *g = gExtraBossRingWork;
    UnkStruct_02304028_spr *s1 = &g->spr1;
    UnkStruct_02304028_n *n;

    for (n = g->list98; n != NULL; n = n->next) {
        g->spr0.x = (n->x + data_ov001_0231f524) >> 12;
        g->spr0.y = (n->y + data_ov001_0231f520) >> 12;
        AnimatorSprite_Draw(&g->spr0);
    }
    for (n = gExtraBossRingWork->listA8; n != NULL; n = n->next) {
        g->spr0.x = (n->x + data_ov001_0231f524) >> 12;
        g->spr0.y = (n->y + data_ov001_0231f520) >> 12;
        AnimatorSprite_Draw(&g->spr0);
    }
    for (n = gExtraBossRingWork->listB0; n != NULL; n = n->next) {
        g->spr0.x = (n->x + data_ov001_0231f524) >> 12;
        g->spr0.y = (n->y + data_ov001_0231f520) >> 12;
        AnimatorSprite_Draw(&g->spr0);
    }
    for (n = gExtraBossRingWork->listA0; n != NULL; n = n->next) {
        s1->x = (n->x + data_ov001_0231f524) >> 12;
        s1->y = (n->y + data_ov001_0231f520) >> 12;
        AnimatorSprite_Draw(s1);
    }
}
