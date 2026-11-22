/*
 * Native sound runtime (Dreamcast port of Sonic Rush) - internal interfaces.
 *
 * Layers, top to bottom (same split as on the DS):
 *   sound.c      game layer: Sound_* from src/game/Sound/ (reused, DS calls
 *                replaced by the native NNS layer below)
 *   snd_nns.c    NitroSystem player layer: NNS_SndArcPlayerStartSeqArcEx,
 *                NNSi_SndPlayerAllocSeqPlayer, AllocSeqPlayer, ForceStopSeq,
 *                NNSi_SndPlayerMain, faders ... (reused from
 *                src/libs/NitroSystem/snd/, NNS_Fnd lists replaced by a
 *                native intrusive list)
 *   snd_drv.c    sound driver (the DS ARM7 SND driver: sequence players,
 *                tracks, 16 ExChannels with envelope / LFO / sweep), a
 *                real-time port of tools/dreamcast/audio/sseqrender.c
 *   snd_out_*.c  output: AICA channels through the KOS AICA command queue
 *                (snd_out_kos.c) or the host test backend (test/)
 *   snd_bank.c   .srb bank loader (tools/dreamcast/audio/sfxbank.py)
 */
#ifndef SND_INTERNAL_H
#define SND_INTERNAL_H

#include <stdint.h>
#include <stddef.h>

/* ------------------------------------------------------------ bank (.srb) */
#define SRB_MAGIC 0x42535253u /* 'SRSB' */
#define SRB_VERSION 2

typedef struct {
    uint32_t magic, version;
    char sdat[8];
    uint32_t n_players, off_players;
    uint32_t n_ssar, off_ssar;
    uint32_t n_banks, off_banks;
    uint32_t n_waves, off_waves;
    uint32_t psg_wave, noise_wave;
    uint32_t off_tables;
    uint32_t off_wavedata, wavedata_size;
    uint32_t mix_gain_tl;   /* port mix gain, 2^(-n/16): the gain baked into the music renders (sdat2dc.py MIX_GAIN_TL) */
} SrbHeader;

typedef struct { uint8_t max_seq, pad; uint16_t alloc_mask; uint32_t heap; } SrbPlayer;
typedef struct { uint32_t n_seq, off_rec, off_data, data_size; } SrbSsar;
/* SSAR record (NNSSndArcSeqArcSeqInfo): 12 bytes */
typedef struct { uint32_t offset; uint16_t bank; uint8_t volume, channel_prio, player_prio, player; uint16_t pad; } SrbSeqInfo;
typedef struct { uint32_t off, size; int16_t wave_base[4]; uint16_t wave_count[4]; } SrbBank;
typedef struct { uint32_t data_off, samples, loop_start; uint16_t timer; uint8_t format, loop; } SrbWave;
enum { SRB_FMT_ADPCM = 0, SRB_FMT_PCM16 = 1, SRB_FMT_PCM8 = 2 };

typedef struct {
    int16_t dbsq[128];    /* ARM7 SNDi_DecibelSquareTable */
    int16_t db[128];      /* ARM9 SNDi_DecibelTable (data_020725e0, NNS player volume) */
    uint8_t volume[724];  /* SND_CalcChannelVolume table */
    uint8_t attack[19];
    uint8_t sine[33];
    uint8_t order[16];    /* channel allocation order */
    uint16_t pitch[768];  /* SND_CalcTimer table */
} SrbTables;

typedef struct SndBank {
    uint8_t *meta;              /* file bytes [0, off_wavedata) */
    const SrbHeader *hdr;
    const SrbPlayer *players;
    const SrbSsar *ssar;
    const SrbBank *banks;
    const SrbWave *waves;
    const SrbTables *tab;
    uint32_t aram;              /* sound RAM address of the wave blob (host: offset 0) */
    uint8_t *host_blob;         /* host backend only: wave blob copy */
    uint32_t blob_size;
} SndBank;

int snd_bank_load(SndBank *b, const char *path);   /* 0 = ok */
void snd_bank_free(SndBank *b);
const SrbSeqInfo *snd_bank_seq_info(const SndBank *b, int ssar, int index); /* NNSi_SndSeqArcGetSeqInfo */
const uint8_t *snd_bank_seq_data(const SndBank *b, int ssar, uint32_t *size);
const uint8_t *snd_bank_sbnk(const SndBank *b, int bank, uint32_t *size);

/* ------------------------------------------------------------ driver */
#define SND_CH_NUM 16
#define SND_PLAYER_NUM 16
#define SND_TRACK_NUM 32
#define SND_FRAME_HZ_X10000 1919555 /* 33513982 / (2728*64) = 191.9555 Hz */

void snd_drv_init(const SndBank *bank);
/* SND_PrepareSeq: set up sequence data at `offset` with SBNK `bank_no` on
 * driver player `player` (0..15); SND_StartPreparedSeq starts it ticking. */
void snd_drv_prepare_seq(int player, const uint8_t *seq, uint32_t seq_len, uint32_t offset, int bank_no);
void snd_drv_start_prepared(int player);
void snd_drv_stop_seq(int player);                      /* SND_StopSeq */
void snd_drv_pause_seq(int player, int flag);           /* SND_PauseSeq */
void snd_drv_player_notes(int player, int *tries, int *ok); /* notes of the current sequence (trace) */
/* trace: called when a sequence player ends (arc / id of the request, notes tried / sounding) */
extern void (*g_snd_seq_end_cb)(int32_t arc, int32_t id, int tries, int ok);
int  snd_drv_player_active(int player);                 /* SND_GetPlayerStatus bit */
void snd_drv_set_player_volume(int player, int db);     /* SND_SetPlayerVolume (extFader, 0.1 dB) */
void snd_drv_set_player_prio(int player, int prio);     /* SND_SetPlayerChannelPriority */
void snd_drv_set_track_pan(int player, uint32_t mask, int pan); /* SND_SetTrackPan */
void snd_drv_set_alloc_mask(int player, uint16_t mask); /* SND_SetTrackAllocatableChannel */
void snd_drv_frame(void);                               /* one ARM7 sound frame */
void snd_drv_flush(void);                               /* push channel registers to the output */
void snd_drv_stop_all(void);
uint32_t snd_drv_mix_gain(void);
void snd_drv_set_fade(uint32_t q16);                    /* extra output fade (loading), q16, 65536 = none */                        /* port mix gain of the loaded bank, q16 */

/* per-channel register state after the last frame (host tests / debug) */
typedef struct {
    int active;            /* 1 playing */
    int wave;              /* bank wave index */
    int vol, shift, pan;   /* DS registers: vol 0..127, shift 0/1/2/4, pan 0..127 */
    int timer;             /* DS timer */
    uint32_t starts;       /* key-on count */
} SndChanRegs;
const SndChanRegs *snd_drv_regs(int ch);
typedef struct { int steals, alloc_fail, inst_missing, wave_missing, max_voices, notes; } SndDrvStats;
const SndDrvStats *snd_drv_stats(void);
extern int g_sound_trace;                               /* sound.c: serial trace on */
void snd_drv_trace(uint32_t frame);                     /* log summed output level (trace) */

/* ------------------------------------------------------------ output */
void snd_out_init(void);
void snd_out_shutdown(void);
int  snd_out_upload(SndBank *b, const void *blob, uint32_t size); /* sets b->aram */
void snd_out_release(SndBank *b);
/* fmt SRB_FMT_*, freq in Hz, vol = linear amplitude q16 (65535 = 0 dB), pan 0..255 (KOS) */
void snd_out_start(int ch, uint32_t aram, int fmt, uint32_t samples, int loop, uint32_t loop_start,
                   uint32_t freq, int vol, int pan);
#define SND_OUT_FREQ 1
#define SND_OUT_VOL 2
#define SND_OUT_PAN 4
void snd_out_update(int ch, unsigned what, uint32_t freq, int vol, int pan);
void snd_out_stop(int ch);
/* KOS file read helpers (host backend uses stdio) */
void *snd_out_read_file(const char *path, uint32_t off, uint32_t size);
int snd_out_file_size(const char *path);

#endif
