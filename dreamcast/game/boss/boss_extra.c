/*
 * Extra Zone boss (stage 26, zone 8 act 2; overlay ov001): the mode itself.
 * ExtraBoss_Start / _Shutdown, the main states (ExtraBoss_State_Battle, ExtraBoss_State_CharacterSwitch
 * and their post-update / draw tasks), rounds (ExtraBoss_ResetRound, _StartBattle, _AdvancePhase,
 * _StartCharacterSwitch), pad remapping, the camera (ExtraBoss_ResetCamera, _UpdateCamera,
 * _CameraFollowPlayer, _CameraTrackObject, the camera slots and ExtraBoss_ApplyCamera /
 * _ApplyScreenFitCamera), display / light set-up (ExtraBoss_InitDisplay), asset loading
 * (ExtraBoss_LoadAssets and the archive helpers), and the self-contained API of boss_extra.h.
 * Translated from src/game/Zone/Extra_Zone/Boss/ (overlay ov001).  See BOSSES.md.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "boss_extra_int.h"
#ifdef _arch_dreamcast
#include "../engine/pak.h"   /* /cd/data.pak: per-stage files without a directory walk each */
#endif

BossExtraHooks g_boss_extra_hooks;
BossExStats g_bossex_stats;
ExGame g_ex;
ExWork *gExtraBossWork;
fx32 data_ov001_0231f520, data_ov001_0231f524;
s32 gExtraBossResultTime, gExtraBossResultRings;
ExCamSlot *data_ov001_0231f080;
char g_ex_model_dir[96] = "/cd/models", g_ex_data_dir[96] = "/cd/boss/extra";
static BossExtraResult s_result;
static int s_active;

ExWork *ExtraBoss_GetWork(void) { return gExtraBossWork; }
s32 ex_level(void) {
    if (g_ex.unk10 == 2 || g_ex.unk10 == 3)
        return 1;
    return g_ex.unk14;
}

/* ---------------------------------------------------------- fx helpers */
s32 FX_Sqrt(s32 v) {
    /* FX_Sqrt: hardware sqrt of (u64)v << 32, (result + 0x200) >> 10 */
    u64 x, r, b;
    if (v <= 0)
        return 0;
    x = (u64)(u32)v << 32;
    r = 0;
    b = (u64)1 << 62;
    while (b > x)
        b >>= 2;
    while (b != 0) {
        if (x >= r + b) {
            x -= r + b;
            r = (r >> 1) + b;
        } else {
            r >>= 1;
        }
        b >>= 2;
    }
    return (s32)(((u32)r + 0x200) >> 10);
}
void ex_mtx_mult_vec33(const fx32 *v, const MtxFx33 *m, fx32 *out) {
    fx32 x = v[0], y = v[1], z = v[2];
    out[0] = FX_Mul(x, m->m[0]) + FX_Mul(y, m->m[3]) + FX_Mul(z, m->m[6]);
    out[1] = FX_Mul(x, m->m[1]) + FX_Mul(y, m->m[4]) + FX_Mul(z, m->m[7]);
    out[2] = FX_Mul(x, m->m[2]) + FX_Mul(y, m->m[5]) + FX_Mul(z, m->m[8]);
}

/* ----------------------------------------------------------- bossex.bin */
typedef struct { u32 addr, size, off; } ExChunk;
typedef struct { u8 model, pad; u16 node; char name[16]; } ExJointRec;
typedef struct {
    u32 flags;
    char model[96];
    char anim[5][96];
    u16 count[5];
    u16 frames[5][8];
    u16 pad;
} ExFxRec;
static u8 *s_bin;
static u32 s_bin_size;
static const ExChunk *s_chunks;
static u32 s_nchunks;
static const char (*s_models)[96];
static u32 s_nmodels;
static const char (*s_anims)[96];
static u32 s_nanims;
static const ExJointRec *s_joints;
static u32 s_njoints;
static const ExFxRec *s_fx;
static u32 s_nfx;

static void *ex_slurp(const char *path, u32 *n) {
    FILE *f;
    void *b;
    long sz;
#ifdef _arch_dreamcast
    {   /* /cd/data.pak (engine/pak.h) */
        void *pb;
        int k = pak_load(path, &pb, n);
        if (k == PAK_FOUND)
            return pb;
        if (k == PAK_MISSING)
            return NULL;
    }
#endif
    f = fopen(path, "rb");
    if (!f)
        return NULL;
    fseek(f, 0, SEEK_END);
    sz = ftell(f);
    fseek(f, 0, SEEK_SET);
    b = malloc((size_t)sz + 4);
    if (b == NULL || fread(b, 1, (size_t)sz, f) != (size_t)sz) {
        fclose(f);
        free(b);
        return NULL;
    }
    fclose(f);
    *n = (u32)sz;
    return b;
}
void *ex_file_load(const char *rel, u32 *n) {
    char path[224];
    snprintf(path, sizeof(path), "%s/%s", g_ex_data_dir, rel);
    return ex_slurp(path, n);
}

static int ex_data_load(void) {
    u32 end, i;
    const u8 *p;
    if (s_bin != NULL)
        return 0;
    s_bin = (u8 *)ex_file_load("bossex.bin", &s_bin_size);
    if (s_bin == NULL || s_bin_size < 8 || memcmp(s_bin, "BXDT", 4) != 0) {
        printf("bossex: cannot load %s/bossex.bin (run dreamcast/game/boss/tools/bossexdata.py)\n", g_ex_data_dir);
        free(s_bin);
        s_bin = NULL;
        return -1;
    }
    s_nchunks = *(const u16 *)(s_bin + 6);
    s_chunks = (const ExChunk *)(s_bin + 8);
    end = 0;
    for (i = 0; i < s_nchunks; i++)
        if (s_chunks[i].off + s_chunks[i].size > end)
            end = s_chunks[i].off + s_chunks[i].size;
    end = (end + 3) & ~3u;
    p = s_bin + end;
    s_nmodels = *(const u32 *)p;
    s_models = (const char(*)[96])(p + 4);
    p += 4 + 96 * s_nmodels;
    s_nanims = *(const u32 *)p;
    s_anims = (const char(*)[96])(p + 4);
    p += 4 + 96 * s_nanims;
    s_njoints = *(const u32 *)p;
    s_joints = (const ExJointRec *)(p + 4);
    p += 4 + sizeof(ExJointRec) * s_njoints;
    s_nfx = *(const u32 *)p;
    s_fx = (const ExFxRec *)(p + 4);
    if ((u32)(p + 4 + sizeof(ExFxRec) * s_nfx - s_bin) > s_bin_size) {
        printf("bossex: bossex.bin is truncated\n");
        free(s_bin);
        s_bin = NULL;
        return -1;
    }
    return 0;
}
const void *ex_data(u32 addr) {
    u32 i;
    for (i = 0; i < s_nchunks; i++)
        if (addr >= s_chunks[i].addr && addr < s_chunks[i].addr + s_chunks[i].size)
            return s_bin + s_chunks[i].off + (addr - s_chunks[i].addr);
    printf("bossex: ROM table %08x not in bossex.bin\n", (unsigned)addr);
    {
        static const u32 zero[64];
        return zero;
    }
}
const char *ex_rom_str(u32 ptr) { return (const char *)ex_data(ptr); }
const char *ex_model_file(int i) { return (u32)i < s_nmodels ? s_models[i] : ""; }
int ex_joint(int model, const char *name) {
    u32 i;
    for (i = 0; i < s_njoints; i++)
        if (s_joints[i].model == model && !strncmp(s_joints[i].name, name, 16))
            return s_joints[i].node == 0xffff ? -1 : s_joints[i].node;
    return -1;
}

/* ------------------------------------------------------------ resources */
ExFxRes gExtraBossFxRes[0x1b];
BossModelRes g_ex_bsex[5];
BossAnimRes data_ov001_0231f068, data_ov001_0231f074, data_ov001_0231f534, data_ov001_0231f53c;
ExSprRes data_ov001_0231f320, gExtraBossRingSprite;
ExBgImage g_ex_bg;

/* ExtraBoss_LoadBossModel: /mod/bsex_md.nsbmd (5 models) + archive files 0x28 / 0x29 */
static int ExtraBoss_LoadBossModel(void) {
    int i, err = 0;
    for (i = 0; i < 5; i++)
        err |= boss_res_load_model(&g_ex_bsex[i], s_models[i]);
    err |= boss_res_load_anim(&data_ov001_0231f068, s_anims[0]);
    err |= boss_res_load_anim(&data_ov001_0231f074, s_anims[1]);
    return err;
}
static void ExtraBoss_ReleaseModelRes(void) {
    int i;
    for (i = 0; i < 5; i++)
        boss_res_free_model(&g_ex_bsex[i]);
    boss_res_free_anim(&data_ov001_0231f068);
    boss_res_free_anim(&data_ov001_0231f074);
}
/* ExtraBoss_LoadArchiveFiles24_25: ex_ply_ca.nsbca / ex_ply_va.nsbva */
static int ExtraBoss_LoadArchiveFiles24_25(void) {
    return boss_res_load_anim(&data_ov001_0231f534, s_anims[2]) | boss_res_load_anim(&data_ov001_0231f53c, s_anims[3]);
}
static void ExtraBoss_ReleaseArchiveFiles24_25(void) {
    boss_res_free_anim(&data_ov001_0231f534);
    boss_res_free_anim(&data_ov001_0231f53c);
}
/* ExtraBoss_LoadObjResources: gExtraBossFxResTable -> gExtraBossFxRes (model + 5 animation slots) */
static int ExtraBoss_LoadObjResources(void) {
    u32 k;
    int s, err = 0;
    for (k = 0; k < s_nfx && k < 0x1b; k++) {
        ExFxRes *r = &gExtraBossFxRes[k];
        memset(r, 0, sizeof(*r));
        r->flags = s_fx[k].flags;
        if (s_fx[k].model[0])
            err |= boss_res_load_model(&r->mdl, s_fx[k].model);
        memcpy(r->frames, s_fx[k].frames, sizeof(r->frames));
        for (s = 0; s < 5; s++) {
            if (s_fx[k].count[s] == 0)
                continue;
            if (s_fx[k].anim[s][0]) {
                err |= boss_res_load_anim(&r->anm[s], s_fx[k].anim[s]);
            } else {
                /* material / pattern / SRT animation: timing only (Model_SetAnimation in boss_model.c) */
                r->anm[s].nframes = r->frames[s];
                r->anm[s].count = s_fx[k].count[s] > 8 ? 8 : s_fx[k].count[s];
            }
        }
    }
    return err;
}
static void ExtraBoss_ReleaseObjResources(void) {
    int k, s;
    for (k = 0; k < 0x1b; k++) {
        boss_res_free_model(&gExtraBossFxRes[k].mdl);
        for (s = 0; s < 5; s++)
            boss_res_free_anim(&gExtraBossFxRes[k].anm[s]);
    }
}
/* ExtraBoss_LoadBgFiles: zex.bbg + its 21 palette files (archive files 0xb..0x20) */
static int ExtraBoss_LoadBgFiles(void) {
    u32 n;
    u8 *b = (u8 *)ex_file_load("zex.bin", &n);
    if (b == NULL || n < 12 || memcmp(b, "EXBG", 4) != 0) {
        free(b);
        printf("bossex: cannot load zex.bin\n");
        return -1;
    }
    memset(&g_ex_bg, 0, sizeof(g_ex_bg));
    g_ex_bg.bytes = b;
    g_ex_bg.w = *(const u16 *)(b + 4);
    g_ex_bg.h = *(const u16 *)(b + 6);
    g_ex_bg.npal = *(const u16 *)(b + 8);
    g_ex_bg.base = (const u16 *)(b + 12);
    g_ex_bg.pals = g_ex_bg.base + 256;
    g_ex_bg.px = (const u8 *)(g_ex_bg.pals + 192 * g_ex_bg.npal);
    if ((u32)(g_ex_bg.px + g_ex_bg.w * g_ex_bg.h - b) > n) {
        free(b);
        memset(&g_ex_bg, 0, sizeof(g_ex_bg));
        return -1;
    }
    return 0;
}
static void ExtraBoss_ReleaseBgFiles(void) {
    free(g_ex_bg.bytes);
    memset(&g_ex_bg, 0, sizeof(g_ex_bg));
}

/* ExtraBoss_LoadArchiveFile26 / 27: ac_ex_fix_moji.bac (HUD), ac_itm_ring.bac */
static int ExtraBoss_LoadArchiveFile26(void) { return ex_sprres_load(&data_ov001_0231f320, "ex_fix_moji"); }
static int ExtraBoss_LoadArchiveFile27(void) { return ex_sprres_load(&gExtraBossRingSprite, "itm_ring"); }
static void ExtraBoss_ReleaseArchiveFile26(void) { ex_sprres_free(&data_ov001_0231f320); }
static void ExtraBoss_ReleaseArchiveFile27(void) { ex_sprres_free(&gExtraBossRingSprite); }
/* ExtraBoss_LoadArchiveFiles21_23: ac_ex_vs.bac + extra_vs_bg_son / _blz.bbg (the sub screen's VS
 * splash).  Optional natively: without them the cut-in is not shown. */
static int ExtraBoss_LoadArchiveFiles21_23(void) {
    if (ex_sprres_load(&g_ex_vs_sprites, "ex_vs") != 0)
        memset(&g_ex_vs_sprites, 0, sizeof(g_ex_vs_sprites));
    ex_vsbg_load();
    return 0;
}
static void ExtraBoss_ReleaseArchiveFiles21_23(void) {
    ex_sprres_free(&g_ex_vs_sprites);
    ex_vsbg_free();
}
/* ExtraBoss_LoadAssets (modified: /narc/extra_act_lz7.narc is not loaded and decompressed - the
 * converter wrote its members as packages - and snd/ze/sound_data.sdat belongs to the integrator's
 * audio; so ExtraBoss_GetCompressionType / GetUncompressedSize have no use) */
int ExtraBoss_LoadAssets(void) {
    int err = 0;
    err |= ExtraBoss_LoadArchiveFiles24_25();
    err |= ExtraBoss_LoadBossModel();
    err |= ExtraBoss_LoadBgFiles();
    err |= ExtraBoss_LoadArchiveFile27();
    err |= ExtraBoss_LoadArchiveFile26();
    err |= ExtraBoss_LoadObjResources();
    err |= ExtraBoss_LoadArchiveFiles21_23();
    return err ? -1 : 0;
}
void ExtraBoss_ReleaseAssets(void) {
    ExtraBoss_ReleaseArchiveFiles24_25();
    ExtraBoss_ReleaseModelRes();
    ExtraBoss_ReleaseBgFiles();
    ExtraBoss_ReleaseArchiveFile27();
    ExtraBoss_ReleaseArchiveFile26();
    ExtraBoss_ReleaseObjResources();
    ExtraBoss_ReleaseArchiveFiles21_23();
}

/* ExtraBoss_LoadModelPalettes (modified: the converted textures carry their palettes; the palette
 * swaps of the core flash / hurt blink are kept as state only, see BOSSES.md) */
static u32 s_pal_state[3];
void ExtraBoss_LoadModelPalettes(u32 names, u32 idx, u32 slots, s32 n) {
    s_pal_state[0] = names;
    s_pal_state[1] = idx;
    s_pal_state[2] = (u32)n;
    (void)slots;
}

/* ----------------------------------------------------------- brightness */
typedef struct { s32 count, total; s16 from, to; u32 flags; s32 *done; ExTask *t; } ExBrightLerp;
static ExBrightLerp s_bl[4];
static void BrightnessLerp_Main(void) {
    ExBrightLerp *w = (ExBrightLerp *)gTaskCurrent->work;
    s16 b;
    if (w->done != NULL && *w->done != 0) {
        ex_task_destroy(gTaskCurrent);
        w->t = NULL;
        return;
    }
    w->count++;
    if (w->count >= w->total) {
        if (w->done != NULL)
            *w->done = 1;
        if (w->flags & 1)
            g_ex.bright_main = w->to;
        if (w->flags & 2)
            g_ex.bright_sub = w->to;
        ex_task_destroy(gTaskCurrent);
        w->t = NULL;
        return;
    }
    b = (s16)((w->count * w->to + w->from * (w->total - w->count)) / w->total);
    if (w->flags & 1)
        g_ex.bright_main = b;
    if (w->flags & 2)
        g_ex.bright_sub = b;
}
void BrightnessLerp_Create(s32 total, s16 from, s16 to, u32 flags, s32 *done) {
    int i;
    for (i = 0; i < 4; i++)
        if (s_bl[i].t == NULL)
            break;
    if (i == 4)
        i = 0;
    memset(&s_bl[i], 0, sizeof(s_bl[i]));
    s_bl[i].total = total;
    s_bl[i].from = from;
    s_bl[i].to = to;
    s_bl[i].flags = flags & 3;
    if (done != NULL) {
        *done = 0;
        s_bl[i].done = done;
    }
    s_bl[i].t = ex_task_create(BrightnessLerp_Main, 0x4000, &s_bl[i]);
}

/* ---------------------------------------------------------------- pad */
static void ExtraBoss_InitPadMap(void) {
    s32 i;
    for (i = 0; i < 12; i++)
        gExtraBossWork->unk22[i] = (u16)(1 << i);
}
static u16 ExtraBoss_RemapPadBits(u32 bits) {
    s32 i;
    u16 r = 0;
    for (i = 0; i < 12; i++) {
        if (bits & (1u << i))
            r |= gExtraBossWork->unk22[i];
    }
    return r;
}
/* Dreamcast controller -> DS pad bits (gPad): A 1, B 2, START 8, RIGHT 0x10, LEFT 0x20, UP 0x40, DOWN 0x80 */
static u16 ex_ds_pad(u32 in) {
    u16 r = 0;
    if (in & IN_JUMP)
        r |= 1;      /* A -> A */
    if (in & IN_TRICK)
        r |= 2;      /* B -> B */
    if (in & IN_BOOST)
        r |= 2;      /* X -> B */
    if (in & IN_ALT)
        r |= 1;      /* Y -> A */
    if (in & IN_START)
        r |= 8;
    if (in & IN_RIGHT)
        r |= 0x10;
    if (in & IN_LEFT)
        r |= 0x20;
    if (in & IN_UP)
        r |= 0x40;
    if (in & IN_DOWN)
        r |= 0x80;
    return r;
}
static u16 s_gpad[5]; /* gPad[0] held, [1] released, [2] pressed, [3] -, [4] repeat */

/* --------------------------------------------------------------- camera */
static void ExtraBoss_ApplyScreenFitCamera(ExCamSlot *p) {
    /* the eye's z follows the field of view: cot(fovy) * 96 (G3i_PerspectiveW_ / LookAt are built by
     * the renderer from the slot, ex_camera in boss_extra_draw.c) */
    p->unk14[2] = FX_Div(COS(p->unk0), SIN(p->unk0)) * 0x60 + p->unk20[2];
}
static void ExtraBoss_ApplyCamera(ExCamSlot *p) { (void)p; }
static void ExtraBoss_AllocCharacterSlots(void) {
    data_ov001_0231f080 = (ExCamSlot *)calloc(3, sizeof(ExCamSlot));
    memcpy(data_ov001_0231f080, ex_data(0x023169b4), 0xcc);
    ExtraBoss_ApplyScreenFitCamera(data_ov001_0231f080);
}
static void ExtraBoss_FreeCharacterSlots(void) {
    free(data_ov001_0231f080);
    data_ov001_0231f080 = NULL;
}
/* data_ov001_0231d52c: ApplyScreenFitCamera, ApplyCamera, ApplyCamera */
static void ExtraBoss_InitCharacterSlot(s32 n) {
    ExCamSlot *t0 = &data_ov001_0231f080[n];
    if (n == 0)
        ExtraBoss_ApplyScreenFitCamera(t0);
    else
        ExtraBoss_ApplyCamera(t0);
    t0->unk38[2] = 0;
    t0->unk38[1] = t0->unk38[2];
    t0->unk38[0] = t0->unk38[1];
}
static void ExtraBoss_CopyCharacterDefaults(s32 n) {
    memcpy(&data_ov001_0231f080[n], (const u8 *)ex_data(0x023169b4) + n * 0x44, 0x44);
}
ExCamSlot *ExtraBoss_GetCurrentCharacterSlot(void) { return &data_ov001_0231f080[gExtraBossWork->unk4]; }

void ExtraBoss_ResetCamera(void) {
    ExWork *t0 = ExtraBoss_GetWork();
    t0->unk31C = 0x80000;
    t0->unk320 = 0xc0000;
    t0->unk318 = 0;
    t0->unk324 = 0;
    t0->unk328 = 0x20000;
    t0->unk32C = -0x20000;
    t0->unk330 = 0;
    t0->unk334 = 0x2000;
}
void ExtraBoss_SetCameraParams(s32 a, s32 b, s32 c, s32 d, s32 e) {
    ExWork *w = ExtraBoss_GetWork();
    w->unk324 = a;
    w->unk328 = b;
    w->unk32C = c;
    w->unk330 = d;
    w->unk334 = e;
}
static ExObj *ExtraBoss_Unk_ov001_02300c54(void) { return gExtraBossPlayerManager->unk8[0]; }
static void ExtraBoss_CameraFollowPlayer(void) {
    ExObj *t = ExtraBoss_GetPlayer();
    ExWork *g = ExtraBoss_GetWork();
    s32 dl, dr, du, dd, sp;
    s32 tx = t->unk18, ty = t->unk1C;
    dl = g->unk31C - g->unk328 - tx;
    dr = tx - (g->unk31C + g->unk32C);
    du = g->unk320 - g->unk324 - ty;
    dd = ty - (g->unk320 + g->unk330);
    sp = g->unk334;
    if (dl > 0) {
        if (dl > sp)
            dl = sp;
        g->unk31C -= dl;
        if (g->unk31C - 0x80000 < g->unk54)
            g->unk31C = g->unk54 + 0x80000;
    } else if (dr > 0) {
        if (dr > sp)
            dr = sp;
        g->unk31C += dr;
        if (g->unk31C + 0x80000 > g->unk58)
            g->unk31C = g->unk58 - 0x80000;
    }
    if (du > 0) {
        if (du > sp)
            du = sp;
        g->unk320 -= du;
        if (g->unk320 - 0x60000 < g->unk50)
            g->unk320 = g->unk50 + 0x60000;
    } else if (dd > 0) {
        if (dd > sp)
            dd = sp;
        g->unk320 += dd;
        if (g->unk320 + 0x60000 > g->unk5C)
            g->unk320 = g->unk5C - 0x60000;
    }
    data_ov001_0231f524 = -(g->unk31C - 0x80000);
    data_ov001_0231f520 = -(g->unk320 - 0x60000);
}
static void ExtraBoss_CameraTrackObject(void) {
    ExObj *o = ExtraBoss_Unk_ov001_02300c54();
    ExWork *g = ExtraBoss_GetWork();
    s32 dx, dy;
    if (o == NULL)
        return;
    dx = o->unk18 - g->unk31C;
    dy = o->unk1C - g->unk320;
    if (dx < 0) {
        if (dx < -0x7800)
            dx = -0x7800;
        g->unk31C += dx;
    } else if (dx > 0) {
        if (dx > 0x7800)
            dx = 0x7800;
        g->unk31C += dx;
    }
    if (dy < 0) {
        if (dy < -0x7800)
            dy = -0x7800;
        g->unk320 += dy;
    } else if (dy > 0) {
        if (dy > 0x7800)
            dy = 0x7800;
        g->unk320 += dy;
    }
    data_ov001_0231f524 = -(g->unk31C - 0x80000);
    data_ov001_0231f520 = -(g->unk320 - 0x60000);
}
void ExtraBoss_UpdateCamera(void) {
    if (ExtraBoss_GetWork()->unk318 == 0) {
        ExtraBoss_CameraFollowPlayer();
        return;
    }
    ExtraBoss_CameraTrackObject();
}

/* ExtraBoss_InitDisplay (modified: VRAM banks, BG / OBJ modes and master brightness become the
 * native renderer's state; the zone lights are set as in the original) */
static dcm_lights_t s_lights;
const dcm_lights_t *ex_lights(void) { return &s_lights; }
static void ex_set_zone_lights(void) {
    u16 c = EX_DATA(0x02074960 + 0x9c, const u16 *)[0];
    const s16 *v = EX_DATA(0x020748bc + 0x90, const s16 *);
    int i;
    memset(&s_lights, 0, sizeof(s_lights));
    s_lights.count = 3;
    for (i = 0; i < 3; i++) {
        s_lights.dir[i][0] = v[3 * i] / 4096.0f;
        s_lights.dir[i][1] = v[3 * i + 1] / 4096.0f;
        s_lights.dir[i][2] = v[3 * i + 2] / 4096.0f;
        s_lights.col[i][0] = (c & 31) / 31.0f;
        s_lights.col[i][1] = ((c >> 5) & 31) / 31.0f;
        s_lights.col[i][2] = ((c >> 10) & 31) / 31.0f;
    }
}
static void ExtraBoss_InitDisplay(void) {
    g_ex.bright_main = g_ex.bright_sub = 0x10;
    ex_set_zone_lights();
}

/* -------------------------------------------------------------- title card */
/* StageTitleCard_CreateExtraBoss (UI module, not part of this translation): the integrator may show it
 * through g_boss_extra_hooks.title_card and call boss_extra_start_battle(); otherwise this stand-in
 * task waits BOSS_EXTRA_TITLE_FRAMES frames and fades the screen in, then calls ExtraBoss_StartBattle
 * as StageTitleCard_State_StartStage does. */
static ExTask *s_title_task;
static void ex_title_card_main(void) {
    if (g_ex.title_timer == BOSS_EXTRA_TITLE_FRAMES - 1)
        BrightnessLerp_Create(16, -16, 0, 3, NULL);
    if (--g_ex.title_timer > 0)
        return;
    ex_task_destroy(gTaskCurrent);
    s_title_task = NULL;
    ExtraBoss_StartBattle();
}
static void StageTitleCard_CreateExtraBoss(void) {
    if (g_boss_extra_hooks.title_card != NULL) {
        g_boss_extra_hooks.title_card(g_boss_extra_hooks.ctx, gExtraBossWork->unk3C);
        return;
    }
    g_ex.title_timer = BOSS_EXTRA_TITLE_FRAMES;
    s_title_task = ex_task_create(ex_title_card_main, 0x3efe, NULL);
}

/* ---------------------------------------------------------------- sound */
static void ex_music(s32 seq) {
    if (g_boss_hooks.music)
        g_boss_hooks.music(g_boss_hooks.ctx, seq);
    boss_log("music", seq, 0);
}

/* --------------------------------------------------------------- rounds */
static void ExtraBoss_ResetNodeStep(void) { data_ov001_0231f064 = 0; }
static void ExtraBoss_StartDummy(void) {}
static void ExtraBoss_ShutdownDummy(void) {}
void ExtraBoss_Main(void) { /* calls the title-card player object's callback (work + 0xd4): unused natively */ }

void ExtraBoss_ResetRound(void) {
    ExWork *g = gExtraBossWork;
    g->unk0 |= 2;
    g->unk0 &= ~0x74u;
    g->unk8->fn = ExtraBoss_Main;
    g->unkC->fn = NULL;
    g->unk10->fn = NULL;
    ExtraBossPlayer_Action_LockControl();
    g->unk4C = 0x32;
    g->unk4 = 0;
    g->unk44 = 0;
    g->unk318 = 0;
    g_ex.bright_main = g_ex.bright_sub = -0x10;
    ex_set_zone_lights();
    StageTitleCard_CreateExtraBoss();
    if (g->unk40 < 3)
        ex_music(0x14);
    else
        ex_music(0x15);
    g_bossex_stats.rounds_started++;
}

void ExtraBoss_SetTimeAndRings(s32 p0, s32 p1) {
    gExtraBossResultTime = p0;
    if (p0 > 0x8c9f)
        gExtraBossResultTime = 0x8c9f;
    else if (p0 < 0)
        gExtraBossResultTime = 0;
    gExtraBossResultRings = p1;
    if (p1 > 0x3e7)
        gExtraBossResultRings = 0x3e7;
    else if (p1 < 0)
        gExtraBossResultRings = 0;
}

void ExtraBoss_State_Battle(void) {
    ExWork *G = gExtraBossWork;
    u32 f;
    s32 t;
    if ((s_gpad[2] & 8) && !(G->unk0 & 8)) {
        g_ex.paused = 1;   /* PauseMenu_Create(0) */
        if (g_boss_extra_hooks.pause)
            g_boss_extra_hooks.pause(g_boss_extra_hooks.ctx);
        return;
    }
    G->unk18 = ExtraBoss_RemapPadBits(s_gpad[0]);
    G->unk1A = ExtraBoss_RemapPadBits(s_gpad[1]);
    G->unk1C = ExtraBoss_RemapPadBits(s_gpad[2]);
    G->unk1E = ExtraBoss_RemapPadBits(s_gpad[3]);
    G->unk20 = ExtraBoss_RemapPadBits(s_gpad[4]);
    ExtraBoss_ClearColliderLists();
    if (!(G->unk0 & 2)) {
        G->unk44++;
        if (!(G->unk0 & 0x10) && FX_ModS32(G->unk44, 0x3c) == 0) {
            G->unk4C--;
            if (G->unk4C < 0) {
                ExtraBossPlayer_Action_Defeat();
                G->unk0 |= 0x10;
                G->unk314 = 0x3c;
                Sound_PlaySeqArc(0, 0, 6);
                g_bossex_stats.player_downs++;
            }
        }
        if (!(G->unk0 & 0x10) && !g_ex.time_unlimited && G->unk44 >= 0x8c9f) {
            ExtraBossPlayer_Action_Defeat();
            G->unk0 |= 0x210;
            G->unk314 = 0x3c;
            G->unk48 = 0;
            Sound_PlaySeqArc(0, 0, 6);
            g_bossex_stats.player_downs++;
        }
    }
    ExtraBoss_UpdateBgScrollPalette();
    ExtraBoss_InitCharacterSlot(G->unk4);
    ExtraBossObj_UpdateMotionAll();
    ExtraBoss_Unk_ov001_02302658();
    ExtraBoss_UpdateSpawnWork();
    f = G->unk0;
    if (f & 0x20) {
        if (G->unk316 == 0)
            return;
        G->unk60[G->unk3C]--;
        ExtraBoss_StopSubScreenTasks();
        if (G->unk60[G->unk3C] < 0) {
            s_result.cleared = 0;
            g_ex.result = BOSS_EXTRA_FAILED;   /* ExtraBoss_Shutdown + GameOverSeq_StartGameOver */
            G->unk8->fn = NULL;
            G->unkC->fn = NULL;
            G->unk10->fn = NULL;
            return;
        }
        G->unk40 = FX_DivS32(G->unk40, 2) * 2;
        ExtraBossObj_DestroyPlayer();
        ExtraBossObj_DestroyKind1();
        ExtraBossObj_DestroyKind2And3();
        ExtraBoss_ClearSpawnLists();
        ExtraBoss_LoadPlayerModel(0);
        G->unk3C = 0;
        ExtraBoss_ResetRound();
        G->unk44 = G->unk48;
    } else if (f & 0x10) {
        if (G->unk314 > 0) {
            G->unk314--;
            return;
        }
        G->unk0 = f | 0x20;
        G->unk316 = 0;
        BrightnessLerp_Create(0x10, 0, -0x10, 3, &G->unk316);
    } else if (f & 0x100) {
        t = G->unk44;
        if (f & 0x200)
            t = 0x8c9f;
        ExtraBoss_SetTimeAndRings(t, G->unk4C);
        s_result.cleared = 1;
        g_ex.result = BOSS_EXTRA_CLEARED;   /* ExtraBoss_Shutdown + DemoScript_StartAfterBoss */
        G->unk8->fn = NULL;
        G->unkC->fn = NULL;
        G->unk10->fn = NULL;
        g_bossex_stats.rounds_won++;
    }
}
void ExtraBoss_PostUpdate_Battle(void) {
    ExtraBossObj_PropagateParentDestroy();
    ExtraBoss_UpdateCamera();
    ExtraBoss_UpdateBgWaveScroll();
    ExtraBoss_CollectRings();
    ExtraBoss_CheckAllCollisions();
    ExtraBossObj_DestroyFlagged();
}
void ExtraBoss_Draw_Battle(void) {
    if (gExtraBossWork->unk0 & 4)
        ExtraBoss_DrawHud();
    ExtraBossObj_DrawAll();
    ExtraBoss_DrawRings();
}

void ExtraBoss_StartBattle(void) {
    ExWork *g = gExtraBossWork;
    boss_sfx_stop(1);   /* NNS_SndPlayerStopSeq(work + 0x33c, 0) */
    g->unk8->fn = ExtraBoss_State_Battle;
    g->unkC->fn = ExtraBoss_PostUpdate_Battle;
    g->unk10->fn = ExtraBoss_Draw_Battle;
    g->unk0 &= ~0x72u;
    g->unk0 |= 4;
    ExtraBoss_ResetPlayerForBattle();
    ExtraBoss_Unk_ov001_02304fb4();
    ExtraBoss_ResetSpawnWork();
    ExtraBoss_ResetFxFlags();
    if (ExtraBoss_GetWork()->unk40 > 1) {
        g->unk50 = 0;
        g->unk54 = 0;
        g->unk58 = 0x180000;
        g->unk5C = 0x180000;
    } else {
        g->unk50 = 0x50000;
        g->unk54 = 0;
        g->unk58 = 0x100000;
        g->unk5C = 0x130000;
    }
    ExtraBoss_ResetCamera();
    ExtraBoss_UpdateCamera();
    if (ExtraBoss_GetWork()->unk40 > 1)
        ExtraBoss_SpawnBoss();
    ExtraBoss_Unk_ov001_02306424();
    g_ex.bright_main = 0;
}

void ExtraBoss_AdvancePhase(void) {
    ExWork *g = gExtraBossWork;
    g->unk40++;
    if (g->unk40 == 3)
        ex_music(0x15);
    g->unk3C = FX_ModS32(g->unk40, 2);
    ExtraBoss_LoadPlayerModel(g->unk3C);
    ExtraBoss_ResetCamera();
    BrightnessLerp_Create(0x10, 0x10, 0, 3, NULL);
    ExtraBoss_StartBattle();
    if (g->unk3C == 0)
        g->unk48 = g->unk44;
    g_bossex_stats.rounds_won++;
    g_bossex_stats.rounds_started++;
}

static void ExtraBoss_State_CharacterSwitch(void) {
    ExWork *g = gExtraBossWork;
    g->unk20 = 0;
    g->unk1E = g->unk20;
    g->unk1C = g->unk1E;
    g->unk1A = g->unk1C;
    g->unk18 = g->unk1A;
    ExtraBoss_ClearColliderLists();
    ExtraBoss_UpdateBgScrollPalette();
    ExtraBoss_InitCharacterSlot(g->unk4);
    ExtraBossObj_UpdateMotionAll();
    if (g->unk0 & 0x400) {
        g->unk0 &= ~0x400u;
        ExtraBoss_Unk_ov001_022ffcf8();
        ExtraBoss_AdvancePhase();
    }
}
static void ExtraBoss_PostUpdate_CharacterSwitch(void) {
    ExtraBossObj_PropagateParentDestroy();
    ExtraBoss_UpdateBgWaveScroll();
    ExtraBossObj_DestroyFlagged();
}
static void ExtraBoss_Draw_CharacterSwitch(void) { ExtraBossObj_DrawAll(); }

void ExtraBoss_StartCharacterSwitch(void) {
    ExWork *g = gExtraBossWork;
    g->unk8->fn = ExtraBoss_State_CharacterSwitch;
    g->unkC->fn = ExtraBoss_PostUpdate_CharacterSwitch;
    g->unk10->fn = ExtraBoss_Draw_CharacterSwitch;
    data_ov001_0231f520 = 0;
    data_ov001_0231f524 = 0;
    ExtraBoss_StopSubScreenTasks();
    g->unk0 &= ~4u;
    ExtraBossObj_DestroyPlayer();
    ExtraBossObj_DestroyKind1();
    ExtraBoss_ClearSpawnLists();
    if (FX_ModS32(g->unk40, 2) == 0)
        ExtraBoss_SetupSubDisplay(1);
    else
        ExtraBoss_SetupSubDisplay(0);
    ExtraBoss_CopyCharacterDefaults(1);
    ExtraBoss_CopyCharacterDefaults(2);
    g_bossex_stats.switches++;
}

/* ------------------------------------------------------------- start / end */
/* ExtraBoss_Start (modified: work from calloc, StoryProgress_RestoreFromSave replaced by the lives set
 * with boss_extra_set_lives, sound handle / archive / sound data handled by the integrator) */
void ExtraBoss_Start(void) {
    gExtraBossWork = (ExWork *)calloc(1, sizeof(ExWork));
    gExtraBossWork->unk0 |= 4;
    gExtraBossWork->unk4C = 0x32;
    gExtraBossWork->unk60[0] = (s8)g_ex.lives_in[0];
    gExtraBossWork->unk60[1] = (s8)g_ex.lives_in[1];
    g_ex.unk0 = 1;
    ExtraBoss_InitPadMap();
    ExtraBoss_InitDisplay();
    ExtraBoss_AllocCharacterSlots();
    ExtraBoss_ResetCamera();
    ExtraBossPlayerManager_Create();
    ExtraBoss_StartDummy();
    ExtraBoss_AllocBgWork();
    ExtraBoss_InitRings();
    ExtraBoss_Unk_ov001_022fdbc0();
    ExtraBoss_AllocFlagBlock();
    ExtraBoss_AllocSubScreenWork();
    ex_set_zone_lights();
    gExtraBossWork->unk50 = 0x50000;
    gExtraBossWork->unk54 = 0;
    gExtraBossWork->unk58 = 0x100000;
    gExtraBossWork->unk5C = 0x130000;
    gExtraBossWork->unk8 = ex_task_create(ExtraBoss_Main, 0x1000, NULL);
    gExtraBossWork->unkC = ex_task_create(NULL, 0xe000, NULL);
    gExtraBossWork->unk10 = ex_task_create(NULL, 0xe010, NULL);
    ExtraBoss_LoadPlayerModel(0);
    gExtraBossWork->unk3C = 0;
    ExtraBoss_ResetRound();
    ExtraBoss_ResetNodeStep();
}

void ExtraBoss_Shutdown(void) {
    ExWork *g = gExtraBossWork;
    s32 size;
    if (g == NULL)
        return;
    s_result.time = gExtraBossResultTime;
    s_result.rings = gExtraBossResultRings;
    s_result.lives_sonic = g->unk60[0];
    s_result.lives_blaze = g->unk60[1];
    s_result.round = g->unk40;
    ExtraBoss_FreeCharacterSlots();
    ExtraBossPlayerManager_Destroy();
    ExtraBoss_ShutdownDummy();
    ExtraBoss_FreeBgWork();
    ExtraBoss_FreeSpawnWork();
    ExtraBoss_FreeObjVramSlots();
    ExtraBoss_FreeFlagBlock();
    ExtraBoss_FreeSubScreenWork();
    ex_tasks_clear();
    /* SaveData_SetLivesBoth(unk60, unk61) (s_result); gGameState.unk0 = 1;
     * data_022c4590.unk14 (current lives) = Blaze's lives clamped to 2..99 */
    size = g->unk60[1];
    if (size > 99)
        size = 99;
    else if (size < 2)
        size = 2;
    g_ex.unk0 = 1;
    s_result.lives_current = size;
    free(g);
    gExtraBossWork = NULL;
}

/* ================================================================= API */
void boss_extra_set_paths(const char *model_dir, const char *data_dir) {
    strncpy(g_ex_model_dir, model_dir, sizeof(g_ex_model_dir) - 1);
    strncpy(g_ex_data_dir, data_dir, sizeof(g_ex_data_dir) - 1);
}
static int s_lives_set;
void boss_extra_set_lives(int sonic, int blaze) {
    s_lives_set = 1;
    g_ex.lives_in[0] = sonic;
    g_ex.lives_in[1] = blaze;
}
void boss_extra_set_difficulty(int level) { g_ex.unk14 = level; }

int boss_extra_init(int character) {
    extern int g_boss_upload_textures;
    void boss_model_set_dir(const char *d);
    boss_extra_shutdown();
    if (!s_lives_set)
        g_ex.lives_in[0] = g_ex.lives_in[1] = 3;
    g_ex.character_at_entry = character;
    g_ex.result = BOSS_EXTRA_RUNNING;
    g_ex.paused = 0;
    memset(&s_result, 0, sizeof(s_result));
    memset(&g_bossex_stats, 0, sizeof(g_bossex_stats));
    memset(&gBossStatus, 0, sizeof(gBossStatus));
    memset(s_gpad, 0, sizeof(s_gpad));
    gExtraBossResultTime = gExtraBossResultRings = 0;
    boss_model_set_dir(g_ex_model_dir);
#ifdef _arch_dreamcast
    g_boss_upload_textures = 1;
#else
    g_boss_upload_textures = 0;
#endif
    ex_tasks_clear();
    ex_draw_reset();
    if (ex_data_load() != 0)
        return -1;
    if (ExtraBoss_LoadAssets() != 0) {
        ExtraBoss_ReleaseAssets();
        return -2;
    }
    g_boss_lights = NULL;
    ExtraBoss_Start();
    s_active = 1;
    return 0;
}

static s32 ex_phase(void) {
    ExWork *g = gExtraBossWork;
    ExObj *b;
    if (g == NULL)
        return 0;
    if (g->unk0 & 0x100)
        return BOSSEX_PHASE_CLEAR;
    if (g->unk0 & 0x10)
        return BOSSEX_PHASE_PLAYER_DOWN;
    if (g->unk8->fn == ExtraBoss_Main)
        return BOSSEX_PHASE_TITLE;
    if (g->unk8->fn != ExtraBoss_State_Battle)
        return BOSSEX_PHASE_SWITCH;
    if (g->unk318 != 0)
        return BOSSEX_PHASE_DEFEAT;
    b = gExtraBossPlayerManager->unk8[0];
    if (b == NULL)
        return BOSSEX_PHASE_SCROLL;
    return (g->unk40 == 1 || g->unk40 == 3) ? BOSSEX_PHASE_HAND : BOSSEX_PHASE_WEAPON;
}

int boss_extra_run_frame(const InputState *in) {
    u16 pad, prev;
    if (!s_active || gExtraBossWork == NULL)
        return g_ex.result;
    if (g_ex.result != BOSS_EXTRA_RUNNING)
        return g_ex.result;
    if (in != NULL)
        g_ex.in = *in;
    if (g_ex.paused)
        return BOSS_EXTRA_RUNNING;
    /* gPad: held, released, pressed (trigger), -, repeat */
    prev = s_gpad[0];
    pad = in != NULL ? ex_ds_pad(in->held) : 0;
    s_gpad[0] = pad;
    s_gpad[1] = (u16)(prev & ~pad);
    s_gpad[2] = (u16)(pad & ~prev);
    s_gpad[4] = s_gpad[2];
    ex_draw_reset();
    if (data_ov001_0231f5a8 != NULL)
        data_ov001_0231f5a8->drawn = 0;
    ex_tasks_run();
    if (g_ex.result != BOSS_EXTRA_RUNNING) {
        ExtraBoss_Shutdown();
        ex_draw_reset();   /* the queued models were freed */
    }
    /* gBossStatus for boss_health() / boss_phase() */
    if (gExtraBossWork != NULL) {
        gBossStatus.unk8 = (u16)(gExtraBossPlayerManager->unk8[0] != NULL && gExtraBossWork->unk308 > 0
                                     ? gExtraBossWork->unk308 : 0);
        gBossStatus.unkA = (u16)gExtraBossWork->unk30C;
        gBossStatus.unk4 = (u32)ex_phase();
        gBossStatus.unk0 = gExtraBossWork->unk8->fn == ExtraBoss_State_Battle;
    } else {
        gBossStatus.unk4 = g_ex.result == BOSS_EXTRA_CLEARED ? BOSSEX_PHASE_CLEAR : 0;
        gBossStatus.unk8 = 0;
    }
    return g_ex.result;
}

void boss_extra_draw(int list) {
    if (!s_active)
        return;
    ex_draw_frame(list);
}

void boss_extra_shutdown(void) {
    if (gExtraBossWork != NULL)
        ExtraBoss_Shutdown();
    if (s_active) {
        ex_draw_shutdown();
        ExtraBoss_ReleaseAssets();
    }
    ex_tasks_clear();
    s_active = 0;
}
void boss_extra_result(BossExtraResult *r) { *r = s_result; }
void boss_extra_start_battle(void) {
    if (gExtraBossWork != NULL && gExtraBossWork->unk8->fn == ExtraBoss_Main)
        ExtraBoss_StartBattle();
}
int boss_extra_paused(void) { return g_ex.paused; }
void boss_extra_resume(void) { g_ex.paused = 0; }
int boss_extra_character(void) { return gExtraBossWork ? gExtraBossWork->unk3C : 0; }
int boss_extra_round(void) { return gExtraBossWork ? gExtraBossWork->unk40 : s_result.round; }
int boss_extra_rings(void) { return gExtraBossWork ? gExtraBossWork->unk4C : s_result.rings; }
int32_t boss_extra_time(void) { return gExtraBossWork ? gExtraBossWork->unk44 : s_result.time; }
