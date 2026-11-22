/*
 * Host tests for the native sound runtime.
 *   test_sound <bank dir> <audio dir> [sdat]
 * 1. banks: every .srb loads; SSAR/record/bank/wave tables consistent; waves
 *    fit the AICA 65534-sample limit; budget numbers.
 * 2. id mapping / coverage: every SSAR id of every bank is run through the
 *    driver; an id is covered when its sequence starts >= 1 channel.
 * 3. driver vs offline render: each z1s id rendered by the driver (DS mixer
 *    model, same player volume model as sseqrender.c) and compared with the
 *    sdat2dc.py render (build/dreamcast/audio/sfx/): SNR and level.
 * 4. NNS layer: player sequence caps (SDAT seqMax), priority, oldest-first
 *    stealing, stop with fade, handle release, gSESoundHandle semantics.
 * 5. hooks adapter: g_dcp_hooks.sfx / sfx_stop, g_obj_hooks.sfx.
 * 6. output: AICA commands (start/update/stop), volume/pan mapping.
 */
#include "snd_internal.h"
#include "snd_nns.h"
#include "sound.h"
#include "snd_out_host.h"
#include "../../player/dcp_player.h"
#include "../../objects/objects.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

DcpHooks g_dcp_hooks;   /* normally in player/dcp_player.c */
ObjHooks g_obj_hooks;   /* normally in objects/obj_manager.c */

static int fails;
#define CHECK(c, ...) do { if (!(c)) { fails++; printf("FAIL %s:%d: ", __FILE__, __LINE__); printf(__VA_ARGS__); printf("\n"); } } while (0)

static const char *BANKS[] = {"z1b", "z1eb", "z1es", "z1s", "z2b", "z2eb", "z2es", "z2s", "z3b", "z3eb", "z3es",
                              "z3s", "z4b", "z4eb", "z4es", "z4s", "z5b", "z5eb", "z5es", "z5s", "z6b", "z6eb",
                              "z6es", "z6s", "z7b", "z7eb", "z7es", "z7s", "ze", "zfeb", "zfes"};
#define NBANKS ((int)(sizeof BANKS / sizeof BANKS[0]))

/* ------------------------------------------------------------ AICA ADPCM decode */
static void aica_decode(const uint8_t *src, long nsamp, int16_t *dst) {
    static const int stepTab[8] = {230, 230, 230, 230, 307, 409, 512, 614};
    int hist = 0, step = 127;
    for (long i = 0; i < nsamp; i++) {
        int byte = src[i / 2];
        int nib = (i & 1) ? (byte >> 4) & 15 : byte & 15;
        int d = nib & 7, diff = ((1 + (d << 1)) * step) >> 3;
        if (diff > 32767) diff = 32767;
        hist = (nib & 8) ? hist - diff : hist + diff;
        if (hist > 32767) hist = 32767; else if (hist < -32768) hist = -32768;
        step = (stepTab[d] * step) >> 8; if (step < 127) step = 127; else if (step > 24576) step = 24576;
        dst[i] = (int16_t)hist;
    }
}

/* ------------------------------------------------------------ DS-model mixer on driver registers */
static int16_t **s_pcm; static int s_npcm;
static void decode_waves(const SndBank *b) {
    s_npcm = (int)b->hdr->n_waves;
    s_pcm = calloc(s_npcm, sizeof *s_pcm);
    for (int i = 0; i < s_npcm; i++) {
        const SrbWave *w = &b->waves[i];
        const uint8_t *d = b->host_blob + w->data_off;
        s_pcm[i] = calloc(w->samples + 1, 2);
        if (w->format == SRB_FMT_ADPCM) aica_decode(d, w->samples, s_pcm[i]);
        else if (w->format == SRB_FMT_PCM16) memcpy(s_pcm[i], d, w->samples * 2);
        else for (uint32_t k = 0; k < w->samples; k++) s_pcm[i][k] = (int16_t)((int8_t)d[k] * 256);
    }
}
static void free_waves(void) { for (int i = 0; i < s_npcm; i++) free(s_pcm[i]); free(s_pcm); s_pcm = NULL; }

typedef struct { int active; uint32_t starts; int wave; uint64_t pos; } MixCh;
static MixCh mixch[16];
static void mix_frame(const SndBank *b, long frame, float *out) {
    long s0 = frame * 341 / 2, n = (frame + 1) * 341 / 2 - s0;
    for (int c = 0; c < 16; c++) {
        const SndChanRegs *r = snd_drv_regs(c);
        MixCh *m = &mixch[c];
        if (r->starts != m->starts) { m->starts = r->starts; m->active = 1; m->pos = 0; m->wave = r->wave; }
        if (!m->active) continue;
        const SrbWave *w = &b->waves[m->wave];
        uint64_t step = ((uint64_t)512 << 32) / (uint64_t)(r->timer ? r->timer : 16);
        if (!r->active) {
            /* natural one-shot end inside this frame, otherwise stopped at frame start */
            if (!(!w->loop && ((m->pos + step * (n - 1)) >> 32) >= w->samples)) { m->active = 0; continue; }
        }
        float g = (float)r->vol / 128.0f / (float)(1 << r->shift) * (float)snd_drv_mix_gain() / 65536.0f;
        float gl = g * (128 - r->pan) / 128.0f, gr = g * r->pan / 128.0f;
        for (long s = 0; s < n; s++) {
            uint64_t ip = m->pos >> 32;
            if (ip >= w->samples) {
                if (w->loop && w->samples > w->loop_start) {
                    uint64_t ll = w->samples - w->loop_start;
                    ip = w->loop_start + (ip - w->loop_start) % ll;
                    m->pos = (ip << 32) | (m->pos & 0xFFFFFFFFu);
                } else { m->active = 0; break; }
            }
            float v = s_pcm[m->wave][ip];
            out[2 * (s0 + s)] += v * gl; out[2 * (s0 + s) + 1] += v * gr;
            m->pos += step;
        }
    }
}

/* render SSAR id with the driver, sseqrender.c player model (NNSi_SndPlayerMain:
   fader = db[record volume] + db[127] * 3 = db[vol], prio = player prio), mix gain of the bank */
static long render_id(const SndBank *b, int ai, int si, float *out, long maxFrames, int *notes) {
    uint32_t size;
    const SrbSeqInfo *info = snd_bank_seq_info(b, ai, si);
    const uint8_t *data = snd_bank_seq_data(b, ai, &size);
    long f;
    snd_drv_init(b);
    memset(mixch, 0, sizeof mixch);
    if (!info) return 0;
    snd_drv_prepare_seq(0, data, size, info->offset, info->bank);
    snd_drv_set_player_prio(0, info->player_prio);
    snd_drv_set_player_volume(0, b->tab->db[info->volume & 127]);
    snd_drv_start_prepared(0);
    for (f = 0; f < maxFrames; f++) {
        snd_drv_frame();
        if (out) mix_frame(b, f, out);
        if (!snd_drv_player_active(0)) {
            int any = 0;
            for (int c = 0; c < 16; c++) any |= snd_drv_regs(c)->active;
            if (!any && f > 2) { f++; break; }
        }
    }
    *notes = snd_drv_stats()->notes;
    return f * 341 / 2;
}

/* rendered WAV (KOS wav2adpcm fmt 0x14, mono or non-interleaved stereo) -> int16 stereo */
static int16_t *load_render(const char *path, long *frames) {
    FILE *f = fopen(path, "rb");
    if (!f) return NULL;
    fseek(f, 0, SEEK_END); long n = ftell(f); fseek(f, 0, SEEK_SET);
    uint8_t *d = malloc(n);
    if (fread(d, 1, n, f) != (size_t)n) { fclose(f); free(d); return NULL; }
    fclose(f);
    int chans = d[22];
    long p = 12; uint32_t dsz = 0; const uint8_t *data = NULL;
    while (p + 8 <= n) {
        uint32_t cs = d[p + 4] | d[p + 5] << 8 | d[p + 6] << 16 | (uint32_t)d[p + 7] << 24;
        if (!memcmp(d + p, "data", 4)) { data = d + p + 8; dsz = cs; break; }
        p += 8 + cs + (cs & 1);
    }
    if (!data) { free(d); return NULL; }
    long ns = (long)dsz * 2 / chans;
    int16_t *l = malloc(ns * 2), *r = malloc(ns * 2), *o = malloc(ns * 4);
    aica_decode(data, ns, l);
    if (chans == 2) aica_decode(data + dsz / 2, ns, r); else memcpy(r, l, ns * 2);
    for (long k = 0; k < ns; k++) { o[2 * k] = l[k]; o[2 * k + 1] = r[k]; }
    free(l); free(r); free(d);
    *frames = ns;
    return o;
}

static char *g_audio;

/* SND_DUMP_DIR: write the driver render (stereo PCM16, 32728 Hz) for
 * tools/dreamcast/audio/sfxbank_check.py (compares with sseqrender PCM output) */
static void dump_wav(const char *dir, const char *sdat, int si, const float *buf, long n) {
    char p[600];
    snprintf(p, sizeof p, "%s/%s_%03d.wav", dir, sdat, si);
    FILE *f = fopen(p, "wb");
    if (!f) return;
    uint32_t bytes = (uint32_t)n * 4, rate = 32728, br = rate * 4, sz = 36 + bytes, fl = 16;
    uint16_t fmt = 1, ch = 2, ba = 4, bits = 16;
    fwrite("RIFF", 1, 4, f); fwrite(&sz, 4, 1, f); fwrite("WAVEfmt ", 1, 8, f); fwrite(&fl, 4, 1, f);
    fwrite(&fmt, 2, 1, f); fwrite(&ch, 2, 1, f); fwrite(&rate, 4, 1, f); fwrite(&br, 4, 1, f);
    fwrite(&ba, 2, 1, f); fwrite(&bits, 2, 1, f); fwrite("data", 1, 4, f); fwrite(&bytes, 4, 1, f);
    for (long k = 0; k < 2 * n; k++) {
        double v = buf[k] * (127.0 / 128.0);
        int16_t o = (int16_t)(v > 32767 ? 32767 : v < -32768 ? -32768 : lrint(v));
        fwrite(&o, 2, 1, f);
    }
    fclose(f);
}

/* ------------------------------------------------------------ 1+2: banks, coverage */
static void test_banks(const char *dir) {
    long total_ids = 0, covered = 0, rendered_ok = 0, both = 0, worst = 0;
    for (int k = 0; k < NBANKS; k++) {
        char path[512];
        SndBank b;
        snprintf(path, sizeof path, "%s/%s.srb", dir, BANKS[k]);
        int r = snd_bank_load(&b, path);
        CHECK(r == 0, "load %s: %d", path, r);
        if (r) continue;
        if (b.blob_size > (uint32_t)worst) worst = b.blob_size;
        CHECK(b.hdr->n_ssar == 1 && b.ssar[0].n_seq == 237, "%s: ssar %u seq %u", BANKS[k], b.hdr->n_ssar, b.ssar[0].n_seq);
        for (uint32_t w = 0; w < b.hdr->n_waves; w++) {
            const SrbWave *wv = &b.waves[w];
            uint32_t bytes = wv->format == SRB_FMT_ADPCM ? (wv->samples + 1) / 2 : wv->format == SRB_FMT_PCM16 ? wv->samples * 2 : wv->samples;
            CHECK(wv->samples <= 65534, "%s wave %u: %u samples", BANKS[k], w, wv->samples);
            CHECK(wv->data_off + bytes <= b.blob_size, "%s wave %u outside blob", BANKS[k], w);
            CHECK(wv->data_off % 32 == 0, "%s wave %u unaligned", BANKS[k], w);
            CHECK(!wv->loop || wv->loop_start < wv->samples, "%s wave %u loop", BANKS[k], w);
        }
        /* rendered statuses */
        char rp[512]; snprintf(rp, sizeof rp, "%s/%s.rendered.txt", dir, BANKS[k]);
        char status[237][32]; memset(status, 0, sizeof status);
        FILE *f = fopen(rp, "r");
        if (f) { int ai, si; long ns; int ch; char st[32], rel[400];
            while (fscanf(f, "%d %d %31s %ld %d %399s", &ai, &si, st, &ns, &ch, rel) == 6) if (ai == 0 && si < 237) memcpy(status[si], st, 32);
            fclose(f); }
        int cov = 0, ids = 0, mism = 0;
        for (int si = 0; si < 237; si++) {
            int notes;
            if (!snd_bank_seq_info(&b, 0, si)) continue;
            ids++;
            CHECK(snd_bank_sbnk(&b, snd_bank_seq_info(&b, 0, si)->bank, &(uint32_t){0}) != NULL, "%s id %d: bank missing", BANKS[k], si);
            render_id(&b, 0, si, NULL, 192 * 30, &notes);
            int starts = 0;
            for (int c = 0; c < 16; c++) starts += snd_drv_regs(c)->starts;
            int rok = strcmp(status[si], "silent") != 0 && status[si][0];
            if (starts > 0) cov++;
            if (rok) rendered_ok++;
            if (rok && starts > 0) both++;
            if ((starts > 0) != rok) { mism++; if (mism <= 3) printf("  %s id %d: driver starts %d, offline render %s\n", BANKS[k], si, starts, status[si]); }
        }
        total_ids += ids; covered += cov;
        printf("bank %-5s ids %3d covered %3d  sound RAM %6u  main RAM %5u  waves %2u  mismatch vs offline %d\n",
               BANKS[k], ids, cov, b.blob_size, b.hdr->off_wavedata, b.hdr->n_waves, mism);
        CHECK(mism == 0, "%s: %d ids differ from the offline render (sound / silence)", BANKS[k], mism);
        snd_bank_free(&b);
    }
    printf("coverage: %ld of %ld ids play (offline render: %ld non-silent, %ld both); worst bank %ld B\n",
           covered, total_ids, rendered_ok, both, worst);
}

/* ------------------------------------------------------------ 3: driver vs offline render */
static int cmpf(const void *a, const void *b) { double x = *(const double *)a, y = *(const double *)b; return x < y ? -1 : x > y; }
static void test_render(const char *dir, const char *sdat) {
    char path[512];
    SndBank b;
    snprintf(path, sizeof path, "%s/%s.srb", dir, sdat);
    if (snd_bank_load(&b, path)) { CHECK(0, "load %s", path); return; }
    decode_waves(&b);
    snprintf(path, sizeof path, "%s/%s.rendered.txt", dir, sdat);
    FILE *f = fopen(path, "r");
    if (!f) { CHECK(0, "no %s", path); return; }
    int ai, si, ch; long ns; char st[32], rel[400];
    double snr[256]; int nsnr = 0, worst_id = -1; double worst = 1e9;
    long maxS = 32728 * 3;
    float *buf = malloc(sizeof(float) * 2 * (maxS + 400));
    while (fscanf(f, "%d %d %31s %ld %d %399s", &ai, &si, st, &ns, &ch, rel) == 6) {
        if (!strcmp(rel, "-")) continue;
        char wp[700]; long rf; int notes;
        snprintf(wp, sizeof wp, "%s/%s", g_audio, rel);
        int16_t *ref = load_render(wp, &rf);
        if (!ref) { CHECK(0, "read %s", wp); continue; }
        memset(buf, 0, sizeof(float) * 2 * (maxS + 400));
        long mine = render_id(&b, ai, si, buf, maxS / 170 + 1, &notes);
        long n = rf < mine ? rf : mine; if (n > maxS) n = maxS;
        double se = 0, sr = 0, sm = 0;
        for (long k = 0; k < 2 * n; k++) {
            double a = ref[k], m = buf[k] * (127.0 / 128.0);
            se += (a - m) * (a - m); sr += a * a; sm += m * m;
        }
        double s = sr > 0 ? 10 * log10(sr / (se > 1e-9 ? se : 1e-9)) : 0;
        if (getenv("SND_DUMP_DIR")) dump_wav(getenv("SND_DUMP_DIR"), sdat, si, buf, mine);
        if (getenv("SND_LAGS") && atoi(getenv("SND_LAGS")) == si) {
            for (int lag = -4; lag <= 4; lag++) {
                double e2 = 0;
                for (long k = 8; k < n - 8; k++) for (int c = 0; c < 2; c++) {
                    double a = ref[2 * k + c], m = buf[2 * (k + lag) + c] * (127.0 / 128.0); e2 += (a - m) * (a - m); }
                printf("    lag %+d: SNR %.1f dB\n", lag, 10 * log10(sr / e2));
            }
            FILE *df = fopen("/tmp/sound/dump.raw", "wb");
            for (long k = 0; k < 2 * n; k++) { int16_t v[2] = {ref[k], (int16_t)lrint(buf[k] * 127.0 / 128.0)}; fwrite(v, 2, 2, df); }
            fclose(df);
        }
        snr[nsnr++] = s;
        if (s < worst) { worst = s; worst_id = si; }
        if (s < 6) printf("  %s id %d (%s): SNR %.1f dB, level %+.2f dB, %ld/%ld samples\n", sdat, si, rel, s,
                           sm > 0 && sr > 0 ? 10 * log10(sm / sr) : 0, mine, rf);
        free(ref);
    }
    fclose(f);
    qsort(snr, nsnr, sizeof(double), cmpf);
    printf("render %s vs offline (first <=3 s): %d ids, SNR min %.1f (id %d) / median %.1f / max %.1f dB\n",
           sdat, nsnr, nsnr ? snr[0] : 0, worst_id, nsnr ? snr[nsnr / 2] : 0, nsnr ? snr[nsnr - 1] : 0);
    CHECK(nsnr > 0 && snr[nsnr / 2] > 10, "median SNR too low");
    free(buf); free_waves(); snd_bank_free(&b);
}

/* ------------------------------------------------------------ 4: NNS layer */
static void frames(int n) { for (int i = 0; i < n; i++) sound_update(); }
static void test_nns(void) {
    SoundHandle h[6];
    int r = sound_load_bank("z1s");
    CHECK(r == 0, "sound_load_bank z1s: %d", r);
    if (r) return;
    /* id 0 SE_Spin: player 3 (seqMax 4), player prio 64 */
    for (int i = 0; i < 5; i++) { NNS_SndHandleInit(&h[i]); Sound_PlaySeqArcEx(&h[i], 0, 0); }
    CHECK(h[0].player == NULL, "oldest sound on player 3 not stolen");
    for (int i = 1; i < 5; i++) CHECK(h[i].player != NULL, "handle %d lost", i);
    CHECK(NNS_SndPlayerCountPlaying(3) == 4, "player 3 count %d", NNS_SndPlayerCountPlaying(3));
    /* lower priority request on a full player fails (Sound_PlaySeqArcBankPrio prio 10) */
    NNS_SndHandleInit(&h[5]);
    Sound_PlaySeqArcBankPrio(&h[5], 0, 0, 10, -1);
    CHECK(h[5].player == NULL, "low-priority start on full player succeeded");
    CHECK(h[1].player != NULL, "low-priority start stole a sound");
    frames(2);
    CHECK(snd_drv_stats()->notes > 0, "no notes after start");
    /* stop with fade: status 2, priority 0, freed after the fade */
    NNS_SndPlayerStopSeq(&h[1], 8);
    CHECK(h[1].player && h[1].player->status == 2 && h[1].player->prio == 0, "fade stop state");
    frames(9);
    CHECK(h[1].player == NULL, "faded sound not freed after 8 frames");
    /* immediate stop */
    NNS_SndPlayerStopSeq(&h[2], 0);
    CHECK(h[2].player == NULL, "stop 0 did not free");
    /* gSESoundHandle: second fire-and-forget releases (not stops) the first */
    sound_stop_all(); frames(1);
    Sound_PlaySeqArcEx(NULL, 0, 4);             /* SE_Ring, player 7 */
    NNSSndSeqPlayer *first = gSESoundHandle.player;
    Sound_PlaySeqArcEx(NULL, 0, 4);
    CHECK(first && first->status == 1 && first->handle == NULL, "first SE not detached/kept");
    CHECK(gSESoundHandle.player && gSESoundHandle.player != first, "gSESoundHandle not rebound");
    /* sequence end frees the seq player (SE_Ring 0.68 s) */
    frames(90);
    CHECK(NNS_SndPlayerCountPlaying(7) == 0, "SE_Ring players still allocated: %d", NNS_SndPlayerCountPlaying(7));
    /* positional volume / pan through the original formulas */
    CHECK(Sound_CalcPan(0x10000, 0) == 127 && Sound_CalcPan(0, 0x10000) == -128 && Sound_CalcPan(0x500, 0x200) == 3, "CalcPan");
    CHECK(Sound_CalcDistanceVolume(0, 0, 0, 0) == 127 && Sound_CalcDistanceVolume(0, 0, 0x20100, 0) == 0, "CalcDistanceVolume");
    /* sound_play API */
    SoundHandle *ph = sound_play(0x3e, 100, -40);  /* SE_Grind (held) */
    CHECK(ph && sound_is_playing(ph), "sound_play");
    frames(60);
    CHECK(sound_is_playing(ph), "held SE ended by itself");
    sound_stop(ph, 16); frames(20);
    CHECK(!sound_is_playing(ph), "sound_stop fade");
    sound_release(ph);
    /* invalid ids */
    CHECK(sound_play(237, 127, 0) == NULL && sound_play(-1, 127, 0) == NULL, "invalid id played");
    /* stage table */
    CHECK(!strcmp(sound_stage_sdat(0, 0, 0), "z1s") && !strcmp(sound_stage_sdat(0, 2, 1), "z1eb") &&
          !strcmp(sound_stage_sdat(7, 2, 0), "zfes") && sound_stage_sdat(8, 2, 0) == NULL, "sound_stage_sdat");
    printf("nns: player caps / priority / steal / fade / handles ok (%d failures so far)\n", fails);
}

/* ------------------------------------------------------------ 5+6: hooks + output */
static void test_hooks_output(void) {
    DcPlayer pl;
    memset(&pl, 0, sizeof pl);
    sound_stop_all(); frames(2);
    sound_hooks_install();
    g_host_out.n = 0; g_host_out.starts = g_host_out.stops = g_host_out.updates = 0;
    g_dcp_hooks.sfx(NULL, &pl, DCP_CH_TRICK, 0x31);   /* SE_Gallery_max on the TRICK handle */
    g_dcp_hooks.sfx(NULL, &pl, DCP_CH_GLOBAL, 0x3c);  /* SE_TMAX_bonus, global */
    g_obj_hooks.sfx(NULL, 0x7a);                      /* SE_Zako_damage */
    frames(3);
    CHECK(g_host_out.starts >= 3, "AICA starts %u", g_host_out.starts);
    int center_ok = 1, range_ok = 1;
    for (int i = 0; i < g_host_out.n; i++) {
        HostOutEvent *e = &g_host_out.ev[i];
        if (e->type != 1) continue;
        if (e->vol < 0 || e->vol > 65535 || e->pan < 0 || e->pan > 255 || e->freq < 172 || e->freq > 0x3FFFFF) range_ok = 0;
    }
    CHECK(range_ok, "AICA parameter out of range");
    (void)center_ok;
    g_dcp_hooks.sfx_stop(NULL, &pl, DCP_CH_TRICK);    /* fade 0x20 */
    frames(40);
    int on = 0; for (int c = 0; c < 16; c++) on += g_host_out.on[c];
    printf("hooks/output: %u starts, %u updates, %u stops, %d channels on after stop\n",
           g_host_out.starts, g_host_out.updates, g_host_out.stops, on);
    /* volume / pan mapping on a held sound: centre pan -> KOS 0x80 */
    sound_stop_all(); frames(2); g_host_out.n = 0;
    SoundHandle *h = sound_play(0x3e, 127, 0);
    frames(30);
    int last_vol = -1, last_pan = -1;
    for (int i = 0; i < g_host_out.n; i++) if (g_host_out.ev[i].type != 3) { last_vol = g_host_out.ev[i].vol; last_pan = g_host_out.ev[i].pan; }
    printf("SE_Grind centred: AICA vol %d pan 0x%02x\n", last_vol, last_pan);
    sound_set_pan(h, 127); frames(4);
    for (int i = 0; i < g_host_out.n; i++) if (g_host_out.ev[i].type == 2 && (g_host_out.ev[i].what & SND_OUT_PAN)) last_pan = g_host_out.ev[i].pan;
    printf("SE_Grind pan +127: AICA pan 0x%02x\n", last_pan);
    CHECK(last_pan > 0x80, "right pan not mapped right");
    sound_stop(h, 0); sound_release(h); frames(2);
}

/* ------------------------------------------------------------ 6: sys bank (menus) */
static void test_sys(void) {
    int r = sound_load_bank("sys"), silent = 0, total = 0;
    CHECK(r == 0, "sound_load_bank sys: %d", r);
    if (r) return;
    /* every SysSound_StartSE id (both characters) and SysSound_StartVoice id must key on a channel */
    for (int pass = 0; pass < 3; pass++) {
        int n_max = pass < 2 ? 23 : 8;
        for (int n = 0; n < n_max; n++) {
            sound_stop_all(); frames(2);
            g_host_out.starts = 0;
            if (pass < 2) SysSound_StartSE(n, pass); else SysSound_StartVoice(n, 0);
            frames(6);
            total++;
            if (!g_host_out.starts) { silent++; printf("  sys %s %d (chara %d): no channel started\n", pass < 2 ? "SE" : "voice", n, pass); }
        }
    }
    /* title screen: SSAR 0 ARC_TITLE seqs 0-5 (TitleScreen_PlayStartSound / TitleDemo_PlaySeq* / PlayTitleVoice) */
    for (int n = 0; n < 6; n++) {
        sound_stop_all(); frames(2);
        g_host_out.starts = 0;
        if (n == 5) Sound_PlayVoice(NULL, 0, 5); else Sound_PlaySeqArc(NULL, 0, (uint32_t)n);
        frames(6);
        total++;
        if (!g_host_out.starts) { silent++; printf("  sys title 0:%d: no channel started\n", n); }
    }
    /* the Alt voice keeps the first voice playing */
    sound_stop_all(); frames(2);
    SysSound_StartVoice(0, 0); frames(2);
    SysSound_StartVoice(1, 1); frames(2);
    int on = 0;
    for (int c = 0; c < 16; c++) on += snd_drv_regs(c)->active;
    printf("sys bank: %d of %d SysSound_StartSE/StartVoice calls start a channel; voice + alt voice: %d channels on\n",
           total - silent, total, on);
    CHECK(silent == 0, "%d sys sounds silent", silent);
    CHECK(on >= 2, "alt voice stopped the first voice");
}

int main(int argc, char **argv) {
    const char *dir = argc > 1 ? argv[1] : "build/dreamcast/audio/sfxbank";
    g_audio = argc > 2 ? argv[2] : "build/dreamcast/audio";
    const char *sdat = argc > 3 ? argv[3] : "z1s";
    test_banks(dir);
    test_render(dir, sdat);
    sound_set_dir(dir);
    sound_init();
    test_nns();
    test_hooks_output();
    test_sys();
    sound_shutdown();
    test_render(dir, "sys");
    printf("%s (%d failures)\n", fails ? "FAILED" : "OK", fails);
    return fails != 0;
}
