/*
 * Sound output on the AICA (KallistiOS). The 16 driver channels map to 16 AICA
 * channels reserved with snd_sfx_chn_alloc (so snd_stream / snd_sfx never use
 * them); channels are programmed with the KOS AICA command queue
 * (AICA_CMD_CHAN: START / STOP / UPDATE freq, vol, pan), the same path
 * snd_sfx_play_ex uses. Waves live in one snd_mem block per bank.
 * Requires snd_init() (done by snd_stream_init() in engine/audio.c audio_init).
 */
#include <string.h>
#include "../engine/pak.h"
#include "snd_internal.h"
#include <kos.h>
#include <dc/sound/sound.h>
#include <dc/sound/sfxmgr.h>
#include <dc/sound/aica_comm.h>
#include <dc/spu.h>
#include <dc/g2bus.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

/* q16 linear amplitude -> KOS linear volume 0..255 (KOS maps it through its
 * logs[] table to the TL register) */
static int kos_vol(int q16) {
    int v = (q16 * 255 + 32767) / 65535;
    return q16 > 0 && v == 0 ? 1 : v;
}
/* q16 linear amplitude -> AICA TL (attenuation, 0.375 dB = 1/16 octave steps) */
static int aica_tl(int q16) {
    float t;
    if (q16 <= 0) return 255;
    t = -16.0f * log2f((float)q16 / 65535.0f) + 0.5f;
    return t > 255.0f ? 255 : (int)t;
}
/* Volume changes after key-on are written straight to the channel's TL
 * register from the SH-4 (AICA reg 0x28: TL << 8 | 0x24, the LPF-off value
 * KOS aica_play writes): the KOS UPDATE command only takes the 8-bit linear
 * volume, whose steps are too coarse below -30 dB (vol 1 = -48 dB, 0 = off)
 * for DS envelope tails. KOS never touches the register of a reserved channel
 * except on commands we send; a START is processed by the ARM long before the
 * next frame's TL write. */
#define AICA_CHNREG(ch, x) (0xa0700000u + 0x80u * (uint32_t)(ch) + (x))

static int s_chan[SND_CH_NUM];
static int s_ready;

void snd_out_init(void) {
    int i;
    if (s_ready) return;
    snd_init(); /* no-op if snd_stream_init already ran it */
    for (i = 0; i < SND_CH_NUM; i++) s_chan[i] = snd_sfx_chn_alloc();
    s_ready = 1;
}

void snd_out_shutdown(void) {
    int i;
    if (!s_ready) return;
    for (i = 0; i < SND_CH_NUM; i++) if (s_chan[i] >= 0) { snd_sfx_stop(s_chan[i]); snd_sfx_chn_free(s_chan[i]); }
    s_ready = 0;
}

int snd_out_upload(SndBank *b, const void *blob, uint32_t size) {
    uint32_t a = snd_mem_malloc(size);
    if (!a) return -1;
    spu_memload(a, (void *)blob, size);
    b->aram = a;
    return 0;
}

void snd_out_release(SndBank *b) {
    if (b->aram) snd_mem_free(b->aram);
    b->aram = 0;
}

static void aica_send(int ch, uint32_t cmdv, uint32_t base, int fmt, uint32_t samples, int loop, uint32_t loop_start,
                 uint32_t freq, int vol, int pan) {
    static const uint32_t aica_fmt[3] = {AICA_SM_ADPCM, AICA_SM_16BIT, AICA_SM_8BIT};
    AICA_CMDSTR_CHANNEL(tmp, cmd, chan);
    if (s_chan[ch] < 0) return;
    cmd->cmd = AICA_CMD_CHAN;
    cmd->timestamp = 0;
    cmd->size = AICA_CMDSTR_CHANNEL_SIZE;
    cmd->cmd_id = s_chan[ch];
    chan->cmd = cmdv;
    chan->base = base;
    chan->type = aica_fmt[fmt > 2 ? 0 : fmt];
    chan->length = samples > 65534 ? 65534 : samples;
    chan->loop = loop;
    chan->loopstart = loop_start;
    chan->loopend = chan->length;
    chan->freq = freq;
    chan->vol = kos_vol(vol);
    chan->pan = pan;
    snd_sh4_to_aica(tmp, cmd->size);
}

void snd_out_start(int ch, uint32_t aram, int fmt, uint32_t samples, int loop, uint32_t loop_start,
                   uint32_t freq, int vol, int pan) {
    aica_send(ch, AICA_CH_CMD_START, aram, fmt, samples, loop, loop_start, freq, vol, pan);
}

void snd_out_update(int ch, unsigned what, uint32_t freq, int vol, int pan) {
    uint32_t c = AICA_CH_CMD_UPDATE;
    if (s_chan[ch] < 0) return;
    if (what & SND_OUT_VOL) g2_write_32(AICA_CHNREG(s_chan[ch], 0x28), ((uint32_t)aica_tl(vol) << 8) | 0x24);
    if (what & SND_OUT_FREQ) c |= AICA_CH_UPDATE_SET_FREQ;
    if (what & SND_OUT_PAN) c |= AICA_CH_UPDATE_SET_PAN;
    if (c != AICA_CH_CMD_UPDATE) aica_send(ch, c, 0, 0, 0, 0, 0, freq, vol, pan);
}

void snd_out_stop(int ch) {
    if (s_chan[ch] >= 0) snd_sfx_stop(s_chan[ch]);
}

/* A bank file from /cd/data.pak is loaded whole once and its ranges served
 * from RAM: the loader asks for the size, then several ranges (each was an
 * ISO 9660 open + seek on the GD-ROM). */
static char s_pak_path[96];
static void *s_pak_buf;
static uint32_t s_pak_size;
static int pak_cached(const char *path)
{
    void *b;
    uint32_t n;
    if (s_pak_buf && !strcmp(s_pak_path, path)) return 1;
    if (pak_load(path, &b, &n) != PAK_FOUND) return 0;
    free(s_pak_buf);
    s_pak_buf = b; s_pak_size = n;
    strncpy(s_pak_path, path, sizeof(s_pak_path) - 1);
    s_pak_path[sizeof(s_pak_path) - 1] = 0;
    return 1;
}

void *snd_out_read_file(const char *path, uint32_t off, uint32_t size) {
    file_t f;
    void *buf;
    if (pak_cached(path)) {
        if (off > s_pak_size || size > s_pak_size - off) return NULL;
        buf = malloc(size ? size : 1);
        if (buf) memcpy(buf, (uint8_t *)s_pak_buf + off, size);
        return buf;
    }
    f = fs_open(path, O_RDONLY);
    if (f == FILEHND_INVALID) return NULL;
    buf = malloc(size ? size : 1);
    if (!buf || fs_seek(f, off, SEEK_SET) < 0 || fs_read(f, buf, size) != (ssize_t)size) {
        free(buf); fs_close(f); return NULL;
    }
    fs_close(f);
    return buf;
}

int snd_out_file_size(const char *path) {
    file_t f;
    int n;
    if (pak_cached(path)) return (int)s_pak_size;
    f = fs_open(path, O_RDONLY);
    if (f == FILEHND_INVALID) return -1;
    n = (int)fs_total(f);
    fs_close(f);
    return n;
}
