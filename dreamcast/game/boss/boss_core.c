/*
 * Shared boss framework:
 *   - bossdata.bin loader (ROM tables by DS address, node index table)
 *   - gBossStatus, gGameState/gDisplayMain fields used by bosses
 *   - Boss_GetStartHealth, Boss_Atan2, Boss_DrawExcBackground (state only),
 *     BossArena_ArcToCirclePos / BossArena_CirclePosToArc
 *   - ScreenShake (src/game/Graphics/Screen/ScreenShake_*.c, the shake the
 *     boss camera reads)
 *   - StageObj_Main for boss parts and effects (obj_main subset)
 *   - the public API (boss.h): stage set-up (BossStage1_OnModelLoaded,
 *     StageLoader boss path), update order, draw.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "boss_internal.h"

BossHooks g_boss_hooks;
BossStatus gBossStatus;
BossGame g_boss_game;
BossStats g_boss_stats;

static char s_data_dir[96] = "/cd/boss";
void boss_model_set_dir(const char *d);
extern int g_boss_upload_textures;

/* ------------------------------------------------------------ hooks */
void boss_sfx(s32 id) {
    if (g_boss_hooks.sfx)
        g_boss_hooks.sfx(g_boss_hooks.ctx, id);
}
void boss_sfx_handle(u32 handle, s32 id) {
    if (g_boss_hooks.sfx_handle)
        g_boss_hooks.sfx_handle(g_boss_hooks.ctx, (s32)handle, id);
    else
        boss_sfx(id);
}
void boss_sfx_stop(u32 handle) {
    if (g_boss_hooks.sfx_stop)
        g_boss_hooks.sfx_stop(g_boss_hooks.ctx, (s32)handle);
}
void boss_voice(s32 id) {
    if (g_boss_hooks.voice)
        g_boss_hooks.voice(g_boss_hooks.ctx, id);
}
void boss_log(const char *ev, s32 a, s32 b) {
    if (g_boss_hooks.log)
        g_boss_hooks.log(g_boss_hooks.ctx, ev, a, b);
}
static s32 s_navi_face;
void NaviMsg_SetFaceAnim(s32 face) { s_navi_face = face; }
void NaviMsg_ShowMessage(s32 msg, s32 time) {
    if (g_boss_hooks.navi)
        g_boss_hooks.navi(g_boss_hooks.ctx, s_navi_face, msg, time);
    boss_log("navi", msg, s_navi_face);
}
/* StageObj_SetPause / StageObj_ClearPause: gObjManagerFlags bit 0 */
void StageObj_SetPause(void) { g_dcp_world.objmgr |= 1; }
void StageObj_ClearPause(void) { g_dcp_world.objmgr &= ~1u; }
/* BossHelp_Create / BossHelp_Destroy (src/game/UI/Hud/BossHelp_*): the help sprite task
 * (gBossHelpTask) becomes UI state through g_boss_hooks.help; Destroy does nothing while
 * none is shown, as on the DS */
static s32 s_help = -1;
void BossHelp_Create(u32 idx) {
    s_help = (s32)idx;
    if (g_boss_hooks.help)
        g_boss_hooks.help(g_boss_hooks.ctx, BOSS_HELP_CREATE, (s32)idx, 0);
    boss_log("help", (s32)idx, 1);
}
void BossHelp_Destroy(void) {
    s32 idx = s_help;
    if (idx < 0)
        return;
    s_help = -1;
    if (g_boss_hooks.help)
        g_boss_hooks.help(g_boss_hooks.ctx, BOSS_HELP_DESTROY, idx, 0);
    boss_log("help", idx, 0);
}
s32 boss_help(void) { return s_help; }

/* ------------------------------------------------------- bossdata.bin */
typedef struct { u32 addr, size, off; } BossChunk;
static u8 *s_data;
static u32 s_data_size;
static const BossChunk *s_chunks;
static u32 s_nchunks;
typedef struct { char model[16], node[16]; u32 index; } BossJoint;
static const BossJoint *s_joints;
static u32 s_njoints;
typedef struct { u8 stage, pad[3]; char name[24], model[96], anim[96]; } BossEffFile;
static const BossEffFile *s_effs;
static u32 s_neffs;
typedef struct { char name[24]; u32 count; } BossAnimLen; /* followed by u16 frames[count], padded to 4 */
static const u8 *s_alens;
static u32 s_nalens;
/* converted material animations (bossdata.py sections 5 / 6) */
typedef struct { char name[24], file[96]; } BossMatAnimFile;
typedef struct { u8 stage, pad[3]; char name[24], file[3][96]; } BossEffMatFiles;
static const BossMatAnimFile *s_matfiles;
static u32 s_nmatfiles;
static const BossEffMatFiles *s_effmats;
static u32 s_neffmats;

static int boss_data_load(void) {
    char path[160];
    FILE *f;
    long n;
    u32 end;
    if (s_data)
        return 0;
    snprintf(path, sizeof(path), "%s/bossdata.bin", s_data_dir);
    f = fopen(path, "rb");
    if (!f) {
        printf("boss: cannot open %s\n", path);
        return -1;
    }
    fseek(f, 0, SEEK_END);
    n = ftell(f);
    fseek(f, 0, SEEK_SET);
    s_data = (u8 *)malloc((size_t)n);
    if (!s_data || fread(s_data, 1, (size_t)n, f) != (size_t)n || memcmp(s_data, "BOSD", 4) != 0) {
        fclose(f);
        free(s_data);
        s_data = NULL;
        return -1;
    }
    fclose(f);
    s_data_size = (u32)n;
    s_nchunks = *(u16 *)(s_data + 6);
    s_chunks = (const BossChunk *)(s_data + 8);
    end = 0;
    {
        u32 i;
        for (i = 0; i < s_nchunks; i++)
            if (s_chunks[i].off + s_chunks[i].size > end)
                end = s_chunks[i].off + s_chunks[i].size;
    }
    end = (end + 3) & ~3u;
    if (end + 4 <= s_data_size) {
        s_njoints = *(u32 *)(s_data + end);
        s_joints = (const BossJoint *)(s_data + end + 4);
        end += 4 + s_njoints * (u32)sizeof(BossJoint);
        if (end + 4 <= s_data_size) {
            s_neffs = *(u32 *)(s_data + end);
            s_effs = (const BossEffFile *)(s_data + end + 4);
            end += 4 + s_neffs * (u32)sizeof(BossEffFile);
            if (end + 4 <= s_data_size) {
                u32 i;
                s_nalens = *(u32 *)(s_data + end);
                s_alens = s_data + end + 4;
                end += 4;
                for (i = 0; i < s_nalens && end + sizeof(BossAnimLen) <= s_data_size; i++) {
                    const BossAnimLen *al = (const BossAnimLen *)(s_data + end);
                    end += (u32)sizeof(*al) + ((al->count * 2 + 3) & ~3u);
                }
                if (end + 4 <= s_data_size) {
                    s_nmatfiles = *(u32 *)(s_data + end);
                    s_matfiles = (const BossMatAnimFile *)(s_data + end + 4);
                    end += 4 + s_nmatfiles * (u32)sizeof(BossMatAnimFile);
                    if (end + 4 <= s_data_size) {
                        s_neffmats = *(u32 *)(s_data + end);
                        s_effmats = (const BossEffMatFiles *)(s_data + end + 4);
                        if (end + 4 + s_neffmats * (u32)sizeof(BossEffMatFiles) > s_data_size)
                            s_neffmats = 0;
                    }
                    if (end > s_data_size)
                        s_nmatfiles = 0;
                }
            }
        }
    }
    return 0;
}

const void *boss_data(u32 addr) {
    u32 i;
    for (i = 0; i < s_nchunks; i++)
        if (addr >= s_chunks[i].addr && addr < s_chunks[i].addr + s_chunks[i].size)
            return s_data + s_chunks[i].off + (addr - s_chunks[i].addr);
    printf("boss: ROM table %08x not in bossdata.bin\n", (unsigned)addr);
    {
        static const u8 zero[64];
        return zero;
    }
}

int boss_effect_file(int stage, int i, const char **name, const char **model, const char **anim) {
    u32 k;
    int n = 0;
    for (k = 0; k < s_neffs; k++)
        if (s_effs[k].stage == stage && n++ == i) {
            *name = s_effs[k].name;
            *model = s_effs[k].model;
            *anim = s_effs[k].anim[0] ? s_effs[k].anim : NULL;
            return 0;
        }
    return -1;
}

/* frame counts of an animation file that is not converted (timing only) */
int boss_anim_lengths(const char *name, const u16 **frames, u32 *count) {
    const u8 *p = s_alens;
    u32 i;
    for (i = 0; p != NULL && i < s_nalens; i++) {
        const BossAnimLen *a = (const BossAnimLen *)p;
        if (!strncmp(a->name, name, sizeof(a->name))) {
            u32 j;
            *frames = (const u16 *)(p + sizeof(*a));
            *count = a->count;
            /* a converted material / pattern / SRT set: read it now, with the boss resources, not in the
             * first boss_model_matstates that uses it (a disc read inside the draw: 10-17 ms each, up to
             * 45 ms frames in the z43 fight in Flycast) */
            for (j = 0; j < s_nmatfiles; j++)
                if (!strncmp(s_matfiles[j].name, a->name, 24) && s_matfiles[j].file[0]) {
                    boss_matanim_for_frames(*frames);
                    break;
                }
            return 0;
        }
        p += sizeof(*a) + ((a->count * 2 + 3) & ~3u);
    }
    return -1;
}

/* The converted material / pattern / SRT set of a timing-table entry (by its frames pointer, as
 * boss_anim_lengths returned it), loaded once and kept: Model_SetAnimation stores only the timing
 * resource, boss_model_matstates finds the values here.  NULL when not converted / not on the disc. */
const dca_set_t *boss_matanim_for_frames(const u16 *frames) {
    static struct {
        const u16 *frames;
        BossAnimRes res;
        int tried;
    } cache[64];
    static int ncache;
    const u8 *p = s_alens;
    u32 i;
    int k;
    if (frames == NULL)
        return NULL;
    for (k = 0; k < ncache; k++)
        if (cache[k].frames == frames)
            return cache[k].res.loaded ? &cache[k].res.set : NULL;
    for (i = 0; p != NULL && i < s_nalens; i++) {
        const BossAnimLen *a = (const BossAnimLen *)p;
        if ((const u16 *)(p + sizeof(*a)) == frames) {
            u32 j;
            if (ncache >= 64)
                return NULL;
            k = ncache++;
            cache[k].frames = frames;
            cache[k].tried = 1;
            for (j = 0; j < s_nmatfiles; j++)
                if (!strncmp(s_matfiles[j].name, a->name, 24) && s_matfiles[j].file[0]) {
                    char f[97];
                    memcpy(f, s_matfiles[j].file, 96);
                    f[96] = 0;
                    boss_res_load_anim(&cache[k].res, f);
                    break;
                }
            return cache[k].res.loaded ? &cache[k].res.set : NULL;
        }
        p += sizeof(*a) + ((a->count * 2 + 3) & ~3u);
    }
    return NULL;
}

/* the converted material (0), pattern (1), SRT (2) sets of a stage effect ("" when absent) */
int boss_effect_matanims(int stage, const char *name, const char *files[3]) {
    u32 i;
    for (i = 0; i < s_neffmats; i++)
        if (s_effmats[i].stage == stage && !strncmp(s_effmats[i].name, name, 24)) {
            files[0] = s_effmats[i].file[0];
            files[1] = s_effmats[i].file[1];
            files[2] = s_effmats[i].file[2];
            return 0;
        }
    return -1;
}

int boss_joint_index(const char *model, const char *node) {
    u32 i;
    for (i = 0; i < s_njoints; i++)
        if (!strncmp(s_joints[i].model, model, 16) && !strncmp(s_joints[i].node, node, 16))
            return s_joints[i].index == 0xffffffffu ? -1 : (int)s_joints[i].index;
    return -1;
}

/* ------------------------------------------------------ Boss_* helpers */
/* Boss_GetStartHealth (copied) */
s32 Boss_GetStartHealth(void) {
    dcp_bool b;
    s32 idx;
    b = g_dcp_world.mode == 2 || g_dcp_world.mode == 3;
    if (b)
        idx = 1;
    else
        idx = g_boss_game.unk14;
    if (idx == 1)
        return 8;
    return 6;
}

/* Boss_Atan2: FX_Atan2 */
s32 Boss_Atan2(s32 y, s32 x) { return dcp_atan2(y, x); }

/* Boss_DrawExcBackground: loads concentrated_line_0x.bbg (z?3_act archive
 * index `id`) into BG2 and shows it.  Native: the index is exposed through
 * boss_exc_background() for the renderer. */
void Boss_DrawExcBackground(u32 id) {
    g_boss_game.exc_bg = (s32)id;
    boss_log("exc_bg", (s32)id, 0);
}

/* BossArena_ArcToCirclePos (copied; boss arenas 2+ use it) */
void BossArena_ArcToCirclePos(s32 len, s32 b, s32 a, s32 *outX, s32 *outY, u16 *outAng) {
    u16 ang = (u16)(FX_Div(a, b) << 4);
    s32 s = SIN(ang);
    s32 c = COS(ang);
    s32 x = (s32)(((s64)len * s + 0x800) >> 12);
    s32 y = (s32)(((s64)len * c + 0x800) >> 12);
    if (outX != NULL)
        *outX = x;
    if (outY != NULL)
        *outY = y;
    if (outAng != NULL)
        *outAng = ang;
}

/* BossArena_CirclePosToArc (copied) */
void BossArena_CirclePosToArc(s32 unused, s32 len, s32 x, s32 y, s32 *out) {
    s32 t = (Boss_Atan2(x, y) << 12) / 0x10000;
    s32 r = (s32)(((s64)len * t + 0x800) >> 12);
    (void)unused;
    if (r < 0x40000)
        r += len;
    if (len + 0x40000 < r)
        r -= len;
    if (out != NULL)
        *out = r;
}

/* --------------------------------------------------------- ScreenShake */
typedef struct { u16 flags; s16 t; u8 mode; u8 pad5; s8 x; s8 y; } ScreenShakeWork;
static ScreenShakeWork s_shake;
static int s_shake_task;

void ScreenShake_Create(void) {
    memset(&s_shake, 0, sizeof(s_shake));
    s_shake_task = 1;
}
void ScreenShake_Stop(void) {
    if (!s_shake_task)
        return;
    s_shake.x = 0;
    s_shake.y = 0;
    s_shake.mode = 0;
    s_shake.flags = 0;
    s_shake.t = 0;
}
void ScreenShake_Start(u8 v) {
    if (!s_shake_task)
        return;
    if (v == 0) {
        ScreenShake_Stop();
        return;
    }
    if (s_shake.mode <= v) {
        s_shake.mode = v;
        s_shake.flags = 1;
        s_shake.t = 0;
    }
}
s8 ScreenShake_GetOffsetX(void) { return s_shake_task ? s_shake.x : 0; }
s8 ScreenShake_GetOffsetY(void) { return s_shake_task ? s_shake.y : 0; }

/* ScreenShake_Main (copied; tables data_02073240 / data_0207323c / data_0207324c from bossdata.bin) */
void ScreenShake_Main(void) {
    ScreenShakeWork *w = &s_shake;
    const s8 *data_0207323c = BOSS_DATA(0x0207323c, const s8 *);
    const s8 *data_02073240 = BOSS_DATA(0x02073240, const s8 *);
    const s8 *data_0207324c = BOSS_DATA(0x0207324c, const s8 *);
    if (!s_shake_task)
        return;
    w->x = 0;
    w->y = 0;
    if (!(w->flags & 1))
        return;
    switch (w->mode) {
    case 1:
        if (w->t > 8) {
            ScreenShake_Stop();
            break;
        }
        /* fallthrough */
    case 5:
        if (w->t >= 12) {
            ScreenShake_Stop();
            break;
        }
        /* fallthrough */
    case 9:
        if (w->t >= 12)
            w->t = 0;
        w->x = (data_02073240[w->t] & 0xf) >> 1;
        w->y = data_02073240[w->t] >> 5;
        break;
    case 2:
        if (w->t > 8) {
            ScreenShake_Stop();
            break;
        }
        /* fallthrough */
    case 6:
        if (w->t >= 12) {
            ScreenShake_Stop();
            break;
        }
        /* fallthrough */
    case 10:
        if (w->t >= 12)
            w->t = 0;
        w->x = data_02073240[w->t] & 0xf;
        w->y = data_02073240[w->t] >> 4;
        break;
    case 3:
        if (w->t >= 8) {
            ScreenShake_Stop();
            break;
        }
        /* fallthrough */
    case 7:
        if (w->t >= 16) {
            ScreenShake_Stop();
            break;
        }
        /* fallthrough */
    case 11:
        if (w->t >= 16)
            w->t = 0;
        w->x = data_0207323c[w->t & 3] & 0xf;
        w->y = data_0207323c[w->t & 3] >> 4;
        break;
    case 4:
        if (w->t >= 8) {
            ScreenShake_Stop();
            break;
        }
        /* fallthrough */
    case 8:
        if (w->t >= 0x16) {
            ScreenShake_Stop();
            break;
        }
        /* fallthrough */
    case 12:
        if (w->t >= 0x16)
            w->t = 0;
        w->x = data_0207324c[w->t] & 0xf;
        w->y = data_0207324c[w->t] >> 4;
        break;
    }
    if (w->x & 8)
        w->x = (s8)-(0x10 - w->x);
    w->t++;
}

/* ------------------------------------------------- StageObj_Main subset */
/* obj_manager.c obj_main for boss parts and boss effects (no sprite, no
 * ride/touch, no map collision: every boss part sets move_flags 0x100). */
void boss_obj_main(DcPlayer *h, BossFn *state, const BossObjOps *ops, void *self, int *alive) {
    u16 saved = 0;
    int restore = 0;
    if (h->obj_flags & 4) {
        if (ops->dtor)
            ops->dtor(self);
        *alive = 0;
        return;
    }
    if (h->obj_flags & 8)
        h->obj_flags |= 4;
    if ((h->obj_flags & 0x10) == 0) {
        if (obj_view_out(h->x, h->y, 0, 0, 0, 0, 0))
            h->obj_flags |= 4;
    }
    if (!obj_pause_check(h->obj_flags)) {
        if (h->hitstop != 0) {
            if (h->hitstop & 0xf)
                h->hitstop--;
            if (h->hitstop & 0xf0)
                h->hitstop -= 0x10;
        }
        if (!obj_pause_check(h->obj_flags)) {
            if ((h->hitstop & 0xf) == 0 && (h->obj_flags & 0x80) == 0 && *state != NULL)
                (*state)(self);
            if ((h->move_flags & 0x2000) == 0)
                dcp_obj_move(h);
            /* map collision skipped: move_flags 0x100 on every boss object */
        }
    }
    if ((h->hitstop & 0xf) || obj_pause_check(h->obj_flags)) {
        saved = h->disp_flags;
        h->disp_flags = saved | 0x10;
        restore = 1;
    }
    if ((h->obj_flags & 4) == 0) {
        if (ops->draw != NULL)
            ops->draw(self);
    }
    if (restore)
        h->disp_flags = saved;
    if (obj_pause_check(h->obj_flags))
        return;
    if ((h->hitstop & 0xf) == 0 && (g_dcp_world.objmgr & 0x10) && ops->collide != NULL)
        ops->collide(self);
    h->push_x = 0;
    h->push_y = 0;
    h->push_z = 0;
    h->shake_x = 0;
    h->shake_y = 0;
}

/* ------------------------------------------- StageObj.unk124 registry */
static struct { const DcPlayer *o; const BossModel *m; } s_objmdl[16];
void boss_obj_set_model(const DcPlayer *o, const BossModel *m) {
    int i, fr = -1;
    for (i = 0; i < 16; i++) {
        if (s_objmdl[i].o == o) {
            s_objmdl[i].m = m;
            if (m == NULL)
                s_objmdl[i].o = NULL;
            return;
        }
        if (s_objmdl[i].o == NULL && fr < 0)
            fr = i;
    }
    if (m != NULL && fr >= 0) {
        s_objmdl[fr].o = o;
        s_objmdl[fr].m = m;
    }
}
const fx32 *boss_obj_model_trans(const DcPlayer *o) {
    int i;
    if (o == NULL)
        return NULL;
    if (o == g_boss_game.player && g_boss_game.player_trans_valid)
        return g_boss_game.player_trans; /* gPlayers->unk124 = the player model (unk17C) */
    for (i = 0; i < 16; i++)
        if (s_objmdl[i].o == o)
            return s_objmdl[i].m->trans;
    return NULL;
}

/* ------------------------------------------------------------ API */
static int s_stage = -1, s_boss_kind;

void boss_set_paths(const char *model_dir, const char *data_dir) {
    boss_model_set_dir(model_dir);
    strncpy(s_data_dir, data_dir, sizeof(s_data_dir) - 1);
}
const char *boss_data_dir(void) { return s_data_dir; }
void boss_set_difficulty(int level_idx) { g_boss_game.unk14 = level_idx; }

static const ObjParam *find_boss_object(const uint8_t *level, u16 type, ObjParam *out) {
    const rlvl_header_t *h = (const rlvl_header_t *)level;
    const rlvl_object_t *o;
    u32 i;
    if (level == NULL)
        return NULL;
    o = (const rlvl_object_t *)rlvl_section(level, RLVL_SEC_OBJECTS, NULL);
    for (i = 0; o != NULL && i < h->num_objects; i++)
        if (o[i].type == type) {
            memset(out, 0, sizeof(*out));
            out->x0 = (u8)(o[i].x & 0xff); /* region-local; only the 0xff/0xff test reads it */
            out->y0 = (u8)(o[i].y & 0xff);
            out->type = o[i].type;
            out->flags = o[i].flags;
            out->p6 = o[i].p0;
            out->p7 = o[i].p1;
            out->p8 = o[i].p2;
            out->p9 = o[i].p3;
            out->ax = o[i].x;
            out->ay = o[i].y;
            return out;
        }
    return NULL;
}

int boss_init(int stage_id, const uint8_t *level, DcPlayer *player, int textures) {
    ObjParam prm;
    const ObjParam *p;
    boss_shutdown();
    memset(&gBossStatus, 0, sizeof(gBossStatus));
    memset(&g_boss_stats, 0, sizeof(g_boss_stats));
    g_boss_game.player = player;
    g_boss_game.bright = 0;
    g_boss_game.exc_bg = -1;
    g_boss_game.cleared = 0;
    g_boss_upload_textures = textures;
    g_boss_game.draw_player = NULL;
    g_boss_game.player_trans_valid = 0;
    g_boss_lights = NULL;
    if (boss_data_load() != 0)
        return -2;
    s_stage = stage_id;
    /* Camera_Create: boss arena size for zone modes 1..3 (gCamera.unkE0 / unkE4) */
    switch (stage_id / 3) {
    case 3:
        g_boss_game.arena_len = 0x128fb8;
        g_boss_game.arena_circ = 0x749fed;
        break;
    case 2:
        g_boss_game.arena_len = 0xf42f8;
        g_boss_game.arena_circ = 0x5fe435;
        break;
    case 1:
        g_boss_game.arena_len = 0x1152f0;
        g_boss_game.arena_circ = 0x6cd988;
        break;
    default:
        g_boss_game.arena_len = g_boss_game.arena_circ = 0;
        break;
    }
    /* Stage_Create: ScreenShake_Create, BossCamera_Create(act == 2 ? 2 : 0) */
    ScreenShake_Create();
    BossCamera_Create((stage_id % 3) == 2 ? 2 : 0);
    boss_effects_init();
    switch (stage_id) {
    case 2: /* z13 Leaf Storm act 3: Egg Hammer (map object 0x3f -> Boss1_Create) */
        if (boss_effects_load(stage_id) != 0 || boss1_load() != 0)
            return -3;
        p = find_boss_object(level, 0x3f, &prm);
        if (p == NULL) {
            memset(&prm, 0, sizeof(prm));
            prm.type = 0x3f;
            p = &prm;
        }
        if (Boss1_Create(p, (s32)p->ax << 8, (s32)p->ay << 8) == NULL)
            return -4;
        s_boss_kind = 1;
        return 0;
    case 5: /* z23 Water Palace act 3: Egg Turtle (map object 0x3e -> Boss2_Create) */
        if (boss_effects_load(stage_id) != 0 || boss2_load() != 0)
            return -3;
        p = find_boss_object(level, 0x3e, &prm);
        if (p == NULL) {
            memset(&prm, 0, sizeof(prm));
            prm.type = 0x3e;
            p = &prm;
        }
        s_boss_kind = 2;
        if (Boss2_Create(p, (s32)p->ax << 8, (s32)p->ay << 8) == NULL)
            return -4;
        return 0;
    case 14: /* z53 Huge Crisis act 3: map object 0x3b -> Boss1_Create (zone-5 variant, Boss5PartD3 missiles) */
        if (boss_effects_load(stage_id) != 0 || boss1_load() != 0)
            return -3;
        p = find_boss_object(level, 0x3b, &prm);
        if (p == NULL) {
            memset(&prm, 0, sizeof(prm));
            prm.type = 0x3b;
            p = &prm;
        }
        if (Boss1_Create(p, (s32)p->ax << 8, (s32)p->ay << 8) == NULL)
            return -4;
        s_boss_kind = 1;
        return 0;
    case 8: /* z33 Mirage Road act 3: map object 0x3d -> Boss3_Create */
        if (boss_effects_load(stage_id) != 0 || boss3_load() != 0)
            return -3;
        p = find_boss_object(level, 0x3d, &prm);
        if (p == NULL) {
            memset(&prm, 0, sizeof(prm));
            prm.type = 0x3d;
            p = &prm;
        }
        s_boss_kind = 3;
        if (Boss3_Create(p, (s32)p->ax << 8, (s32)p->ay << 8) == NULL)
            return -4;
        return 0;
    case 11: /* z43 Night Carnival act 3: map object 0x3c -> Boss4_Create */
        if (boss_effects_load(stage_id) != 0 || boss4_load() != 0)
            return -3;
        p = find_boss_object(level, 0x3c, &prm);
        if (p == NULL) {
            memset(&prm, 0, sizeof(prm));
            prm.type = 0x3c;
            p = &prm;
        }
        s_boss_kind = 4;
        if (Boss4_Create(p, (s32)p->ax << 8, (s32)p->ay << 8) == NULL)
            return -4;
        return 0;
    case 17: /* z63 Altitude Limit act 3: map object 0x3a -> Boss6_Create (part 0xd4: Boss6PartD4) */
        if (boss_effects_load(stage_id) != 0 || boss6_load() != 0)
            return -3;
        p = find_boss_object(level, 0x3a, &prm);
        if (p == NULL) {
            memset(&prm, 0, sizeof(prm));
            prm.type = 0x3a;
            p = &prm;
        }
        s_boss_kind = 6;
        if (Boss6_Create(p, (s32)p->ax << 8, (s32)p->ay << 8) == NULL)
            return -4;
        return 0;
    case 20: /* z73 Dead Line act 3: map object 0x39 -> Boss7_Create (the rival 0xd5, parts 0xd6 / 0xd7) */
        if (boss_effects_load(stage_id) != 0 || boss7_load() != 0)
            return -3;
        p = find_boss_object(level, 0x39, &prm);
        if (p == NULL) {
            memset(&prm, 0, sizeof(prm));
            prm.type = 0x39;
            p = &prm;
        }
        s_boss_kind = 7;
        if (Boss7_Create(p, (s32)p->ax << 8, (s32)p->ay << 8) == NULL)
            return -4;
        return 0;
    case 23: /* zf3 final zone boss (DS zone 7 act 2): map object 0x38 -> BossF_Create (parts 0xd8..0xde) */
        bossf_init();
        if (boss_effects_load(stage_id) != 0 || bossf_load() != 0)
            return -3;
        if (level != NULL)
            bossf_set_map_width(((const rlvl_header_t *)level)->width_blocks); /* gStageMapCollision.unk18 */
        p = find_boss_object(level, 0x38, &prm);
        if (p == NULL) {
            memset(&prm, 0, sizeof(prm));
            prm.type = 0x38;
            p = &prm;
        }
        s_boss_kind = 0xf;
        if (BossF_Create(p, (s32)p->ax << 8, (s32)p->ay << 8) == NULL)
            return -4;
        return 0;
    default:
        return -1;
    }
}

void boss_shutdown(void) {
    if (s_boss_kind == 1) {
        boss1_destroy();
        boss1_unload();
    }
    if (s_boss_kind == 2) {
        boss2_destroy();
        boss2_unload();
    }
    if (s_boss_kind == 3) {
        boss3_destroy();
        boss3_unload();
    }
    if (s_boss_kind == 4) {
        boss4_destroy();
        boss4_unload();
    }
    if (s_boss_kind == 6) {
        boss6_destroy();
        boss6_unload();
    }
    if (s_boss_kind == 7) {
        boss7_destroy();
        boss7_unload();
    }
    if (s_boss_kind == 0xf) {
        bossf_destroy();
        bossf_unload();
    }
    BossHelp_Destroy();
    memset(s_objmdl, 0, sizeof(s_objmdl));
    g_boss_game.draw_player = NULL;
    g_boss_game.player_trans_valid = 0;
    g_boss_lights = NULL;
    boss_effects_shutdown();
    BossCamera_Destructor();
    s_boss_kind = 0;
    s_stage = -1;
}

void boss_start(void) { gBossStatus.unk0 |= 1; }

void boss_update_pre(DcPlayer *player) {
    g_boss_game.player = player;
    if (s_boss_kind == 0xf)
        bossf_update_pre();  /* BossFPartD8 0xf0 */
}

void boss_update(DcPlayer *player) {
    g_boss_game.player = player;
    /* the player's StageObj_Main (task 0x100) ran before: its draw step,
     * gPlayers->unk70 (Boss2_Draw_PlayerOnArena) */
    if (g_boss_game.draw_player != NULL && player != NULL)
        g_boss_game.draw_player();
    if (s_boss_kind == 1)
        boss1_update();      /* 0x500 / 0x501 / 0x502 */
    if (s_boss_kind == 2)
        boss2_update();      /* Boss2Water 0x4ff, Boss2 0x500, Boss2WaterLight 0x500 */
    if (s_boss_kind == 3)
        boss3_update();      /* Boss3 0x500, Boss3PartCC 0x501, Boss3PartCD 0x502 */
    if (s_boss_kind == 4)
        boss4_update();      /* Boss4 0x500, parts 0x501..0x505 */
    if (s_boss_kind == 6)
        boss6_update();
    if (s_boss_kind == 7)
        boss7_update();      /* Boss7 0x500, Boss7PartD5 0x501, D6 / D7 0x502, frame counter 0x503,
                                StageZoneScript_Zone7Boss (0x3eff) */      /* Boss6 0x500, Boss6PartD4 0x502, Boss6 pieces 0xa00 */
    if (s_boss_kind == 0xf)
        bossf_update();      /* (BossFPartD8 0xf0 if boss_update_pre was not called), BossF 0x500, parts 0x501 */
    boss_effects_update();   /* 0xa00 */
    ScreenShake_Main();      /* 0x3e00 */
    if (s_boss_kind == 3)
        boss3_zone_script(); /* StageZoneScript_Zone3Boss 0x3eff */
    if (s_boss_kind == 0xf)
        bossf_zone_script(); /* StageZoneScript_ZoneFBoss 0x3eff */
    BossCamera_Main();       /* 0x3f00 */
}

int boss_draw(int list) {
    int n = 0;
    if (s_boss_kind == 1)
        n += boss1_draw(list);
    if (s_boss_kind == 2)
        n += boss2_draw(list);
    if (s_boss_kind == 3)
        n += boss3_draw(list);
    if (s_boss_kind == 4)
        n += boss4_draw(list);
    if (s_boss_kind == 6)
        n += boss6_draw(list);
    if (s_boss_kind == 7)
        n += boss7_draw(list);
    if (s_boss_kind == 0xf)
        n += bossf_draw(list);
    n += boss_effects_draw(list);
    if (data_ov000_0236a444.unk0 & 1)
        n += boss_model_draw(&data_ov000_0236a444.unkE0, list);
    return n;
}

s32 boss_health(void) { return gBossStatus.unk8; }
s32 boss_health_max(void) { return gBossStatus.unkA; }
s32 boss_phase(void) { return (s32)gBossStatus.unk4; }
int boss_defeated(void) {
    switch (s_boss_kind) {
    case 2:
        return gBossStatus.unk4 == BOSS2_PHASE_DEFEAT;
    case 3:
        return gBossStatus.unk4 == BOSS3_PHASE_DEFEAT;
    case 4:
        return gBossStatus.unk4 == BOSS4_PHASE_DEFEAT;
    case 6:
        return gBossStatus.unk4 == BOSS6_PHASE_DEFEAT;
    case 7:
        return boss7_defeated(); /* Boss7_State_ClashWon ran */
    case 0xf:
        return gBossStatus.unk4 == 0x3b; /* BOSSF_PHASE_DEFEAT */
    default:
        return gBossStatus.unk4 == BOSS_PHASE_DEFEAT;
    }
}
int boss_cleared(void) { return g_boss_game.cleared; }
void boss_shake_offset(s32 *x, s32 *y) {
    *x = ScreenShake_GetOffsetX();
    *y = ScreenShake_GetOffsetY();
}
s32 boss_brightness(void) { return g_boss_game.bright; }
s32 boss_exc_background(void) { return g_boss_game.exc_bg; }
const BossStats *boss_stats(void) { return &g_boss_stats; }
const char *boss_state_name(int part) {
    switch (s_boss_kind) {
    case 1:
        return boss1_state_name(part);
    case 2:
        return boss2_state_name(part);
    case 3:
        return boss3_state_name(part);
    case 4:
        return boss4_state_name(part);
    case 6:
        return boss6_state_name(part);
    case 7:
        return boss7_state_name(part);
    case 0xf:
        return bossf_state_name(part);
    }
    return "-";
}
void boss_hammer_pos(s32 *x, s32 *y, s32 *z) {
    if (s_boss_kind == 1)
        boss1_hammer_pos(x, y, z);
    else if (s_boss_kind == 2)
        boss2_body_pos(x, y, z);
    else if (s_boss_kind == 3)
        boss3_body_pos(x, y, z);
    else if (s_boss_kind == 4)
        boss4_body_pos(x, y, z);
    else if (s_boss_kind == 6)
        boss6_body_pos(x, y, z);
    else if (s_boss_kind == 7)
        boss7_body_pos(x, y, z);
    else if (s_boss_kind == 0xf)
        bossf_body_pos(x, y, z);
    else
        *x = *y = *z = 0;
}
int boss_hammer_box_armed(int box) {
    return s_boss_kind == 1   ? boss1_hammer_box_armed(box)
           : s_boss_kind == 2 ? boss2_box_armed(box)
           : s_boss_kind == 3 ? boss3_box_armed(box)
           : s_boss_kind == 4 ? boss4_box_armed(box)
           : s_boss_kind == 6 ? boss6_box_armed(box)
           : s_boss_kind == 7 ? boss7_box_armed(box)
           : s_boss_kind == 0xf ? bossf_box_armed(box)
                              : 0;
}

int boss_player_model(s32 trans[3], s32 rot[9]) {
    int i;
    if (!g_boss_game.player_trans_valid)
        return 0;
    for (i = 0; i < 3; i++)
        trans[i] = g_boss_game.player_trans[i];
    for (i = 0; i < 9; i++)
        rot[i] = g_boss_game.player_rot.m[i];
    return 1;
}
int boss_lights(dcm_lights_t *out) {
    if (s_boss_kind == 2)
        boss2_build_lights();
    if (g_boss_lights == NULL)
        return 0;
    *out = *g_boss_lights;
    return 1;
}
s32 boss_water_level(void) { return s_boss_kind == 2 ? (s32)g_obj_camera[0].water : -1; }
