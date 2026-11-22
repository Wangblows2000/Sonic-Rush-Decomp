/*
 * .srb sound bank loader (bank built by tools/dreamcast/audio/sfxbank.py).
 * Replaces the DS sound archive loading (NNS_SndArcInit / NNS_SndArcLoadGroup:
 * StageLoader_LoadZoneSoundData + SysSound_LoadSEGroup): the SSAR sequences,
 * SBNK instruments and tables stay in main RAM, the wave blob goes to sound RAM.
 */
#include "snd_internal.h"
#include <stdlib.h>
#include <string.h>

int snd_bank_load(SndBank *b, const char *path) {
    SrbHeader *h;
    uint8_t *blob;
    memset(b, 0, sizeof *b);
    h = (SrbHeader *)snd_out_read_file(path, 0, sizeof(SrbHeader));
    if (!h) return -1;
    if (h->magic != SRB_MAGIC || h->version != SRB_VERSION || h->off_wavedata < sizeof(SrbHeader)) { free(h); return -2; }
    {
        uint32_t meta = h->off_wavedata, blob_size = h->wavedata_size;
        free(h);
        b->meta = (uint8_t *)snd_out_read_file(path, 0, meta);
        if (!b->meta) return -3;
        blob = (uint8_t *)snd_out_read_file(path, meta, blob_size);
        if (!blob) { free(b->meta); b->meta = NULL; return -3; }
        b->hdr = (const SrbHeader *)b->meta;
        b->players = (const SrbPlayer *)(b->meta + b->hdr->off_players);
        b->ssar = (const SrbSsar *)(b->meta + b->hdr->off_ssar);
        b->banks = (const SrbBank *)(b->meta + b->hdr->off_banks);
        b->waves = (const SrbWave *)(b->meta + b->hdr->off_waves);
        b->tab = (const SrbTables *)(b->meta + b->hdr->off_tables);
        b->blob_size = blob_size;
        if (snd_out_upload(b, blob, blob_size)) { free(blob); free(b->meta); memset(b, 0, sizeof *b); return -4; }
        free(blob);
    }
    return 0;
}

void snd_bank_free(SndBank *b) {
    if (!b->meta) return;
    snd_out_release(b);
    free(b->meta);
    memset(b, 0, sizeof *b);
}

/* NNSi_SndSeqArcGetSeqInfo: SSAR record `index` of archive `ssar` (NULL if empty) */
const SrbSeqInfo *snd_bank_seq_info(const SndBank *b, int ssar, int index) {
    const SrbSsar *a;
    const SrbSeqInfo *r;
    if (!b->meta || ssar < 0 || (uint32_t)ssar >= b->hdr->n_ssar) return NULL;
    a = &b->ssar[ssar];
    if (index < 0 || (uint32_t)index >= a->n_seq) return NULL;
    r = (const SrbSeqInfo *)(b->meta + a->off_rec) + index;
    return r->offset == 0xFFFFFFFFu ? NULL : r;
}

const uint8_t *snd_bank_seq_data(const SndBank *b, int ssar, uint32_t *size) {
    if (!b->meta || ssar < 0 || (uint32_t)ssar >= b->hdr->n_ssar) return NULL;
    *size = b->ssar[ssar].data_size;
    return b->meta + b->ssar[ssar].off_data;
}

const uint8_t *snd_bank_sbnk(const SndBank *b, int bank, uint32_t *size) {
    if (!b->meta || bank < 0 || (uint32_t)bank >= b->hdr->n_banks || !b->banks[bank].size) return NULL;
    *size = b->banks[bank].size;
    return b->meta + b->banks[bank].off;
}
