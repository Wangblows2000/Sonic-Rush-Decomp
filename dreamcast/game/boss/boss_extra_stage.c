/*
 * Extra Zone boss: rings (ExtraBoss_InitRings, the four ring lists, collection, suction, scatter),
 * the background (zex.bbg: palette cycling, wave scroll, meteor and big-beam spawning while it
 * scrolls), the HUD (ExtraBoss_Unk_ov001_022fdbc0, rewritten from its ARM listing, and
 * ExtraBoss_DrawHud) and the sub-screen "VS" sequence (ExtraBoss_AllocSubScreenWork,
 * ExtraBoss_Unk_ov001_0230xxxx).  Translated from src/game/Zone/Extra_Zone/Boss/ (overlay ov001).
 */
#include <stdlib.h>
#include <string.h>
#include "boss_extra_int.h"

ExRingWork *gExtraBossRingWork;
ExBgWork *data_ov001_0231f54c;
ExSubWork *data_ov001_0231f5a8;
ExSprite *data_ov001_0231f324;

/* --------------------------------------------------------------- rings */
static ExRing *ExtraBoss_StackPop(void) {
    ExRingWork *t = gExtraBossRingWork;
    ExRing *v;
    if (t->unkB8 >= 0x64)
        return NULL;
    v = t->unkBC[t->unkB8];
    t->unkB8++;
    return v;
}
static void ExtraBoss_StackPush(ExRing *v) {
    gExtraBossRingWork->unkB8--;
    gExtraBossRingWork->unkBC[gExtraBossRingWork->unkB8] = v;
}
#define EX_RING_LIST(NAME, HEAD, TAIL)                                                                   \
    static void ExtraBoss_RingList_Add##NAME(ExRing *n) {                                               \
        if (gExtraBossRingWork->TAIL != NULL) {                                                         \
            gExtraBossRingWork->TAIL->next = n;                                                         \
            n->prev = gExtraBossRingWork->TAIL;                                                         \
            n->next = NULL;                                                                             \
            gExtraBossRingWork->TAIL = n;                                                               \
        } else {                                                                                        \
            gExtraBossRingWork->TAIL = n;                                                               \
            gExtraBossRingWork->HEAD = gExtraBossRingWork->TAIL;                                        \
            n->prev = n->next = NULL;                                                                   \
        }                                                                                               \
    }                                                                                                   \
    static void ExtraBoss_RingList_Remove##NAME(ExRing *n) {                                            \
        if (n->prev == NULL)                                                                            \
            gExtraBossRingWork->HEAD = n->next;                                                         \
        else                                                                                            \
            n->prev->next = n->next;                                                                    \
        if (n->next == NULL)                                                                            \
            gExtraBossRingWork->TAIL = n->prev;                                                         \
        else                                                                                            \
            n->next->prev = n->prev;                                                                    \
    }
EX_RING_LIST(Placed, unk98, unk9C)
EX_RING_LIST(Collected, unkA0, unkA4)
EX_RING_LIST(Loose, unkA8, unkAC)
EX_RING_LIST(Attracted, unkB0, unkB4)

static BOOL ExtraBoss_IsOutOfField(const ExRing *p) {
    if (p->x < -0x10000 || 0x190000 < p->x || p->y < -0x10000 || 0x190000 < p->y)
        return 1;
    return 0;
}

static void ExtraBoss_UpdateRings(void) {
    ExRing *n, *next;
    next = gExtraBossRingWork->unk98;
    while (next != NULL) {
        n = next;
        next = n->next;
        n->x -= 0x4000;
        if (ExtraBoss_IsOutOfField(n)) {
            ExtraBoss_RingList_RemovePlaced(n);
            ExtraBoss_StackPush(n);
        }
    }
    next = gExtraBossRingWork->unkA8;
    while (next != NULL) {
        n = next;
        next = n->next;
        n->vx += n->ax;
        n->vy += n->ay;
        n->x += n->vx;
        n->y += n->vy;
        if (ExtraBoss_IsOutOfField(n)) {
            ExtraBoss_RingList_RemoveLoose(n);
            ExtraBoss_StackPush(n);
        }
    }
    next = gExtraBossRingWork->unkA0;
    while (next != NULL) {
        n = next;
        next = n->next;
        n->x -= 0x4000;
        n->unk18--;
        if (n->unk18 < 0) {
            ExtraBoss_RingList_RemoveCollected(n);
            ExtraBoss_StackPush(n);
        }
    }
}

static void ExtraBoss_UpdateRingsSuction(void) {
    s32 tz = ExtraBoss_GetWork()->unk310;
    u16 a;
    ExRing *p, *n;
    n = gExtraBossRingWork->unk98;
    while (n != NULL) {
        p = n;
        n = p->next;
        p->x -= 0x4000;
        if (p->x < ExtraBoss_GetWork()->unk31C + 0x80000) {
            p->unk18--;
            if (p->unk18 <= 0) {
                p->vx = p->vy = 0;
                a = FX_Atan2(tz - p->y, 0xc0000 - p->x);
                p->ax = FX_Mul(COS(a), 0x100);
                p->ay = FX_Mul(SIN(a), 0x100);
                ExtraBoss_RingList_RemovePlaced(p);
                ExtraBoss_RingList_AddAttracted(p);
                continue;
            }
        }
        if (ExtraBoss_IsOutOfField(p)) {
            ExtraBoss_RingList_RemovePlaced(p);
            ExtraBoss_StackPush(p);
        }
    }
    n = gExtraBossRingWork->unkA8;
    while (n != NULL) {
        p = n;
        n = p->next;
        p->vx += p->ax;
        p->vy += p->ay;
        p->x += p->vx;
        p->y += p->vy;
        p->unk18--;
        if (p->unk18 <= 0) {
            p->vx = p->vy = 0;
            a = FX_Atan2(tz - p->y, 0xc0000 - p->x);
            p->ax = FX_Mul(COS(a), 0x100);
            p->ay = FX_Mul(SIN(a), 0x100);
            ExtraBoss_RingList_RemoveLoose(p);
            ExtraBoss_RingList_AddAttracted(p);
        } else if (ExtraBoss_IsOutOfField(p)) {
            ExtraBoss_RingList_RemoveLoose(p);
            ExtraBoss_StackPush(p);
        }
    }
    n = gExtraBossRingWork->unkB0;
    while (n != NULL) {
        p = n;
        n = p->next;
        p->vx += p->ax;
        p->vy += p->ay;
        p->x += p->vx;
        p->y += p->vy;
        if (p->x <= 0x11a000 && p->x >= 0x66000 && p->y <= tz + 0x10000 && p->y >= tz - 0x10000) {
            ExtraBoss_RingList_RemoveAttracted(p);
            ExtraBoss_RingList_AddCollected(p);
            p->unk20 = 0xf;
        }
    }
    p = gExtraBossRingWork->unkA0;
    while (p != NULL) {
        ExRing *nx = p->next;
        p->unk20--;
        if (p->unk20 < 0) {
            ExtraBoss_RingList_RemoveCollected(p);
            ExtraBoss_StackPush(p);
        }
        p = nx;
    }
}

void ExtraBoss_StartRingSuction(void) { gExtraBossRingWork->unk90 = ExtraBoss_UpdateRingsSuction; }

void ExtraBoss_EndRingSuction(void) {
    ExRing *n, *next;
    u16 ang;
    gExtraBossRingWork->unk90 = ExtraBoss_UpdateRings;
    next = gExtraBossRingWork->unkB0;
    while (next != NULL) {
        n = next;
        next = n->next;
        ang = (u16)((Rand() & 0x3fff) + 0xa000);
        n->ax = 0;
        n->ay = 0x100;
        n->vx = FX_Mul(COS(ang), 0x2000);
        n->vy = FX_Mul(SIN(ang), 0x2000);
        n->unk18 = 2;
        n->unk1C = 1;
        ExtraBoss_RingList_RemoveAttracted(n);
        ExtraBoss_RingList_AddLoose(n);
    }
}

static BOOL ExtraBoss_IsRingTouchingPlayer(const ExRing *pos) {
    ExObj *player = ExtraBoss_GetPlayer();
    const ExCollider *box = &player->unkB0[EX_COLL_B0];
    s32 cx, cy, pl, pt, pr, pb, rx, rr, rl, ry, rt;
    cx = (s32)((u32)player->unk18 + (u32)box->unkC[0]);
    pr = cx + box->unk18[0];
    pl = cx - box->unk18[0];
    cy = (s32)((u32)player->unk1C + (u32)box->unkC[1]);
    pt = cy - box->unk18[1];
    pb = cy + box->unk18[1];
    rx = pos->x;
    rr = rx + 0x8000;
    rl = rx - 0x8000;
    ry = pos->y;
    rt = ry - 0x10000;
    if (pl > rl && pl > rr)
        return FALSE;
    if (rl > pl && rl > pr)
        return FALSE;
    if (pt > rt && pt > ry)
        return FALSE;
    if (rt > pt && rt > pb)
        return FALSE;
    return TRUE;
}

static void ex_collect(ExRing *cur) {
    ExtraBoss_RingList_AddCollected(cur);
    cur->unk20 = 0xf;
    ExtraBoss_GetWork()->unk4C++;
    if (ExtraBoss_GetWork()->unk4C > 999)
        ExtraBoss_GetWork()->unk4C = 999;
    Sound_PlaySeqArc(0, 0, 4);
    g_bossex_stats.rings_collected++;
}
void ExtraBoss_CollectRings(void) {
    ExPlayerWork *p = (ExPlayerWork *)ExtraBoss_GetPlayer()->unk1DC;
    ExRing *e, *cur;
    if ((gExtraBossRingWork->unk94 & 2) && !(ExtraBoss_GetWork()->unk0 & 0x10) && p->unk4 != 5 && p->unk4 != 6 &&
        p->unk4 != 0xc) {
        e = gExtraBossRingWork->unk98;
        while (e != NULL) {
            cur = e;
            e = e->next;
            if (ExtraBoss_IsRingTouchingPlayer(cur)) {
                ExtraBoss_RingList_RemovePlaced(cur);
                ex_collect(cur);
            }
        }
        e = gExtraBossRingWork->unkA8;
        while (e != NULL) {
            cur = e;
            e = e->next;
            if (cur->unk1C > 0) {
                cur->unk1C--;
            } else if (ExtraBoss_IsRingTouchingPlayer(cur)) {
                ExtraBoss_RingList_RemoveLoose(cur);
                ex_collect(cur);
            }
        }
        e = gExtraBossRingWork->unkB0;
        while (e != NULL) {
            cur = e;
            e = e->next;
            if (cur->unk1C > 0) {
                cur->unk1C--;
            } else if (ExtraBoss_IsRingTouchingPlayer(cur)) {
                ExtraBoss_RingList_RemoveAttracted(cur);
                ex_collect(cur);
            }
        }
    }
    ex_sprite_process(&gExtraBossRingWork->unk0);
    ex_sprite_process(&gExtraBossRingWork->unk48);
}

void ExtraBoss_DrawRings(void) {
    ExRingWork *g = gExtraBossRingWork;
    ExRing *n;
    for (n = g->unk98; n != NULL; n = n->next) {
        g->unk0.x = (s16)((n->x + data_ov001_0231f524) >> 12);
        g->unk0.y = (s16)((n->y + data_ov001_0231f520) >> 12);
        ex_sprite_draw(&g->unk0);
    }
    for (n = g->unkA8; n != NULL; n = n->next) {
        g->unk0.x = (s16)((n->x + data_ov001_0231f524) >> 12);
        g->unk0.y = (s16)((n->y + data_ov001_0231f520) >> 12);
        ex_sprite_draw(&g->unk0);
    }
    for (n = g->unkB0; n != NULL; n = n->next) {
        g->unk0.x = (s16)((n->x + data_ov001_0231f524) >> 12);
        g->unk0.y = (s16)((n->y + data_ov001_0231f520) >> 12);
        ex_sprite_draw(&g->unk0);
    }
    for (n = g->unkA0; n != NULL; n = n->next) {
        g->unk48.x = (s16)((n->x + data_ov001_0231f524) >> 12);
        g->unk48.y = (s16)((n->y + data_ov001_0231f520) >> 12);
        ex_sprite_draw(&g->unk48);
    }
}

void ExtraBoss_ScatterPlayerRings(s32 x, s32 y, s32 n) {
    ExWork *g = ExtraBoss_GetWork();
    ExRing *e;
    s32 i;
    u16 ang, step;
    if (g->unk4C < n)
        n = g->unk4C;
    if (n <= 0)
        return;
    g->unk4C -= n;
    g_bossex_stats.rings_lost += (u32)n;
    if (n == 1) {
        ang = 0xc000;
        step = 0;
    } else {
        ang = 0xa000;
        step = (u16)FX_DivS32(0x4000, n - 1);
    }
    for (i = 0; i < n; i++, ang += step) {
        e = ExtraBoss_StackPop();
        if (e == NULL)   /* the DS writes through a NULL pointer here; natively the ring is dropped */
            break;
        e->x = x;
        e->y = y;
        e->ax = 0;
        e->ay = 0x100;
        e->vx = FX_Mul(COS(ang), 0x2800);
        e->vy = FX_Mul(SIN(ang), 0x2800);
        e->unk18 = 0x11;
        e->unk1C = 5;
        ExtraBoss_RingList_AddLoose(e);
    }
}

void ExtraBoss_SpawnRingBurst(s32 x, s32 y, s32 count) {
    s32 i;
    u16 ang, step;
    ExRing *n;
    if (count == 1) {
        ang = 0xbfff;
        step = 0;
    } else {
        ang = 0xaaaa;
        step = (u16)FX_DivS32(0x2aab, count - 1);
    }
    for (i = 0; i < count; i++, ang += step) {
        n = ExtraBoss_StackPop();
        if (n == NULL)
            break;
        n->x = x;
        n->y = y;
        n->ax = 0;
        n->ay = 0x100;
        n->vx = FX_Mul(COS(ang), 0x2000);
        n->vy = FX_Mul(SIN(ang), 0x2000);
        n->unk18 = 2;
        n->unk1C = 5;
        ExtraBoss_RingList_AddLoose(n);
    }
}

static void ExtraBoss_PushScrollEntry(s32 p0, s32 p1) {
    ExRing *t0 = ExtraBoss_StackPop();
    if (t0 == NULL)
        return;
    t0->x = p0;
    t0->y = p1;
    t0->ay = 0;
    t0->ax = t0->ay;
    t0->vy = 0;
    t0->vx = t0->vy;
    t0->unk18 = 2;
    t0->unk1C = 0;
    ExtraBoss_RingList_AddPlaced(t0);
}

static void ExtraBoss_SpawnFromScrollTable(void) {
    ExRingWork *g = gExtraBossRingWork;
    g->unk1380 += 0x4000;
    for (;;) {
        const s32 *t = EX_DATA(g->unk1384, const s32 *);
        s32 idx = g->unk137C;
        s32 pos = g->unk1380;
        if (pos < t[idx * 2])
            return;
        ExtraBoss_PushScrollEntry(t[idx * 2] + 0x188000 - pos, t[idx * 2 + 1]);
        g->unk137C++;
        if (t[g->unk137C * 2] == -1) {
            g->unk137C = 0;
            g->unk1380 -= 0xe00000;
        }
    }
}

void ExtraBoss_SetRingPickupEnabled(s32 p0) {
    gExtraBossRingWork->unk94 &= ~2u;
    gExtraBossRingWork->unk94 |= (p0 != 0) ? 2 : 0;
}
void ExtraBoss_SetScrollSpawnEnabled(s32 p0) {
    gExtraBossRingWork->unk94 &= ~1u;
    gExtraBossRingWork->unk94 |= (p0 != 0);
}

void ExtraBoss_ClearSpawnLists(void) {
    ExRing *n, *nx;
    for (n = gExtraBossRingWork->unk98; n != NULL; n = nx) {
        nx = n->next;
        ExtraBoss_RingList_RemovePlaced(n);
        ExtraBoss_StackPush(n);
    }
    for (n = gExtraBossRingWork->unkA8; n != NULL; n = nx) {
        nx = n->next;
        ExtraBoss_RingList_RemoveLoose(n);
        ExtraBoss_StackPush(n);
    }
    for (n = gExtraBossRingWork->unkB0; n != NULL; n = nx) {
        nx = n->next;
        ExtraBoss_RingList_RemoveAttracted(n);
        ExtraBoss_StackPush(n);
    }
    for (n = gExtraBossRingWork->unkA0; n != NULL; n = nx) {
        nx = n->next;
        ExtraBoss_RingList_RemoveCollected(n);
        ExtraBoss_StackPush(n);
    }
}

void ExtraBoss_ResetSpawnWork(void) {
    ExtraBoss_ClearSpawnLists();
    gExtraBossRingWork->unk137C = 0;
    gExtraBossRingWork->unk1380 = gExtraBossRingWork->unk137C;
    if (ExtraBoss_GetWork()->unk40 == 4)
        gExtraBossRingWork->unk1384 = 0x02316b9c;
    else
        gExtraBossRingWork->unk1384 = 0x02316cac;
    gExtraBossRingWork->unk90 = ExtraBoss_UpdateRings;
    gExtraBossRingWork->unk94 |= 3;
}

void ExtraBoss_UpdateSpawnWork(void) {
    if (gExtraBossRingWork->unk94 & 1)
        ExtraBoss_SpawnFromScrollTable();
    if (gExtraBossRingWork->unk90 == NULL)
        return;
    gExtraBossRingWork->unk90();
}

void ExtraBoss_InitRings(void) {
    s32 i;
    gExtraBossRingWork = (ExRingWork *)calloc(1, sizeof(ExRingWork));
    for (i = 0; i < 100; i++)
        gExtraBossRingWork->unkBC[i] = &gExtraBossRingWork->unk24C[i];
    gExtraBossRingWork->unk90 = ExtraBoss_UpdateRings;
    ex_sprite_init(&gExtraBossRingWork->unk0, &gExtraBossRingSprite, 0, 0x4060);
    gExtraBossRingWork->unk0.unk44 = 0;
    gExtraBossRingWork->unk0.flags |= 4;
    ex_sprite_process(&gExtraBossRingWork->unk0);
    ex_sprite_init(&gExtraBossRingWork->unk48, &gExtraBossRingSprite, 1, 0x4030);
    gExtraBossRingWork->unk48.unk44 = 0;
    gExtraBossRingWork->unk48.flags |= 4;
    ex_sprite_process(&gExtraBossRingWork->unk48);
    gExtraBossRingWork->unk1384 = 0x02316cac;
}

void ExtraBoss_FreeSpawnWork(void) {
    free(gExtraBossRingWork);
    gExtraBossRingWork = NULL;
}

/* ---------------------------------------------------------- background */
/* PaletteTransfer_QueueCopy(src, 0xc0, 0, 0x5000000): 192 colours of a zex palette file into BG
 * palette 0.  File 0 is zex.bbg itself (its own palette), 1..21 the palette-only BBGs. */
static void ex_bg_palette_copy(int file) {
    const u16 *src = file == 0 ? g_ex_bg.base : g_ex_bg.pals + 192 * (file - 1);
    if (g_ex_bg.bytes == NULL || file > g_ex_bg.npal)
        return;
    memcpy(data_ov001_0231f54c->pal, src, 192 * 2);
    data_ov001_0231f54c->pal_dirty = 1;
}
/* PaletteTransfer_QueueLoad (whole palette resource of the file) */
static void ex_bg_palette_load(int file) {
    if (g_ex_bg.bytes == NULL)
        return;
    if (file == 0) {
        memcpy(data_ov001_0231f54c->pal, g_ex_bg.base, 256 * 2);
    } else {
        memset(data_ov001_0231f54c->pal, 0, sizeof(data_ov001_0231f54c->pal));
        memcpy(data_ov001_0231f54c->pal, g_ex_bg.pals + 192 * (file - 1), 192 * 2);
    }
    data_ov001_0231f54c->pal_dirty = 1;
}

void ExtraBoss_AllocBgWork(void) {
    data_ov001_0231f54c = (ExBgWork *)calloc(1, sizeof(ExBgWork));
    /* Background_Init(bg, zex.bbg, 0x31, 0, 1, w, h) + Background_Draw: tiles, map and palette */
    ex_bg_palette_load(0);
}
void ExtraBoss_FreeBgWork(void) {
    free(data_ov001_0231f54c);
    data_ov001_0231f54c = NULL;
}

void ExtraBoss_StartBgShake(s32 a, s32 b) {
    data_ov001_0231f54c->unkB4C = a;
    data_ov001_0231f54c->unkB50 = b * 2 + 1;
}

static void ExtraBoss_State_ScrollDone(void) {}
static void ExtraBoss_UpdateSpawnTimer(void) {
    s32 y, s;
    data_ov001_0231f54c->unkB3C--;
    if (data_ov001_0231f54c->unkB3C > 0)
        return;
    y = FX_ModS32(Rand(), 6) << 17;
    s = -data_ov001_0231f520;
    data_ov001_0231f54c->unkB40++;
    y = y + 0x10000 + (s & ~0x1f);
    if (data_ov001_0231f54c->unkB40 >= 10) {
        data_ov001_0231f54c->unkB40 = 0;
        ExtraBoss_SpawnMeteorR(NULL, 0x108000, y, 0);
    } else {
        ExtraBoss_SpawnMeteor(NULL, 0x108000, y, 0);
    }
    data_ov001_0231f54c->unkB3C = (Rand() & 0x1f) + 0x14;
}
static void ExtraBoss_State_ScrollSpawn(void) {
    ExWork *g;
    ExtraBoss_UpdateSpawnTimer();
    data_ov001_0231f54c->unkB44 += 1;
    if (data_ov001_0231f54c->unkB44 >= data_ov001_0231f54c->unkB48) {
        data_ov001_0231f54c->unkB44 = 0;
        ExtraBoss_SpawnBigBeam(NULL, ExtraBoss_GetPlayer()->unk1C, 0);
    }
    if (data_ov001_0231f54c->unk588 < 0xe10000)
        return;
    g = ExtraBoss_GetWork();
    data_ov001_0231f54c->unk598 = ExtraBoss_State_ScrollDone;
    g->unk50 = 0;
    g->unk54 = 0;
    g->unk58 = 0x180000;
    g->unk5C = 0x180000;
    ExtraBoss_SpawnBoss();
}

void ExtraBoss_SpawnBoss(void) {
    ExWork *g = ExtraBoss_GetWork();
    g->unk30C = EX_S32(0x023167bc, ex_level() * 5 + g->unk40);
    g->unk308 = g->unk30C;
    if (g->unk40 == 1 || g->unk40 == 3)
        ExtraBoss_CreateHandBoss();
    else
        ExtraBoss_CreateWeaponBoss();
}

void ExtraBoss_Unk_ov001_02304fb4(void) {
    s32 stage = ExtraBoss_GetWork()->unk40;
    s32 i, j;
    s16 *dst;
    ExBgWork *b = data_ov001_0231f54c;
    if (stage < 4) {
        ex_bg_palette_load(0);
        b->unk5A4 = 0x02316f44;
        b->unk5A8 = 0x14;
    } else {
        ex_bg_palette_load(11);
        b->unk5A4 = 0x02316f94;
        b->unk5A8 = 0x14;
    }
    b->unk588 = EX_S32(0x02316ee0, ExtraBoss_GetWork()->unk40);
    b->unk58C = 0x60000;
    b->unk584 = 0;
    b->unk586 = 0x20;
    b->unk590 = EX_S32(0x02316f1c, stage);
    if (ExtraBoss_GetWork()->unk40 > 1)
        b->unk598 = ExtraBoss_State_ScrollDone;
    else
        b->unk598 = ExtraBoss_State_ScrollSpawn;
    b->unkB3C = 0;
    b->unkB40 = 0;
    b->unkB44 = 0;
    b->unkB48 = EX_S32(0x02316f08, stage);
    b->unk59C = 0;
    b->unk5A0 = 0;
    b->unkB2C = 0;
    b->unkB34 = 0;
    b->unkB38 = 0;
    b->unkB30 = 0xff;
    dst = b->unk8AC;
    for (i = 0; i < 8; i++) {
        s32 amp;
        u32 angle;
        amp = (Rand() & 0xfff) + 0x1000;
        angle = (u16)((i & 1) << 15);
        for (j = 0; j < 0x20; j++, dst++) {
            s32 s = SIN((u16)angle);
            *dst = (s16)((s32)(((s64)s * amp + 0x800) >> 12) >> 9);
            angle += 0x400;
        }
    }
    /* MIi_CpuCopy16(unk8AC[1], unk5AC / unk72C, 0x180): the HDMA buffers start from the wave */
    memcpy(b->unk5AC[0], &b->unk8AC[0x20], 0x180);
    memcpy(b->unk5AC[1], &b->unk8AC[0x20], 0x180);
}

void ExtraBoss_UpdateBgScrollPalette(void) {
    ExBgWork *b = data_ov001_0231f54c;
    s32 s;
    const u16 *f;
    if (b->unk0 & 1) {
        b->unk594 = 0;
        return;
    }
    s = b->unk590;
    if (ExtraBoss_IsPlayerSpawned() && ((ExPlayerWork *)ExtraBoss_GetPlayer()->unk1DC)->unk4 == 5)
        s >>= 1;
    b->unk588 += s;
    b->unk594 = s;
    if (b->unk598 != NULL)
        b->unk598();
    b->unk59C--;
    if (b->unk59C > 0)
        return;
    b->unk5A0++;
    if (b->unk5A0 >= b->unk5A8)
        b->unk5A0 = 0;
    f = EX_DATA(b->unk5A4, const u16 *) + 2 * b->unk5A0;
    ex_bg_palette_copy(f[0]);
    b->unk59C = f[1];
}

void ExtraBoss_UpdateBgWaveScroll(void) {
    ExBgWork *g = data_ov001_0231f54c;
    s32 type, amp, i, ang, period, a, b, t, s, k, j;
    u16 phase, x, y;
    s16 *w, *src;
    s16 *dst;
    if (g->unk0 & 1)
        return;
    type = ExtraBoss_GetWork()->unk40;
    x = (u16)((FX_DivS32((ExtraBoss_GetWork()->unk31C >> 12) - 0x80, 3) + (g->unk588 >> 16)) & 0xff);
    y = (u16)(FX_DivS32(((ExtraBoss_GetWork()->unk320 >> 12) - 0x60) * 0x38, 0xc0) & 0xff);
    if (g->unkB4C > 0) {
        g->unkB4C--;
        x = (u16)(x + FX_ModS32(Rand(), g->unkB50) - (g->unkB50 >> 1));
        y = (u16)(y + FX_ModS32(Rand(), g->unkB50) - (g->unkB50 >> 1));
    }
    g->unkB34 += 0x800;
    phase = (u16)(g->unkB34 >> 12);
    if (phase + 0x100 - g->unkB30 > 0) {
        amp = (Rand() & 0x7ff) + 0x1000;
        k = FX_ModS32(g->unkB30 + 1, 0x140);
        w = &g->unk8AC[k];
        ang = (u16)((FX_DivS32(k, 0x20) & 1) << 15);
        for (i = 0; i < 0x20; i++, w++) {
            *w = (s16)(FX_Mul(SIN((u16)ang), amp) >> 9);
            ang += 0x400;
        }
        g->unkB30 += 0x20;
    }
    if (g->unkB34 >= 0x140000) {
        g->unkB34 -= 0x140000;
        g->unkB30 -= 0x140;
    }
    g->unkB2C ^= 1;
    period = EX_S32(0x02316ef4, type);
    a = EX_S32(0x02316f30, type);
    b = EX_S32(0x02316ecc, type);
    g->unkB38++;
    if (g->unkB38 > period * 2)
        g->unkB38 = 0;
    t = g->unkB38;
    if (t >= period)
        t = period * 2 - t;
    s = FX_DivS32(a * t + b * (period - t), period);
    dst = g->unk5AC[g->unkB2C];
    k = (phase + y) & 0xff;
    src = &g->unk8AC[k];
    j = 0;
    for (; j < 0xc0 && k < 0x140; j++, k++, dst++, src++)
        *dst = (s16)(x + (FX_Mul(*src << 12, s) >> 12));
    if (j < 0xc0) {
        src = g->unk8AC;
        for (; j < 0xc0; j++) {
            *dst = (s16)(x + (FX_Mul(*src << 12, s) >> 12));
            src++;
            dst++;
        }
    }
    g->vofs = (s16)y;   /* gDisplayMain BG1 VOFS; gSystemFlags |= 0x2000 (HDMA buffer swap) */
}

/* ----------------------------------------------------------------- HUD */
/* ExtraBoss_Unk_ov001_022fdbc0 (substantially rewritten from the ARM listing in
 * src/asm_fallback/ExtraBoss_Unk_ov001_022fdbc0.c): 0x1b sprites of ac_ex_fix_moji.bac (archive file
 * 0x26), animations data_ov001_02316e94[i], palette 1 for 0..0x17 and 2 for 0x18..0x1a, and the fixed
 * positions of the labels: sprite 0x14 (0, 2), 0x15 (0, 2), 0x16 (0x5c, 0x12), 0x18 (0x1c, 0xbc),
 * 0x19 / 0x1a (4, 0xaa).  Digits 0..19 and the separators are placed by ExtraBoss_DrawHud. */
void ExtraBoss_Unk_ov001_022fdbc0(void) {
    s32 i;
    data_ov001_0231f324 = (ExSprite *)calloc(0x1b, sizeof(ExSprite));
    for (i = 0; i < 0x18; i++) {
        ex_sprite_init(&data_ov001_0231f324[i], &data_ov001_0231f320, EX_U16(0x02316e94, i), 0x60);
        data_ov001_0231f324[i].unk44 = 1;
        ex_sprite_process(&data_ov001_0231f324[i]);
    }
    for (; i < 0x1b; i++) {
        ex_sprite_init(&data_ov001_0231f324[i], &data_ov001_0231f320, EX_U16(0x02316e94, i), 0x60);
        data_ov001_0231f324[i].unk44 = 2;
        ex_sprite_process(&data_ov001_0231f324[i]);
    }
    data_ov001_0231f324[0x14].x = 0;
    data_ov001_0231f324[0x14].y = 2;
    data_ov001_0231f324[0x15].x = 0;
    data_ov001_0231f324[0x15].y = 2;
    data_ov001_0231f324[0x16].x = 0x5c;
    data_ov001_0231f324[0x16].y = 0x12;
    data_ov001_0231f324[0x19].x = 4;
    data_ov001_0231f324[0x19].y = 0xaa;
    data_ov001_0231f324[0x1a].x = 4;
    data_ov001_0231f324[0x1a].y = 0xaa;
    data_ov001_0231f324[0x18].x = 0x1c;
    data_ov001_0231f324[0x18].y = 0xbc;
}
void ExtraBoss_FreeObjVramSlots(void) {
    free(data_ov001_0231f324);
    data_ov001_0231f324 = NULL;
}

static s32 s_hud_ring_blink, s_hud_time_blink; /* G + 0x798 / 0x79c */
void ExtraBoss_DrawHud(void) {
    ExSprite *G = data_ov001_0231f324;
    u16 d[7];
    s32 i, div, n, off, t, r, lives;
    s16 x;
    ExSprite *e;
    n = ExtraBoss_GetWork()->unk4C;
    if (n > 999)
        n = 999;
    else if (n < 0)
        n = 0;
    off = 0;
    if (n <= 10) {
        s_hud_ring_blink += 1;
        if (s_hud_ring_blink & 8)
            off = 10;
    }
    for (i = 0, x = 0x1c, div = 100; i < 3; i++, div /= 10) {
        e = &G[off + FX_DivS32(n, div)];
        e->x = x;
        e->y = 0x12;
        ex_sprite_draw(e);
        n = FX_ModS32(n, div);
        x += 10;
    }
    ex_sprite_draw(&G[0x14]);
    ex_sprite_draw(&G[0x15]);
    t = ExtraBoss_GetWork()->unk44;
    off = 0;
    if (t > 0x7e90) {
        s_hud_time_blink += 1;
        if (s_hud_time_blink & 8)
            off = 10;
    }
    if (t > 0x87b3 && t < 0x8c9f && FX_ModS32(t, 0x78) == 0)
        Sound_PlaySeqArc(0, 0, 0x36);
    if (t < 0x8c9f) {
        d[0] = (u16)(off + FX_DivS32(t, 0xe10));
        t = FX_ModS32(t, 0xe10);
        d[2] = (u16)(off + FX_DivS32(t, 0x258));
        t = FX_ModS32(t, 0x258);
        d[3] = (u16)(off + FX_DivS32(t, 0x3c));
        r = FX_ModS32(t, 0x3c) * 100 / 60;
        d[5] = (u16)(off + FX_DivS32(r, 10));
        r = FX_ModS32(r, 10);
        d[6] = (u16)(r + off);
    } else {
        u16 v = (u16)(off + 9);
        d[0] = v;
        d[2] = (u16)(off + 5);
        d[3] = v;
        d[5] = v;
        d[6] = v;
    }
    d[1] = 0x17;
    d[4] = 0x17;
    ex_sprite_draw(&G[0x16]);
    for (i = 0; i < 7; i++) {
        e = &G[d[i]];
        e->x = (s16)EX_U16(0x02316e84, i);   /* s16 data_ov001_02316e84[] */
        e->y = 0x12;
        ex_sprite_draw(e);
    }
    lives = ExtraBoss_GetWork()->unk60[ExtraBoss_GetWork()->unk3C];
    if (lives > 9)
        lives = 9;
    else if (lives < 0)
        lives = 0;
    ex_sprite_draw(&G[ExtraBoss_GetWork()->unk3C + 0x19]);
    ex_sprite_draw(&G[0x18]);
    e = &G[lives];
    e->x = 0x26;
    e->y = 0xbc;
    ex_sprite_draw(e);
}

/* ----------------------------------------------------------- sub screen */
/* The DS sub screen shows "SONIC / BLAZE VS EGGMAN / EGGMAN NEGA" over the VS portrait
 * (extra_vs_bg_son / _blz.bbg, ac_ex_vs.bac) when a round starts.  The state (window, HDMA slide,
 * sprite scales) is kept; boss_extra_draw.c shows it as a cut-in inside the single view. */
ExSprRes g_ex_vs_sprites;
static void ExtraBoss_Unk_ov001_023062f0(void);
static void ExtraBoss_Unk_ov001_02306244(void);
static void ExtraBoss_Unk_ov001_023061b0(void);
static void ExtraBoss_Unk_ov001_02305f08(void);
static void ExtraBoss_State_Unk_ov001_0230611c(void);
static void ExtraBoss_State_Unk_ov001_02305fbc(void);
static void ExtraBoss_Unk_ov001_02305ee8(void);
static void ExtraBoss_Unk_ov001_02305c84(void);
static void ExtraBoss_Unk_ov001_02305bb8(void);
static void ExtraBoss_Unk_ov001_02305d40(void);
static void ExtraBoss_Unk_ov001_02305cd8(void);
static void ExtraBoss_Unk_ov001_02305aec(void);
static void ExtraBoss_Unk_ov001_023059b4(void);
static void ExtraBoss_State_Unk_ov001_0230587c(void);

void ExtraBoss_AllocSubScreenWork(void) {
    s32 i;
    const u16 *pal = EX_DATA(0x0231df2c, const u16 *);
    const s16 *pos = EX_DATA(0x0231df54, const s16 *);
    const u16 *anim;
    data_ov001_0231f5a8 = (ExSubWork *)calloc(1, sizeof(ExSubWork));
    for (i = 0; i <= 3; i++) {
        ExSprite *o = &data_ov001_0231f5a8->unk98[i];
        ex_sprite_init(o, &g_ex_vs_sprites, (u16)i, 0x60);
        o->unk44 = pal[i];
        o->x = pos[2 * i];
        o->y = pos[2 * i + 1];
        ex_sprite_process(o);
    }
    /* data_ov001_0231df3c[gGameLanguage]: the "VS" letters (the same table for every language) */
    anim = EX_DATA(EX_DATA(0x0231df3c, const u32 *)[1], const u16 *);
    for (i = 4; i <= 7; i++) {
        ExSprite *o = &data_ov001_0231f5a8->unk98[i];
        ex_sprite_init(o, &g_ex_vs_sprites, anim[i - 4], 0x260);
        o->unk44 = pal[i];
        o->x = pos[2 * i];
        o->y = pos[2 * i + 1];
        ex_sprite_process(o);
    }
}
void ExtraBoss_StopSubScreenTasks(void) {
    s32 i;
    if (data_ov001_0231f5a8 == NULL)
        return;
    for (i = 0; i < 5; i++) {
        if (data_ov001_0231f5a8->unk4[i] != NULL) {
            ex_task_destroy(data_ov001_0231f5a8->unk4[i]);
            data_ov001_0231f5a8->unk4[i] = NULL;
        }
    }
    data_ov001_0231f5a8->unk0 = 0;   /* DISPCNT sub: nothing shown */
    data_ov001_0231f5a8->hdma_on = 0;
}
void ExtraBoss_FreeSubScreenWork(void) {
    ExtraBoss_StopSubScreenTasks();
    free(data_ov001_0231f5a8);
    data_ov001_0231f5a8 = NULL;
}

/* ExtraBoss_Unk_ov001_02306424 (StartBattle): portrait BG of the character, sprite task, window */
void ExtraBoss_Unk_ov001_02306424(void) {
    s32 k = ExtraBoss_GetWork()->unk3C;
    s32 i;
    ExSubWork *s = data_ov001_0231f5a8;
    s->unk0 = 0;
    s->bg = k;
    for (i = 0; i < 0xc0; i++) {
        s->unk2D8[0][i] = 0x80;
        s->unk2D8[1][i] = 0x80;
    }
    for (i = 0; i < 0x20; i++) {
        s->unk2D8[0][0x3a + i] = 0x100;
        s->unk2D8[0][0x5c + i] = 0;
        s->unk2D8[1][0x3a + i] = 0x100;
        s->unk2D8[1][0x5c + i] = 0;
    }
    s->unk4[0] = ex_task_create(ExtraBoss_Unk_ov001_023062f0, 0x3000, NULL);
    s->name_scale = 0;
    s->text_scale = 0x1000;
    s->cutin_t = 0;
    ExtraBoss_Unk_ov001_02306244();
}
/* draw task: the names (flag 1) and the "VS" letters (flag 2) are drawn every frame (modified: the
 * cut-in renderer in boss_extra_draw.c reads the flags and the sprite state) */
static void ExtraBoss_Unk_ov001_023062f0(void) {
    ExSubWork *s = data_ov001_0231f5a8;
    s->drawn = 1;
    if (s->unk0 & 2)
        s->cutin_t++;
}

/* window 0: a thin band (y 0x5a..0x5c) that opens */
static void ExtraBoss_Unk_ov001_02306244(void) {
    ExSubWork *s = data_ov001_0231f5a8;
    s->unk4[1] = ex_task_create(ExtraBoss_Unk_ov001_023061b0, 0x3100, NULL);
    s->unk5E0 = 0x10;
    s->win[1] = 0;
    s->win[5] = 0x5a;
    s->win[0] = 0;
    s->win[4] = 0x5c;
    s->win[3] = 0xfe;
    s->win[7] = 0x5a;
    s->win[2] = 0;
    s->win[6] = 0x5c;
    s->win[8] = 2;
    s->win[9] = 2;
    s->win[10] = 1;
    s->win[12] |= 1;
}
/* HDMA of the sub BG horizontal scroll: a task swapping the two line buffers every frame */
static void ExtraBoss_Unk_ov001_02305ec8(void) { data_ov001_0231f5a8->unk5DE ^= 1; }
static void ExtraBoss_Unk_ov001_02305f08(void) {
    ExSubWork *s = data_ov001_0231f5a8;
    s->unk4[3] = ex_task_create(ExtraBoss_Unk_ov001_02305ec8, 0x3100, NULL);
    s->unk5DC = 0;
    s->unk5DE = 0;
    s->hdma_on = 1;   /* data_02087cdc: reg 0x04001014, buffers unk2D8 / unk458, gSystemFlags |= 0x400 */
}
static void ExtraBoss_Unk_ov001_023061b0(void) {
    ExSubWork *s = data_ov001_0231f5a8;
    s->unk5E0--;
    if (s->unk5E0 > 0) {
        s->win[0] = (u8)FX_DivS32((0x10 - s->unk5E0) << 8, 0x10);
        return;
    }
    gTaskCurrent->fn = ExtraBoss_State_Unk_ov001_0230611c;
    s->win[0] = 0xff;
    s->win[12] |= 2;
    s->unk5E0 = 0;
    ExtraBoss_Unk_ov001_02305f08();
}
static void ExtraBoss_State_Unk_ov001_0230611c(void) {
    ExSubWork *w = data_ov001_0231f5a8;
    w->unk5E0++;
    w->win[5] = w->win[7] = (u8)(w->win[5] - w->unk5E0);
    w->win[4] = w->win[6] = (u8)(w->win[4] + w->unk5E0);
    if (w->unk5E0 < 4)
        return;
    gTaskCurrent->fn = ExtraBoss_State_Unk_ov001_02305fbc;
    w->unk5E4 = 0;
}
static void ExtraBoss_State_Unk_ov001_02305fbc(void) {
    ExSubWork *s = data_ov001_0231f5a8;
    s32 v, acc, i;
    s->unk5E4 = (s16)(s->unk5E4 + 0x199);
    v = s->unk5E4;
    if (v < 0x1000) {
        i = 3;
        acc = 0x56;
        do {
            acc += FX_Mul(0 - acc, v);
        } while (i-- != 0);
        s->win[7] = (u8)acc;
        s->win[5] = s->win[7];
        i = 3;
        acc = 0x60;
        do {
            acc += FX_Mul(0xc0 - acc, v);
        } while (i-- != 0);
        s->win[6] = (u8)acc;
        s->win[4] = s->win[6];
        return;
    }
    ex_task_destroy(gTaskCurrent);
    s->unk4[1] = NULL;
    s->win[12] = 0;
    ExtraBoss_Unk_ov001_02305ee8();
    ExtraBoss_Unk_ov001_02305c84();
}
/* HDMA slide of the two portrait halves */
static void ExtraBoss_Unk_ov001_02305ee8(void) {
    if (data_ov001_0231f5a8->unk4[3] != NULL)
        data_ov001_0231f5a8->unk4[3]->fn = ExtraBoss_Unk_ov001_02305d40;
}
static void ExtraBoss_Unk_ov001_02305d40(void) {
    ExSubWork *s = data_ov001_0231f5a8;
    s32 v, i, x;
    s16 c;
    s->unk5DC = (s16)(s->unk5DC + 0x111);
    v = s->unk5DC;
    if (v < 0x1000) {
        i = 3;
        x = 0;
        do {
            x = FX_Mul(x - 0x80, v) + 0x80;
        } while (i-- != 0);
        c = (s16)x;
        for (i = 0; i < 0x20; i++)
            s->unk2D8[s->unk5DE ^ 1][0x3a + i] = (s16)(c + 0x80);
        for (i = 0; i < 0x20; i++)
            s->unk2D8[s->unk5DE ^ 1][0x5c + i] = (s16)(0x80 - c);
        s->unk5DE ^= 1;
        return;
    }
    gTaskCurrent->fn = ExtraBoss_Unk_ov001_02305cd8;
    s->unk5D8 = 10;
    s->hdma_on = 0;
    for (i = 0; i < 0xc0; i++)
        s->unk2D8[0][i] = s->unk2D8[1][i] = 0x80;   /* gDisplaySub BG HOFS 0x80 */
}
static void ExtraBoss_Unk_ov001_02305cd8(void) {
    data_ov001_0231f5a8->unk5D8 -= 1;
    if (data_ov001_0231f5a8->unk5D8 > 0)
        return;
    ex_task_destroy(gTaskCurrent);
    data_ov001_0231f5a8->unk4[3] = NULL;
    ExtraBoss_Unk_ov001_02305aec();
}
/* names: vertical scale 0 -> 1 */
static void ExtraBoss_Unk_ov001_02305c84(void) {
    data_ov001_0231f5a8->unk4[2] = ex_task_create(ExtraBoss_Unk_ov001_02305bb8, 0x3100, NULL);
    data_ov001_0231f5a8->unk5EC = 0;
}
static void ExtraBoss_Unk_ov001_02305bb8(void) {
    ExSubWork *s = data_ov001_0231f5a8;
    s->unk5EC += 0x199;
    if (s->unk5EC >= 0x1000) {
        ex_task_destroy(gTaskCurrent);
        s->unk4[2] = NULL;
        s->unk0 |= 1;
        s->unk5EC = 0x1000;
    }
    s->name_scale = s->unk5EC;   /* AnimatorSprite_DrawRotScale(objs[c] / objs[c + 2], 0x1000, t, 0) */
}
/* "VS": zoom 2 -> 1, then shake */
static void ExtraBoss_Unk_ov001_02305aec(void) {
    s32 i;
    data_ov001_0231f5a8->unk4[4] = ex_task_create(ExtraBoss_Unk_ov001_023059b4, 0x3100, NULL);
    data_ov001_0231f5a8->unk5E6 = 0;
    for (i = 4; i <= 7; i++) {
        data_ov001_0231f5a8->unk98[i].x = 0x80;
        data_ov001_0231f5a8->unk98[i].y = 0x5b;
    }
    if (ExtraBoss_GetWork()->unk3C == 0)
        Sound_PlayVoice(0, 0, 0x22);
    else
        Sound_PlayVoice(0, 0, 0x2b);
}
static void ExtraBoss_Unk_ov001_023059b4(void) {
    ExSubWork *s = data_ov001_0231f5a8;
    s32 i, v, t;
    s->unk5E6 = (s16)(s->unk5E6 + 0x199);
    if (s->unk5E6 >= 0x1000) {
        gTaskCurrent->fn = ExtraBoss_State_Unk_ov001_0230587c;
        s->unk5E8 = 0x10;
        s->unk5E6 = 0x1000;
    }
    i = 3;
    v = 0x1000;
    t = s->unk5E6;
    do {
        v = FX_Mul(v - 0x2000, t) + 0x2000;
    } while (i-- != 0);
    s->text_scale = v;   /* AnimatorSprite_DrawRotScale(objs[4..7], v, v, 0) */
}
static void ExtraBoss_State_Unk_ov001_0230587c(void) {
    ExSubWork *s = data_ov001_0231f5a8;
    s16 x, y;
    s32 i, r;
    s->unk5E8--;
    if (s->unk5E8 > 0) {
        r = s->unk5E8 * 0x500;
        x = (s16)(((FX_ModS32(Rand(), r) - (r >> 1)) >> 12) + 0x80);
        y = (s16)(((FX_ModS32(Rand(), r) - (r >> 1)) >> 12) + 0x5b);
    } else {
        ex_task_destroy(gTaskCurrent);
        x = 0x80;
        s->unk4[4] = NULL;
        y = 0x5b;
        s->unk0 |= 2;
    }
    s->text_scale = 0x1000;
    for (i = 4; i <= 7; i++) {
        s->unk98[i].x = x;
        s->unk98[i].y = y;
    }
}

/* ExtraBoss_SetupSubDisplay / ExtraBoss_ResetSubDisplay: the dual-screen 3D mode of the character
 * switch (ExtraBoss_UpdateDualScreen3D alternates the camera slots 1 / 2 every frame).  The single
 * view renders both slots' content with one composed camera (boss_extra_draw.c, slot 3). */
static void ExtraBoss_UpdateDualScreen3D(void) {
    ExWork *g = gExtraBossWork;
    g->unk338 ^= 1;
    if (g->unk338 != 0) {
        g->unk4 = 1;
        g->unk320 = 0x60000;
    } else {
        g->unk4 = 2;
        g->unk320 = 0x120000;
    }
}
void ExtraBoss_SetupSubDisplay(BOOL on) {
    if (on) {
        gExtraBossWork->unk338 = 1;
        gExtraBossWork->unk4 = 1;
    } else {
        gExtraBossWork->unk338 = 0;
        gExtraBossWork->unk4 = 2;
    }
    gExtraBossWork->unk14 = ex_task_create(ExtraBoss_UpdateDualScreen3D, 0xf000, NULL);
    gExtraBossWork->unk0 |= 1;
}
void ExtraBoss_ResetSubDisplay(void) {
    gExtraBossWork->unk4 = 0;
    if (gExtraBossWork->unk14 != NULL) {
        ex_task_destroy(gExtraBossWork->unk14);
        gExtraBossWork->unk14 = NULL;
    }
    gExtraBossWork->unk0 &= ~1u;
}
/* ExtraBoss_Unk_ov001_022ffcf8 (CharacterSwitch, flag 0x400): back to one screen, boss parts gone */
void ExtraBoss_Unk_ov001_022ffcf8(void) {
    ExtraBoss_ResetSubDisplay();
    ExtraBossObj_DestroyKind2();
    gExtraBossWork->unk0 |= 4;
}
