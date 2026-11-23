/*
 * NitroSystem sound player layer, native. Reused from the matched
 * decompilation in src/libs/NitroSystem/snd/ (same names, same logic); the DS
 * services are replaced:
 *   NNS_Fnd* list            -> native intrusive list below (same semantics:
 *                               GetNext(list, NULL) = head, Insert before obj,
 *                               NULL = append)
 *   SND_* driver commands    -> direct calls into snd_drv.c (synchronous)
 *   SND_IsFinishedCommandTag -> seqId != 0 (set when the start was issued)
 *   sound archive / heap     -> resident .srb bank (snd_bank.c); heaps unused
 */
#include "snd_nns.h"
#include <string.h>

/* ------------------------------------------------------------ NNS_Fnd list */
#define LINK(list, obj) ((NNSFndLink *)((uint8_t *)(obj) + (list)->offset))
void NNS_FndInitList(NNSFndList *list, uint16_t offset) {
    list->head = list->tail = NULL; list->numObjects = 0; list->offset = offset;
}
void NNS_FndAppendListObject(NNSFndList *list, void *obj) {
    NNSFndLink *l = LINK(list, obj);
    l->next = NULL; l->prev = list->tail;
    if (list->tail) LINK(list, list->tail)->next = obj; else list->head = obj;
    list->tail = obj; list->numObjects++;
}
void NNS_FndInsertListObject(NNSFndList *list, void *before, void *obj) {
    NNSFndLink *l, *b;
    if (!before) { NNS_FndAppendListObject(list, obj); return; }
    l = LINK(list, obj); b = LINK(list, before);
    l->next = before; l->prev = b->prev;
    if (b->prev) LINK(list, b->prev)->next = obj; else list->head = obj;
    b->prev = obj; list->numObjects++;
}
void NNS_FndRemoveListObject(NNSFndList *list, void *obj) {
    NNSFndLink *l = LINK(list, obj);
    if (l->prev) LINK(list, l->prev)->next = l->next; else if (list->head == obj) list->head = l->next; else return;
    if (l->next) LINK(list, l->next)->prev = l->prev; else list->tail = l->prev;
    l->prev = l->next = NULL; list->numObjects--;
}
void *NNS_FndGetNextListObject(NNSFndList *list, void *obj) {
    return obj ? LINK(list, obj)->next : list->head;
}

/* ------------------------------------------------------------ state */
static const SndBank *s_bank;
static NNSFndList data_0208344c;              /* free seq players */
static NNSFndList data_02083458;              /* seq players by priority (ascending) */
static NNSSndSeqPlayer data_02083464[16];     /* seq players */
static NNSSndPlayer data_020838a4[32];        /* players */

/* ------------------------------------------------------------ faders (NNSi_SndFader*) */
static void NNSi_SndFaderInit(NNSSndFader *fader) { fader->origin = fader->target = 0; fader->counter = fader->frame = 0; }
static int32_t NNSi_SndFaderGet(NNSSndFader *f) {
    if (f->counter >= f->frame) return f->target;
    return f->origin + (f->target - f->origin) * f->counter / f->frame;
}
static void NNSi_SndFaderSet(NNSSndFader *f, int32_t target, int32_t frame) {
    f->origin = NNSi_SndFaderGet(f); f->target = target; f->frame = frame; f->counter = 0;
}
static void NNSi_SndFaderUpdate(NNSSndFader *fader) { if (fader->counter < fader->frame) fader->counter++; }
static BOOL_ NNSi_SndFaderIsFinished(NNSSndFader *fader) { return fader->counter >= fader->frame; }

/* ------------------------------------------------------------ handles */
void NNS_SndHandleInit(NNSSndHandle *handle) { handle->player = NULL; }
void NNS_SndHandleReleaseSeq(NNSSndHandle *handle) {
    NNSSndSeqPlayer *p = handle->player;
    if (p != NULL) { p->handle = NULL; handle->player = NULL; }
}

/* ------------------------------------------------------------ player lists */
static void InsertPlayerList(NNSSndPlayer *heap, NNSSndSeqPlayer *p) {
    NNSSndSeqPlayer *it = NNS_FndGetNextListObject(&heap->playerList, NULL);
    while (it != NULL) {
        if (p->prio < it->prio) break;
        it = NNS_FndGetNextListObject(&heap->playerList, it);
    }
    NNS_FndInsertListObject(&heap->playerList, it, p);
    p->player = heap;
}
static void InsertPrioList(NNSSndSeqPlayer *p) {
    NNSSndSeqPlayer *it = NNS_FndGetNextListObject(&data_02083458, NULL);
    while (it != NULL) {
        if (p->prio < it->prio) break;
        it = NNS_FndGetNextListObject(&data_02083458, it);
    }
    NNS_FndInsertListObject(&data_02083458, it, p);
}
void (*g_snd_seq_end_cb)(int32_t arc, int32_t id, int tries, int ok);
static void ShutdownPlayer_snd_player(NNSSndSeqPlayer *p) {
    NNSSndPlayer *heap;
    if (p->handle != NULL) { p->handle->player = NULL; p->handle = NULL; }
    heap = p->player;
    NNS_FndRemoveListObject(&heap->playerList, p);
    p->player = NULL;
    /* (player heap release: no heaps natively) */
    NNS_FndRemoveListObject(&data_02083458, p);
    NNS_FndAppendListObject(&data_0208344c, p);
    p->status = 0;
}
static void ForceStopSeq(NNSSndSeqPlayer *p) {
    if (p->status == 2) snd_drv_set_player_volume(p->playerNo, -723);
    snd_drv_stop_seq(p->playerNo);
    ShutdownPlayer_snd_player(p);
}
static void SetPlayerPriority(NNSSndSeqPlayer *p, uint8_t prio) {
    NNSSndPlayer *heap = p->player;
    if (heap != NULL) { NNS_FndRemoveListObject(&heap->playerList, p); p->player = NULL; }
    NNS_FndRemoveListObject(&data_02083458, p);
    p->prio = prio;
    if (heap != NULL) InsertPlayerList(heap, p);
    InsertPrioList(p);
}
static NNSSndSeqPlayer *AllocSeqPlayer(int32_t prio) {
    NNSSndSeqPlayer *p = NNS_FndGetNextListObject(&data_0208344c, NULL);
    if (p == NULL) {
        p = NNS_FndGetNextListObject(&data_02083458, NULL);
        if (prio < p->prio) return NULL;
        ForceStopSeq(p);
    }
    NNS_FndRemoveListObject(&data_0208344c, p);
    p->prio = (uint8_t)prio;
    InsertPrioList(p);
    return p;
}
static NNSSndSeqPlayer *NNSi_SndPlayerAllocSeqPlayer(NNSSndHandle *handle, int32_t playerNo, int32_t prio) {
    NNSSndPlayer *heap = &data_020838a4[playerNo];
    NNSSndSeqPlayer *p;
    if (handle->player != NULL) NNS_SndHandleReleaseSeq(handle);
    if (heap->playerList.numObjects >= heap->playableSeqCount) {
        p = NNS_FndGetNextListObject(&heap->playerList, NULL);
        if (p == NULL) return NULL;
        if (prio < p->prio) return NULL;
        ForceStopSeq(p);
    }
    p = AllocSeqPlayer(prio);
    if (p == NULL) return NULL;
    InsertPlayerList(heap, p);
    p->handle = handle;
    handle->player = p;
    return p;
}
static void NNSi_SndPlayerFreeSeqPlayer(NNSSndSeqPlayer *p) { ShutdownPlayer_snd_player(p); }
static void InitPlayer(NNSSndSeqPlayer *p) {
    p->unk2e = 0; p->prepared = 0; p->pauseReq = 0; p->seqArcNo = 0; p->volume = 0;
    p->volume1 = 0x7f; p->volume2 = 0x7f;
    NNSi_SndFaderInit(&p->fader);
    NNSi_SndFaderSet(&p->fader, 0x7f00, 1);
}
static void NNSi_SndPlayerStartSeq(NNSSndSeqPlayer *p, const uint8_t *data, uint32_t dataSize, uint32_t offset, int bankNo) {
    NNSSndPlayer *heap = p->player;
    snd_drv_prepare_seq(p->playerNo, data, dataSize, offset, bankNo); /* SND_PrepareSeq */
    if (heap->allocChBitFlag != 0) snd_drv_set_alloc_mask(p->playerNo, (uint16_t)heap->allocChBitFlag);
    InitPlayer(p);
    p->seqId = 0; /* SND_GetCurrentCommandTag: finished once the start has been issued */
    p->pauseReq = 1;
    p->status = 1;
}
static void NNSi_SndPlayerStopSeq(NNSSndSeqPlayer *p, int32_t frames) {
    if (p == NULL) return;
    if (p->status == 0) return;
    if (frames == 0) { ForceStopSeq(p); return; }
    NNSi_SndFaderSet(&p->fader, 0, frames);
    SetPlayerPriority(p, 0);
    p->status = 2;
}

/* ------------------------------------------------------------ public */
void NNSi_SndPlayerInit(void) {
    int32_t i;
    NNS_FndInitList(&data_02083458, (uint16_t)offsetof(NNSSndSeqPlayer, prioLink));
    NNS_FndInitList(&data_0208344c, (uint16_t)offsetof(NNSSndSeqPlayer, prioLink));
    memset(data_02083464, 0, sizeof data_02083464);
    for (i = 0; i < 16; i++) {
        NNSSndSeqPlayer *p = &data_02083464[i];
        p->status = 0;
        p->playerNo = (uint8_t)i;
        NNS_FndAppendListObject(&data_0208344c, p);
    }
    for (i = 0; i < 32; i++) {
        NNSSndPlayer *h = &data_020838a4[i];
        NNS_FndInitList(&h->playerList, (uint16_t)offsetof(NNSSndSeqPlayer, playerLink));
        h->volume = 0x7f;
        h->playableSeqCount = 1;
        h->allocChBitFlag = 0;
    }
}

/* NNS_SndArcPlayerSetup: player info from the archive (heaps not needed) */
void NNS_SndArcPlayerSetup(const SndBank *bank) {
    uint32_t i;
    s_bank = bank;
    for (i = 0; i < 32 && i < bank->hdr->n_players; i++) {
        data_020838a4[i].playableSeqCount = bank->players[i].max_seq;  /* NNS_SndPlayerSetPlayableSeqCount */
        data_020838a4[i].allocChBitFlag = bank->players[i].alloc_mask; /* NNS_SndPlayerSetAllocatableChannel */
    }
}

void NNSi_SndPlayerMain(void) {
    NNSSndSeqPlayer *p, *next;
    int32_t vol, v2, v1, v0;
    const int16_t *db = s_bank->tab->db; /* data_020725e0 */
    p = NNS_FndGetNextListObject(&data_02083458, NULL);
    if (p == NULL) return;
    do {
        next = NNS_FndGetNextListObject(&data_02083458, p);
        if (!p->prepared) {
            if (p->seqId) p->prepared = 1;
        }
        if (p->prepared && !snd_drv_player_active(p->playerNo)) {
            if (g_snd_seq_end_cb && p->status == 1) {   /* trace: the sequence ended by itself */
                int tries, ok;
                snd_drv_player_notes(p->playerNo, &tries, &ok);
                g_snd_seq_end_cb(p->seqArcNo, p->seqNo, tries, ok);
            }
            ShutdownPlayer_snd_player(p);
        } else {
            NNSi_SndFaderUpdate(&p->fader);
            v2 = db[p->volume2];
            v1 = db[p->volume1];
            v0 = db[p->player->volume];
            vol = db[NNSi_SndFaderGet(&p->fader) >> 8];
            vol += v1 + v2 + v0;
            if (vol < -723) vol = -723;
            else if (vol > 0) vol = 0;
            if (vol != p->volume) {
                snd_drv_set_player_volume(p->playerNo, vol);
                p->volume = (int16_t)vol;
            }
            if (p->status == 2 && NNSi_SndFaderIsFinished(&p->fader)) ForceStopSeq(p);
            if (p->pauseReq) {
                snd_drv_start_prepared(p->playerNo); /* SND_StartPreparedSeq */
                p->seqId = 1;
                p->pauseReq = 0;
            }
        }
        p = next;
    } while (p != NULL);
}

static BOOL_ StartSeqArc(NNSSndHandle *handle, uint32_t playerNo, int32_t bankNo, int32_t prio,
                         const SrbSeqInfo *info, int32_t a, int32_t b) {
    NNSSndSeqPlayer *seq;
    const uint8_t *data;
    uint32_t size, bsize;
    if (playerNo >= 32) return 0;
    seq = NNSi_SndPlayerAllocSeqPlayer(handle, (int32_t)playerNo, prio);
    if (seq == NULL) return 0;
    /* NNSi_SndPlayerAllocHeap / NNSi_SndArcLoadBank: the bank is resident */
    if (snd_bank_sbnk(s_bank, bankNo, &bsize) == NULL) {
        NNSi_SndPlayerFreeSeqPlayer(seq);
        return 0;
    }
    data = snd_bank_seq_data(s_bank, a, &size);
    NNSi_SndPlayerStartSeq(seq, data, size, info->offset, bankNo);
    NNS_SndPlayerSetInitialVolume(handle, info->volume);
    NNS_SndPlayerSetChannelPriority(handle, info->channel_prio);
    seq->seqArcNo = a; seq->seqNo = b; /* NNS_SndPlayerSetSeqArcNo */
    return 1;
}

BOOL_ NNS_SndArcPlayerStartSeqArcEx(NNSSndHandle *handle, int32_t playerNo, int32_t bankNo, int32_t prio,
                                    int32_t seqArcNo, int32_t index) {
    const SrbSeqInfo *info;
    if (!s_bank || !s_bank->meta) return 0;
    info = snd_bank_seq_info(s_bank, seqArcNo, index); /* NNS_SndArcGetSeqArcInfo + NNSi_SndSeqArcGetSeqInfo */
    if (info == NULL) return 0;
    if (prio < 0) prio = info->player_prio;
    if (bankNo < 0) bankNo = info->bank;
    if (playerNo < 0) playerNo = info->player;
    return StartSeqArc(handle, (uint32_t)playerNo, bankNo, prio, info, seqArcNo, index);
}

BOOL_ NNS_SndArcPlayerStartSeqArc(NNSSndHandle *handle, int32_t seqArcNo, int32_t index) {
    const SrbSeqInfo *info;
    if (!s_bank || !s_bank->meta) return 0;
    info = snd_bank_seq_info(s_bank, seqArcNo, index);
    if (info == NULL) return 0;
    return StartSeqArc(handle, info->player, info->bank, info->player_prio, info, seqArcNo, index);
}

void NNS_SndPlayerStopSeq(NNSSndHandle *handle, int32_t fadeFrames) { NNSi_SndPlayerStopSeq(handle->player, fadeFrames); }
void NNS_SndPlayerStopSeqAll(int32_t fadeFrames) {
    int32_t i;
    for (i = 0; i < 16; i++) if (data_02083464[i].status) NNSi_SndPlayerStopSeq(&data_02083464[i], fadeFrames);
}
/* NNS_SndPlayerCountPlayingSeqBySeqArcIdx: players running sequence <index> of sequence archive <seqArcNo> */
int32_t NNS_SndPlayerCountPlayingSeqBySeqArcIdx(int32_t seqArcNo, int32_t index) {
    int32_t i, n = 0;
    for (i = 0; i < 16; i++)
        if (data_02083464[i].status && data_02083464[i].seqArcNo == seqArcNo && data_02083464[i].seqNo == index)
            n++;
    return n;
}
/* NNS_SndPlayerStopSeqBySeqArcIdx: every player running sequence <index> of sequence archive <seqArcNo> */
void NNS_SndPlayerStopSeqBySeqArcIdx(int32_t seqArcNo, int32_t index, int32_t fadeFrames) {
    int32_t i;
    for (i = 0; i < 16; i++)
        if (data_02083464[i].status && data_02083464[i].seqArcNo == seqArcNo && data_02083464[i].seqNo == index)
            NNSi_SndPlayerStopSeq(&data_02083464[i], fadeFrames);
}
/* native: volumes clamped to 0..127 (the DS indexes the 128-entry table unchecked) */
void NNS_SndPlayerSetVolume(NNSSndHandle *handle, int volume) { if (handle->player) handle->player->volume2 = (uint8_t)(volume < 0 ? 0 : volume > 127 ? 127 : volume); }
void NNS_SndPlayerSetInitialVolume(NNSSndHandle *handle, int volume) { if (handle->player) handle->player->volume1 = (uint8_t)(volume < 0 ? 0 : volume > 127 ? 127 : volume); }
void NNS_SndPlayerSetChannelPriority(NNSSndHandle *handle, int prio) {
    if (handle->player) snd_drv_set_player_prio(handle->player->playerNo, prio);
}
void NNS_SndPlayerSetTrackPan(NNSSndHandle *handle, uint32_t trackMask, int pan) {
    if (handle->player) snd_drv_set_track_pan(handle->player->playerNo, trackMask, pan);
}
void NNS_SndPlayerSetTempoRatio(NNSSndHandle *handle, int ratio) {
    if (handle->player) snd_drv_set_tempo_ratio(handle->player->playerNo, ratio);
}
int NNS_SndPlayerCountPlaying(int playerNo) { return data_020838a4[playerNo].playerList.numObjects; }

/* NNSi_SndPlayerPause / NNS_SndPlayerPause / NNS_SndPlayerPauseByPlayerNo
 * (src/libs/NitroSystem/snd): unk2e is the pause flag (+0x2e) */
void NNSi_SndPlayerPause(NNSSndSeqPlayer *p, int flag) {
    if (p == NULL) return;
    if ((uint8_t)flag == p->unk2e) return;
    snd_drv_pause_seq(p->playerNo, flag);   /* SND_PauseSeq */
    p->unk2e = (uint8_t)flag;
}
void NNS_SndPlayerPause(NNSSndHandle *handle, int flag) { NNSi_SndPlayerPause(handle->player, flag); }
void NNS_SndPlayerPauseByPlayerNo(int playerNo, int flag) {
    NNSSndPlayer *heap = &data_020838a4[playerNo];
    NNSSndSeqPlayer *p = NNS_FndGetNextListObject(&heap->playerList, NULL), *next;
    if (p == NULL) return;
    do {
        next = NNS_FndGetNextListObject(&heap->playerList, p);
        NNSi_SndPlayerPause(p, flag);
        p = next;
    } while (next != NULL);
}
