#include <nitro/types.h>

typedef struct UnkStruct_023041a8_e {
    u8 pad0[0x1c];
    s32 unk1C;
    s32 unk20;
    u8 pad24[4];
    struct UnkStruct_023041a8_e *next;
} UnkStruct_023041a8_e;

typedef struct UnkStruct_023041a8 {
    u8 pad0[0x94];
    u32 unk94;
    UnkStruct_023041a8_e *list98;
    u8 pad9C[0xa8 - 0x9c];
    UnkStruct_023041a8_e *listA8;
    u8 padAC[4];
    UnkStruct_023041a8_e *listB0;
} UnkStruct_023041a8;

typedef struct UnkStruct_023041a8_p {
    u8 pad0[4];
    s32 unk4;
} UnkStruct_023041a8_p;

typedef struct UnkStruct_023041a8_g {
    u32 flags;
    u8 pad4[0x4c - 4];
    s32 unk4C;
} UnkStruct_023041a8_g;

typedef struct UnkStruct_023041a8_t {
    u8 pad0[0x1dc];
    UnkStruct_023041a8_p *unk1DC;
} UnkStruct_023041a8_t;

extern UnkStruct_023041a8 *gExtraBossRingWork;

UnkStruct_023041a8_t *ExtraBoss_GetPlayer(void);
UnkStruct_023041a8_g *ExtraBoss_GetWork(void);
BOOL ExtraBoss_IsRingTouchingPlayer(UnkStruct_023041a8_e *e);
void ExtraBoss_RingList_RemovePlaced(UnkStruct_023041a8_e *e);
void ExtraBoss_RingList_RemoveLoose(UnkStruct_023041a8_e *e);
void ExtraBoss_RingList_RemoveAttracted(UnkStruct_023041a8_e *e);
void ExtraBoss_RingList_AddCollected(UnkStruct_023041a8_e *e);
void Sound_PlaySeqArc(u32 *p, u32 a, u32 b);
void AnimatorSprite_ProcessAnimation(void *c, u32 a, u32 b);

void ExtraBoss_CollectRings(void) {
    UnkStruct_023041a8_p *p = ExtraBoss_GetPlayer()->unk1DC;
    UnkStruct_023041a8_e *e;
    UnkStruct_023041a8_e *cur;
    UnkStruct_023041a8_e *e2;
    UnkStruct_023041a8_e *cur2;
    UnkStruct_023041a8_e *e3;
    UnkStruct_023041a8_e *cur3;

    if ((gExtraBossRingWork->unk94 & 2) && !(ExtraBoss_GetWork()->flags & 0x10) && p->unk4 != 5 && p->unk4 != 6 && p->unk4 != 0xc) {
        e = gExtraBossRingWork->list98;
        while (e != NULL) {
            cur = e;
            e = e->next;
            if (ExtraBoss_IsRingTouchingPlayer(cur)) {
                ExtraBoss_RingList_RemovePlaced(cur);
                ExtraBoss_RingList_AddCollected(cur);
                cur->unk20 = 0xf;
                ExtraBoss_GetWork()->unk4C++;
                if (ExtraBoss_GetWork()->unk4C > 999)
                    ExtraBoss_GetWork()->unk4C = 999;
                Sound_PlaySeqArc(0, 0, 4);
            }
        }
        e2 = gExtraBossRingWork->listA8;
        while (e2 != NULL) {
            cur2 = e2;
            e2 = e2->next;
            if (cur2->unk1C > 0) {
                cur2->unk1C--;
            } else if (ExtraBoss_IsRingTouchingPlayer(cur2)) {
                ExtraBoss_RingList_RemoveLoose(cur2);
                ExtraBoss_RingList_AddCollected(cur2);
                cur2->unk20 = 0xf;
                ExtraBoss_GetWork()->unk4C++;
                if (ExtraBoss_GetWork()->unk4C > 999)
                    ExtraBoss_GetWork()->unk4C = 999;
                Sound_PlaySeqArc(0, 0, 4);
            }
        }
        e3 = gExtraBossRingWork->listB0;
        while (e3 != NULL) {
            cur3 = e3;
            e3 = e3->next;
            if (cur3->unk1C > 0) {
                cur3->unk1C--;
            } else if (ExtraBoss_IsRingTouchingPlayer(cur3)) {
                ExtraBoss_RingList_RemoveAttracted(cur3);
                ExtraBoss_RingList_AddCollected(cur3);
                cur3->unk20 = 0xf;
                ExtraBoss_GetWork()->unk4C++;
                if (ExtraBoss_GetWork()->unk4C > 999)
                    ExtraBoss_GetWork()->unk4C = 999;
                Sound_PlaySeqArc(0, 0, 4);
            }
        }
    }
    AnimatorSprite_ProcessAnimation(gExtraBossRingWork, 0, 0);
    AnimatorSprite_ProcessAnimation((u8 *)gExtraBossRingWork + 0x48, 0, 0);
}
