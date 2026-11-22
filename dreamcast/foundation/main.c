/*
 * Sonic Rush Dreamcast port — Milestone 2 hardware foundation.
 *
 * Boots under KallistiOS and exercises every hardware path the port needs:
 *   - KOS init, 640x480 video (VGA or composite/RGB, chosen by KOS from the cable)
 *   - PowerVR2: opaque + translucent lists, textured and untextured quads,
 *     three parallax layers, alpha blending
 *   - Maple controller: analog stick (with dead zone) and D-pad move the marker,
 *     A plays the sample, START quits
 *   - AICA: a Yamaha ADPCM WAV loaded into sound RAM and played by hardware
 *   - Original Sonic Rush asset: if the owner's converted assets are on the
 *     disc (/cd/assets/sprite.argb4444 + /cd/assets/sample.wav, produced by
 *     tools/dreamcast/convert_assets.py from their own ROM), they are used;
 *     otherwise built-in placeholder content is shown/played.
 *
 * The demo contains no Sonic Rush data itself.
 */
#include <kos.h>
#include <dc/pvr.h>
#include <dc/maple.h>
#include <dc/maple/controller.h>
#include <dc/sound/sound.h>
#include <dc/sound/sfxmgr.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SCREEN_W 640.0f
#define SCREEN_H 480.0f
#define SPRITE_TEX_W 32
#define SPRITE_TEX_H 32
#define DEADZONE 24

static pvr_ptr_t g_sprite_tex;
static int g_have_rom_sprite;
static sfxhnd_t g_sample = SFXHND_INVALID;
static int g_have_rom_sample;

/* Draw an axis-aligned quad. tex==NULL draws flat colour. */
static void draw_quad(pvr_list_t list, pvr_ptr_t tex, int tw, int th,
                      float x, float y, float w, float h, float z, uint32_t argb)
{
    pvr_poly_cxt_t cxt;
    pvr_poly_hdr_t hdr;
    pvr_vertex_t v;

    if(tex)
        pvr_poly_cxt_txr(&cxt, list, PVR_TXRFMT_ARGB4444 | PVR_TXRFMT_NONTWIDDLED,
                         tw, th, tex, PVR_FILTER_NONE);
    else
        pvr_poly_cxt_col(&cxt, list);
    pvr_poly_compile(&hdr, &cxt);
    pvr_prim(&hdr, sizeof(hdr));

    v.flags = PVR_CMD_VERTEX;
    v.argb = argb;
    v.oargb = 0;
    v.z = z;
    v.x = x;     v.y = y + h; v.u = 0.0f; v.v = 1.0f; pvr_prim(&v, sizeof(v));
    v.x = x;     v.y = y;     v.u = 0.0f; v.v = 0.0f; pvr_prim(&v, sizeof(v));
    v.x = x + w; v.y = y + h; v.u = 1.0f; v.v = 1.0f; pvr_prim(&v, sizeof(v));
    v.flags = PVR_CMD_VERTEX_EOL;
    v.x = x + w; v.y = y;     v.u = 1.0f; v.v = 0.0f; pvr_prim(&v, sizeof(v));
}

/* Placeholder sprite: an original 32x32 star-shaped marker (ARGB4444). */
static void make_placeholder_sprite(uint16_t *pix)
{
    int x, y;
    for(y = 0; y < SPRITE_TEX_H; y++)
        for(x = 0; x < SPRITE_TEX_W; x++) {
            int dx = x - 16, dy = y - 16;
            int ax = dx < 0 ? -dx : dx, ay = dy < 0 ? -dy : dy;
            int in = (ax + ay < 14) || (ax < 3 && ay < 15) || (ay < 3 && ax < 15);
            pix[y * SPRITE_TEX_W + x] = in ? (ax + ay < 6 ? 0xffff : 0xffc4) : 0x0000;
        }
}

static void load_sprite(void)
{
    uint16_t *pix = malloc(SPRITE_TEX_W * SPRITE_TEX_H * 2);
    file_t f = fs_open("/cd/assets/sprite.argb4444", O_RDONLY);
    g_have_rom_sprite = 0;
    if(f != FILEHND_INVALID) {
        if(fs_read(f, pix, SPRITE_TEX_W * SPRITE_TEX_H * 2) == SPRITE_TEX_W * SPRITE_TEX_H * 2)
            g_have_rom_sprite = 1;
        fs_close(f);
    }
    if(!g_have_rom_sprite)
        make_placeholder_sprite(pix);
    g_sprite_tex = pvr_mem_malloc(SPRITE_TEX_W * SPRITE_TEX_H * 2);
    pvr_txr_load(pix, g_sprite_tex, SPRITE_TEX_W * SPRITE_TEX_H * 2);
    free(pix);
    printf("sprite: %s\n", g_have_rom_sprite ? "/cd/assets/sprite.argb4444 (owner ROM)" : "placeholder");
}

static void load_sample(void)
{
    g_sample = snd_sfx_load("/cd/assets/sample.wav");
    g_have_rom_sample = g_sample != SFXHND_INVALID;
    if(!g_have_rom_sample)
        g_sample = snd_sfx_load("/cd/tone.wav");
    printf("sample: %s (%s)\n",
           g_have_rom_sample ? "/cd/assets/sample.wav (owner ROM)" : "/cd/tone.wav (placeholder)",
           g_sample != SFXHND_INVALID ? "loaded into AICA RAM" : "FAILED");
}

int main(int argc, char **argv)
{
    float px = SCREEN_W / 2 - 32, py = SCREEN_H / 2 - 32;
    float scroll = 0.0f;
    int frame = 0, prev_a = 0, quit = 0;

    pvr_init_defaults();
    snd_init();
    printf("Sonic Rush DC foundation: video %dx%d, cable %d\n",
           vid_mode->width, vid_mode->height, vid_check_cable());

    load_sprite();
    load_sample();
    if(g_sample != SFXHND_INVALID)
        snd_sfx_play(g_sample, 255, 128);

    while(!quit) {
        maple_device_t *cont = maple_enum_type(0, MAPLE_FUNC_CONTROLLER);
        if(cont) {
            cont_state_t *st = (cont_state_t *)maple_dev_status(cont);
            if(st) {
                int jx = st->joyx, jy = st->joyy;
                if(jx > -DEADZONE && jx < DEADZONE) jx = 0;
                if(jy > -DEADZONE && jy < DEADZONE) jy = 0;
                px += jx / 32.0f;
                py += jy / 32.0f;
                if(st->buttons & CONT_DPAD_LEFT)  px -= 3;
                if(st->buttons & CONT_DPAD_RIGHT) px += 3;
                if(st->buttons & CONT_DPAD_UP)    py -= 3;
                if(st->buttons & CONT_DPAD_DOWN)  py += 3;
                if((st->buttons & CONT_A) && !prev_a && g_sample != SFXHND_INVALID)
                    snd_sfx_play(g_sample, 255, (int)(px / SCREEN_W * 255));
                prev_a = st->buttons & CONT_A;
                if(st->buttons & CONT_START)
                    quit = 1;
            }
        }
        if(px < 0) px = 0;
        if(py < 0) py = 0;
        if(px > SCREEN_W - 64) px = SCREEN_W - 64;
        if(py > SCREEN_H - 64) py = SCREEN_H - 64;
        scroll += 1.0f;

        pvr_wait_ready();
        pvr_scene_begin();

        pvr_list_begin(PVR_LIST_OP_POLY);
        /* sky gradient bands + two parallax layers (far: 0.25x, near: 1x) */
        draw_quad(PVR_LIST_OP_POLY, NULL, 0, 0, 0, 0, SCREEN_W, 240, 1.0f, 0xff3060c0);
        draw_quad(PVR_LIST_OP_POLY, NULL, 0, 0, 0, 240, SCREEN_W, 240, 1.0f, 0xff204890);
        {
            int i;
            float off = -((int)(scroll * 0.25f) % 160);
            for(i = 0; i < 6; i++)
                draw_quad(PVR_LIST_OP_POLY, NULL, 0, 0, off + i * 160, 300, 120, 100, 2.0f, 0xff2c7a3a);
            off = -((int)scroll % 80);
            for(i = 0; i < 10; i++)
                draw_quad(PVR_LIST_OP_POLY, NULL, 0, 0, off + i * 80, 400, 60, 80, 3.0f, 0xff8a5a2a);
        }
        pvr_list_finish();

        pvr_list_begin(PVR_LIST_TR_POLY);
        /* translucent panel (alpha blend test) and the sprite (2x scale) */
        draw_quad(PVR_LIST_TR_POLY, NULL, 0, 0, 32, 32, 256, 48, 4.0f, 0x80000000);
        draw_quad(PVR_LIST_TR_POLY, g_sprite_tex, SPRITE_TEX_W, SPRITE_TEX_H,
                  px, py, 64, 64, 5.0f, 0xffffffff);
        pvr_list_finish();

        pvr_scene_finish();

        if(++frame % 300 == 0)
            printf("frame %d pos %.0f,%.0f\n", frame, px, py);
    }
    snd_sfx_unload_all();
    return 0;
}
